#pragma once

#include <cstdint>
#include <cstdio>
#include <fstream>

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/muxer.hpp"

namespace hisui::webm::output {

class Context;

}  // namespace hisui::webm::output

namespace hisui::muxer {

class AudioProducer;
class VideoProducer;

class AsyncWebMMuxer : public Muxer {
 public:
  AsyncWebMMuxer(const hisui::Config&, const hisui::Metadata&);
  ~AsyncWebMMuxer();

  void run();

 private:
  hisui::webm::output::Context* m_context;
  std::FILE* m_file;

  hisui::Config m_config;
  hisui::Metadata m_metadata;

  VideoProducer* m_video_producer;
  AudioProducer* m_audio_producer;

  void addAndConsumeAudio(std::uint8_t*,
                          const std::size_t,
                          const std::uint64_t);
  void addAndConsumeVideo(std::uint8_t*,
                          const std::size_t,
                          const std::uint64_t,
                          const bool);
};

}  // namespace hisui::muxer
