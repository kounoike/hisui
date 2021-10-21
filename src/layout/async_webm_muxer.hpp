#pragma once

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/muxer.hpp"

namespace hisui {

struct Frame;

}

namespace hisui::webm::output {

class Context;

}  // namespace hisui::webm::output

namespace hisui::layout {

class AsyncWebMMuxer : public hisui::muxer::Muxer {
 public:
  AsyncWebMMuxer(const hisui::Config&, const hisui::layout::Metadata&);
  ~AsyncWebMMuxer();

  void setUp() override;
  void run() override;
  void cleanUp() override;

 private:
  void muxFinalize() override;
  void appendAudio(hisui::Frame) override;
  void appendVideo(hisui::Frame) override;

  hisui::webm::output::Context* m_context;

  hisui::Config m_config;
  hisui::layout::Metadata m_metadata;
};

}  // namespace hisui::layout
