#include "muxer/async_webm_muxer.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>

#include "audio/opus.hpp"
#include "constants.hpp"
#include "frame.hpp"
#include "muxer/audio_producer.hpp"
#include "muxer/multi_channel_vpx_video_producer.hpp"
#include "muxer/no_video_producer.hpp"
#include "muxer/opus_audio_producer.hpp"
#include "muxer/video_producer.hpp"
#include "muxer/vpx_video_producer.hpp"
#include "report/reporter.hpp"
#include "webm/output/context.hpp"

namespace hisui::muxer {

AsyncWebMMuxer::AsyncWebMMuxer(const hisui::Config& t_config,
                               const double t_duration)
    : m_config(t_config), m_duration(t_duration) {}

AsyncWebMMuxer::AsyncWebMMuxer(const hisui::Config& t_config,
                               hisui::MetadataSet* t_metadata_set)
    : m_config(t_config), m_metadata_set(t_metadata_set) {
  m_duration = m_metadata_set->getMaxStopTimeOffset();
  m_audio_archives = m_metadata_set->getArchives();
  m_normal_archive_size = std::size(m_metadata_set->getNormalArchives());
}

void AsyncWebMMuxer::setVideoProducer(
    std::shared_ptr<VideoProducer> t_video_producer) {
  m_video_producer = t_video_producer;
}

void AsyncWebMMuxer::setAudioArchives(
    const std::vector<hisui::Archive>& t_audio_archives) {
  m_audio_archives = t_audio_archives;
}

void AsyncWebMMuxer::setUp() {
  if (m_config.out_filename == "") {
    std::filesystem::path metadata_path(m_config.in_metadata_filename);
    if (m_config.audio_only) {
      m_config.out_filename = metadata_path.replace_extension(".weba");
    } else {
      m_config.out_filename = metadata_path.replace_extension(".webm");
    }
  }

  m_context = new hisui::webm::output::Context(m_config.out_filename);
  m_context->init();

  if (!m_video_producer) {
    if (!m_metadata_set) {
      throw std::runtime_error("m_metadata_set is null");
    }
    if (m_config.audio_only) {
      m_video_producer = std::make_shared<NoVideoProducer>();
    } else {
      if (m_config.out_video_bit_rate == 0) {
        m_config.out_video_bit_rate =
            static_cast<std::uint32_t>(m_normal_archive_size) *
            hisui::Constants::VIDEO_VPX_BIT_RATE_PER_FILE;
      }

      if (m_metadata_set->hasPreferred()) {
        m_video_producer = std::make_shared<MultiChannelVPXVideoProducer>(
            m_config,
            MultiChannelVPXVideoProducerParameters{
                .normal_archives = m_metadata_set->getNormal().getArchives(),
                .preferred_archives =
                    m_metadata_set->getPreferred().getArchives(),
                .max_stop_time_offset = m_metadata_set->getMaxStopTimeOffset(),
            });
      } else {
        m_video_producer = std::make_shared<VPXVideoProducer>(
            m_config, VPXVideoProducerParameters{
                          .archives = m_metadata_set->getNormal().getArchives(),
                          .max_stop_time_offset =
                              m_metadata_set->getMaxStopTimeOffset()});
      }
    }
  }

  m_context->setVideoTrack(m_video_producer->getWidth(),
                           m_video_producer->getHeight(),
                           m_video_producer->getFourcc());

  auto audio_producer = std::make_shared<OpusAudioProducer>(
      m_config, m_audio_archives, m_duration);
  const auto skip = audio_producer->getSkip();
  m_audio_producer = audio_producer;

  const auto private_data =
      hisui::audio::create_opus_private_data({.skip = skip});

  m_context->setAudioTrack(static_cast<std::uint64_t>(skip) *
                               hisui::Constants::NANO_SECOND /
                               hisui::Constants::PCM_SAMPLE_RATE,
                           private_data.data(), std::size(private_data));

  if (hisui::report::Reporter::hasInstance()) {
    hisui::report::Reporter::getInstance().registerOutput({
        .container = "WebM",
        .video_codec =
            m_config.audio_only ? "none"
            : m_video_producer->getFourcc() == hisui::Constants::VP9_FOURCC
                ? "vp9"
                : "vp8",
        .audio_codec = "opus",
        .duration = m_metadata_set->getMaxStopTimeOffset(),
    });
  }
}

AsyncWebMMuxer::~AsyncWebMMuxer() {
  delete m_context;
}

void AsyncWebMMuxer::appendAudio(hisui::Frame frame) {
  m_context->addAudioFrame(frame.data, frame.data_size, frame.timestamp);
  delete[] frame.data;
  m_audio_producer->bufferPop();
}

void AsyncWebMMuxer::appendVideo(hisui::Frame frame) {
  m_context->addVideoFrame(frame.data, frame.data_size, frame.timestamp,
                           frame.is_key);
  delete[] frame.data;
  m_video_producer->bufferPop();
}

void AsyncWebMMuxer::run() {
  mux();
}

void AsyncWebMMuxer::cleanUp() {}

void AsyncWebMMuxer::muxFinalize() {}

}  // namespace hisui::muxer
