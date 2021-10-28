#pragma once

#include <memory>
#include <vector>

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/muxer.hpp"

namespace hisui {

struct Frame;

}

namespace hisui::webm::output {

class Context;

}  // namespace hisui::webm::output

namespace hisui::muxer {

class AsyncWebMMuxer : public Muxer {
 public:
  AsyncWebMMuxer(const hisui::Config&, const double);
  AsyncWebMMuxer(const hisui::Config&, hisui::MetadataSet*);
  ~AsyncWebMMuxer();

  void setUp() override;
  void setVideoProducer(std::shared_ptr<VideoProducer>);
  void setAudioArchives(const std::vector<hisui::Archive>&);
  void run() override;
  void cleanUp() override;

 private:
  void muxFinalize() override;
  void appendAudio(hisui::Frame) override;
  void appendVideo(hisui::Frame) override;

  hisui::webm::output::Context* m_context;

  hisui::Config m_config;
  hisui::MetadataSet* m_metadata_set;
  std::vector<hisui::Archive> m_audio_archives;
  double m_duration;
  std::size_t m_normal_archive_size;
};

}  // namespace hisui::muxer
