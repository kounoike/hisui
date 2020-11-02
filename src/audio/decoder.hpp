#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

namespace hisui::audio {

class Decoder {
 public:
  virtual ~Decoder() = default;
  virtual std::pair<const std::int16_t*, const std::size_t> decode(
      const unsigned char*,
      const std::size_t) = 0;
};

}  // namespace hisui::audio
