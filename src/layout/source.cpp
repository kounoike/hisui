#include "layout/source.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <limits>

namespace hisui::layout {

Source::Source(const SourceParameters& params) {
  name = params.name;
  start_time = params.start_time;
  end_time = params.end_time;
}

void Source::SubstructTime(const std::uint64_t t) {
  if (start_time < t) {
    throw std::invalid_argument(
        fmt::format("start_time({}) < t({})", start_time, t));
  }
  start_time -= t;
  end_time -= t;
}

VideoSource::VideoSource(const SourceParameters& params) : Source(params) {}

AudioSource::AudioSource(const SourceParameters& params) : Source(params) {}

std::uint64_t minimum_video_start_time(
    const MinimumVideoStartTimeParameters& params) {
  auto min = std::numeric_limits<std::uint64_t>::max();
  for (const auto& s : params.sources) {
    min = std::min(min, s.start_time);
  }
  return min;
}

std::uint64_t minimum_audio_start_time(
    const MinimumAudioStartTimeParameters& params) {
  auto min = std::numeric_limits<std::uint64_t>::max();
  for (const auto& s : params.sources) {
    min = std::min(min, s.start_time);
  }
  return min;
}

}  // namespace hisui::layout
