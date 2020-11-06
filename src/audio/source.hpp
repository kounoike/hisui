#pragma once

#include <cstdint>
#include <utility>

namespace hisui::audio {

class Source {
 public:
  virtual ~Source() = default;
  virtual std::pair<std::int16_t, std::int16_t> getSample(
      const std::uint64_t) = 0;
};

}  // namespace hisui::audio
