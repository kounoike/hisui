#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace hisui::audio {

class Sequencer {
 public:
  virtual ~Sequencer() = default;

  virtual void getSamples(std::vector<std::pair<std::int16_t, std::int16_t>>*,
                          const std::uint64_t) = 0;
};

}  // namespace hisui::audio
