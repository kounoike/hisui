#pragma once

#include <mutex>
#include <optional>
#include <queue>

#include "config.hpp"
#include "metadata.hpp"
#include "webm/output/context.hpp"

namespace hisui::video {

class Encoder;
class Sequencer;
class Composer;

}  // namespace hisui::video

namespace hisui::muxer {

class VideoProducer {
 public:
  VideoProducer(const hisui::Config&,
                const hisui::Metadata& i,
                hisui::webm::output::Context* m_context);
  ~VideoProducer();
  void produce();

  void bufferPop();
  std::optional<hisui::webm::output::FrameTuple> bufferFront();

  bool isFinished();

 private:
  const hisui::Config m_config;
  const hisui::Metadata m_metadata;
  hisui::webm::output::Context* m_context;

  hisui::video::Sequencer* m_sequencer;
  hisui::video::Encoder* m_encoder;
  hisui::video::Composer* m_composer;

  std::queue<hisui::webm::output::FrameTuple> m_buffer;

  bool m_show_progress_bar;

  bool m_is_finished = false;

  std::mutex m_mutex_buffer;
};

}  // namespace hisui::muxer
