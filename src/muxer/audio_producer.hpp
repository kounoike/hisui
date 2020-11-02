#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <queue>

#include "config.hpp"
#include "metadata.hpp"
#include "webm/output/context.hpp"

namespace hisui::audio {

class Encoder;
class Sequencer;

}  // namespace hisui::audio

namespace hisui::muxer {

class AudioProducer {
 public:
  AudioProducer(const hisui::Config&,
                const hisui::Metadata& i,
                hisui::webm::output::Context* m_context);
  ~AudioProducer();
  void produce();
  void bufferPop();
  std::optional<hisui::webm::output::FrameTuple> bufferFront();
  bool isFinished();

 private:
  const hisui::Config m_config;
  const hisui::Metadata m_metadata;
  hisui::webm::output::Context* m_context;

  std::queue<hisui::webm::output::FrameTuple> m_buffer;

  hisui::audio::Sequencer* m_sequencer;
  hisui::audio::Encoder* m_encoder;
  std::int16_t (*m_mix_sample)(const std::int16_t, const std::int16_t);

  std::mutex m_mutex_buffer;

  bool m_is_finished = false;
};

}  // namespace hisui::muxer
