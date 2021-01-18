#include "audio/mixer.hpp"

#include <limits>

namespace hisui::audio {

// https://stackoverflow.com/a/12090491
std::int16_t mix_sample_simple(const std::int16_t sample1,
                               const std::int16_t sample2) {
  const std::int32_t result(static_cast<std::int32_t>(sample1) +
                            static_cast<std::int32_t>(sample2));
  typedef std::numeric_limits<std::int16_t> Range;
  if (Range::max() < result) {
    return Range::max();
  } else if (Range::min() > result) {
    return Range::min();
  } else {
    return static_cast<std::int16_t>(result);
  }
}

// https://stackoverflow.com/a/25102339
std::int16_t mix_sample_vttoth(const std::int16_t sample1,
                               const std::int16_t sample2) {
  std::int32_t s1 = sample1 + 32768;
  std::int32_t s2 = sample2 + 32768;
  std::int32_t m;
  if ((s1 < 32768) || (s2 < 32768)) {
    m = s1 * s2 / 32768;
  } else {
    m = 2 * (s1 + s2) - (s1 * s2) / 32768 - 65536;
  }
  if (m >= 65536) {
    m = 65535;
  }
  return static_cast<std::int16_t>(m - 32768);
}

}  // namespace hisui::audio
