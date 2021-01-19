#include "muxer/async_webm_muxer.hpp"

#include <cxxabi.h>
#include <spdlog/spdlog.h>

#include <array>
#include <chrono>
#include <filesystem>
#include <future>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>

#include "audio/opus.hpp"
#include "constants.hpp"
#include "frame.hpp"
#include "muxer/audio_producer.hpp"
#include "muxer/opus_audio_producer.hpp"
#include "muxer/video_producer.hpp"
#include "muxer/vpx_video_producer.hpp"
#include "webm/output/context.hpp"

namespace hisui::muxer {

AsyncWebMMuxer::AsyncWebMMuxer(const hisui::Config& t_config,
                               const hisui::Metadata& t_metadata)
    : m_config(t_config), m_metadata(t_metadata) {}

void AsyncWebMMuxer::setUp() {
  if (m_config.out_filename == "") {
    std::filesystem::path metadata_path(m_config.in_metadata_filename);
    auto webm_path = metadata_path.replace_extension(".webm");
    m_config.out_filename = webm_path;
  }

  m_file = std::fopen(m_config.out_filename.c_str(), "wb");
  if (!m_file) {
    throw std::runtime_error("Unable to open: " + m_config.out_filename);
  }
  m_context = new hisui::webm::output::Context(m_file);

  if (m_config.out_video_bit_rate == 0) {
    m_config.out_video_bit_rate =
        static_cast<std::uint32_t>(std::size(m_metadata.getArchives())) *
        hisui::Constants::VIDEO_VPX_BIT_RATE_PER_FILE;
  }

  m_video_producer = new VPXVideoProducer(m_config, m_metadata);
  m_context->setVideoTrack(m_video_producer->getWidth(),
                           m_video_producer->getHeight(),
                           m_video_producer->getFourcc());
  OpusAudioProducer* audio_producer =
      new OpusAudioProducer(m_config, m_metadata);
  const auto skip = audio_producer->getSkip();
  m_audio_producer = audio_producer;

  const auto private_data =
      hisui::audio::create_opus_private_data({.skip = skip});

  m_context->setAudioTrack(static_cast<std::uint64_t>(skip) *
                               hisui::Constants::NANO_SECOND /
                               hisui::Constants::PCM_SAMPLE_RATE,
                           private_data.data(), std::size(private_data));
}

AsyncWebMMuxer::~AsyncWebMMuxer() {
  delete m_context;
  std::fclose(m_file);

  delete m_video_producer;
  delete m_audio_producer;
}

void AsyncWebMMuxer::addAndConsumeAudio(std::uint8_t* data,
                                        const std::size_t data_length,
                                        const std::uint64_t timestamp) {
  m_context->addAudioFrame(data, data_length, timestamp);
  delete[] data;
  m_audio_producer->bufferPop();
}

void AsyncWebMMuxer::addAndConsumeVideo(std::uint8_t* data,
                                        const std::size_t data_length,
                                        const std::uint64_t timestamp,
                                        const bool is_keyframe) {
  m_context->addVideoFrame(data, data_length, timestamp, is_keyframe);
  delete[] data;
  m_video_producer->bufferPop();
}

void AsyncWebMMuxer::run() {
  const auto video_future = std::async(
      std::launch::async, &VPXVideoProducer::produce, m_video_producer);

  const auto audio_future = std::async(
      std::launch::async, &OpusAudioProducer::produce, m_audio_producer);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  bool video_finished = false;

  while (!m_audio_producer->isFinished()) {
    const auto audio_front = m_audio_producer->bufferFront();
    if (!audio_front.has_value()) {
      spdlog::debug("audio queue is empty");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }
    const auto audio_timestamp = audio_front.value().timestamp;

    if (video_finished) {
      addAndConsumeAudio(audio_front.value().data,
                         audio_front.value().data_size, audio_timestamp);
      continue;
    }

    if (m_video_producer->isFinished()) {
      video_finished = true;
      addAndConsumeAudio(audio_front.value().data,
                         audio_front.value().data_size, audio_timestamp);
      continue;
    }

    const auto video_front = m_video_producer->bufferFront();
    if (!video_front.has_value()) {
      spdlog::debug("video queue is empty (1)");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }
    const auto video_timestamp = video_front.value().timestamp;

    if (video_timestamp <= audio_timestamp) {
      addAndConsumeVideo(video_front.value().data,
                         video_front.value().data_size, video_timestamp,
                         video_front.value().is_key);
      continue;
    }
    addAndConsumeAudio(audio_front.value().data, audio_front.value().data_size,
                       audio_timestamp);
  }

  spdlog::debug("audio was processed");

  if (video_finished) {
    spdlog::debug("video was processed");
    return;
  }

  spdlog::debug("video is processing");
  while (!m_video_producer->isFinished()) {
    const auto video_front = m_video_producer->bufferFront();
    if (!video_front.has_value()) {
      spdlog::debug("video queue is empty (2)");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }

    addAndConsumeVideo(video_front.value().data, video_front.value().data_size,
                       video_front.value().timestamp,
                       video_front.value().is_key);
  }

  spdlog::debug("video was processed");
}  // namespace hisui::muxer

void AsyncWebMMuxer::cleanUp() {}

}  // namespace hisui::muxer
