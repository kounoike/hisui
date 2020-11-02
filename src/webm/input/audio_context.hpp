#pragma once

#include <cstdint>
#include <cstdio>

#include "webm/input/context.hpp"

namespace hisui::webm::input {

enum struct AudioCodec {
  None,
  Opus,
};

class AudioContext : public Context {
 public:
  AudioContext();
  ~AudioContext();

  void reset();
  bool init(std::FILE*);
  int getChannels() const;
  std::uint64_t getBitDepth() const;
  double getSamplingRate() const;
  AudioCodec getCodec() const;

 private:
  int m_channels = 0;
  std::uint64_t m_bit_depth = 0;
  double m_sampling_rate = 0.0;
  AudioCodec m_codec = AudioCodec::None;
};

}  // namespace hisui::webm::input
