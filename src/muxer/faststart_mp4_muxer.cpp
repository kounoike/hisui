#include "muxer/faststart_mp4_muxer.hpp"

#include <cxxabi.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <future>
#include <iosfwd>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>

#include "config.hpp"
#include "constants.hpp"
#include "frame.hpp"
#include "metadata.hpp"
#include "muxer/audio_producer.hpp"
#include "muxer/video_producer.hpp"
#include "shiguredo/mp4/track/soun.hpp"
#include "shiguredo/mp4/track/vide.hpp"
#include "shiguredo/mp4/writer/faststart_writer.hpp"

namespace shiguredo::mp4::track {

class Track;

}

namespace hisui::muxer {

FaststartMP4Muxer::FaststartMP4Muxer(const hisui::Config& t_config,
                                     const hisui::Metadata& t_metadata)
    : m_config(t_config), m_metadata(t_metadata) {}

void FaststartMP4Muxer::setUp() {
  std::filesystem::path directory_for_faststart_intermediate_file;
  if (m_config.directory_for_faststart_intermediate_file != "") {
    directory_for_faststart_intermediate_file =
        m_config.directory_for_faststart_intermediate_file;
    if (!std::filesystem::is_directory(
            directory_for_faststart_intermediate_file)) {
      throw std::invalid_argument(
          fmt::format("{} is not directory",
                      m_config.directory_for_faststart_intermediate_file));
    }
  } else {
    std::filesystem::path metadata_path(m_config.in_metadata_filename);
    if (metadata_path.is_relative()) {
      metadata_path = std::filesystem::absolute(metadata_path);
    }
    directory_for_faststart_intermediate_file = metadata_path.parent_path();
  }
  spdlog::debug("directory_for_faststart_intermediate_file: {}",
                directory_for_faststart_intermediate_file.string());

  float duration = static_cast<float>(m_metadata.getMaxStopTimeOffset());
  m_faststart_writer = new shiguredo::mp4::writer::FaststartWriter(
      m_ofs, {.mvhd_timescale = 1000,
              .duration = duration,
              .mdat_path_templete =
                  directory_for_faststart_intermediate_file.string() +
                  std::filesystem::path::preferred_separator + "mdatXXXXXX"});
  initialize(m_config, m_metadata, m_faststart_writer, duration);
}

FaststartMP4Muxer::~FaststartMP4Muxer() {
  delete m_faststart_writer;
}

void FaststartMP4Muxer::run() {
  auto video_future =
      std::async(std::launch::async, &VideoProducer::produce, m_video_producer);

  auto audio_future =
      std::async(std::launch::async, &AudioProducer::produce, m_audio_producer);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  bool video_finished = false;

  m_faststart_writer->writeFtypBox();
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
  m_faststart_writer->appendTrakAndUdtaBoxInfo({m_soun_track, m_vide_track});
  m_faststart_writer->writeMoovBox();
  m_faststart_writer->writeMdatHeader();
  m_faststart_writer->copyMdatData();
  spdlog::debug("end");
}

void FaststartMP4Muxer::cleanUp() {
  if (std::filesystem::exists(m_faststart_writer->getIntermediateFilePath())) {
    m_faststart_writer->deleteIntermediateFile();
  }
}

}  // namespace hisui::muxer
