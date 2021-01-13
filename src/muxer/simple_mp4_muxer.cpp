#include "muxer/simple_mp4_muxer.hpp"

#include <cxxabi.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <future>
#include <iosfwd>
#include <optional>
#include <system_error>
#include <thread>

#include "frame.hpp"
#include "metadata.hpp"
#include "muxer/audio_producer.hpp"
#include "muxer/video_producer.hpp"
#include "shiguredo/mp4/track/soun.hpp"
#include "shiguredo/mp4/track/vide.hpp"
#include "shiguredo/mp4/writer/simple_writer.hpp"

namespace shiguredo::mp4::track {

class Track;

}

namespace hisui::muxer {

SimpleMP4Muxer::SimpleMP4Muxer(const hisui::Config& config,
                               const hisui::Metadata& metadata) {
  float duration = static_cast<float>(metadata.getMaxStopTimeOffset());
  m_simple_writer = new shiguredo::mp4::writer::SimpleWriter(
      m_ofs, {.mvhd_timescale = 1000, .duration = duration});
  initialize(config, metadata, m_simple_writer, duration);
  m_simple_writer->writeFtypBox();
}

SimpleMP4Muxer::~SimpleMP4Muxer() {
  delete m_simple_writer;
}

void SimpleMP4Muxer::run() {
  auto video_future =
      std::async(std::launch::async, &VideoProducer::produce, m_video_producer);

  auto audio_future =
      std::async(std::launch::async, &AudioProducer::produce, m_audio_producer);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  bool video_finished = false;

  while (!m_audio_producer->isFinished()) {
    auto audio_front = m_audio_producer->bufferFront();
    if (!audio_front.has_value()) {
      spdlog::debug("audio queue is empty");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }
    auto audio_timestamp = audio_front.value().timestamp;

    if (video_finished) {
      addAudioBuffer(audio_front.value());
      continue;
    }

    if (m_video_producer->isFinished()) {
      video_finished = true;
      addAudioBuffer(audio_front.value());
      continue;
    }

    auto video_front = m_video_producer->bufferFront();
    if (!video_front.has_value()) {
      spdlog::debug("video queue is empty (1)");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }
    auto video_timestamp = video_front.value().timestamp *
                           m_soun_track->getTimescale() /
                           m_vide_track->getTimescale();

    if (video_timestamp <= audio_timestamp) {
      addVideoBuffer(video_front.value());
      continue;
    }
    addAudioBuffer(audio_front.value());
  }

  spdlog::debug("audio was processed");

  if (video_finished) {
    spdlog::debug("video was processed");
  } else {
    spdlog::debug("video is processing");
    while (!m_video_producer->isFinished()) {
      auto video_front = m_video_producer->bufferFront();
      if (!video_front.has_value()) {
        spdlog::debug("video queue is empty (2)");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        continue;
      }

      addVideoBuffer(video_front.value());
    }
  }

  spdlog::debug("video was processed");

  writeTrackData();

  spdlog::debug("mp4");

  m_simple_writer->appendTrakAndUdtaBoxInfo({m_soun_track, m_vide_track});
  m_simple_writer->writeFreeBoxAndMdatHeader();
  m_simple_writer->writeMoovBox();
  spdlog::debug("end");
}

}  // namespace hisui::muxer
