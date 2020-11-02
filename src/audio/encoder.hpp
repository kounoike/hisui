#pragma once

#include <cstdint>

namespace hisui::audio {

class Encoder {
 public:
  virtual ~Encoder() = default;

  virtual void addSample(const std::int16_t, const std::int16_t) = 0;
  virtual void flush() = 0;
};

}  // namespace hisui::audio
