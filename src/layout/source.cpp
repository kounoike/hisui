#include "layout/source.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <limits>

namespace hisui::layout {

Source::Source(const SourceParameters& params) {
  name = params.name;
  channel_id = params.channel_id;
  interval.start_time = params.start_time;
  interval.end_time = params.end_time;
}

void Source::SubstructTime(const std::uint64_t t) {
  if (interval.start_time < t) {
    throw std::invalid_argument(
        fmt::format("start_time({}) < t({})", interval.start_time, t));
  }
  interval.start_time -= t;
  interval.end_time -= t;
}

VideoSource::VideoSource(const SourceParameters& params) : Source(params) {}

AudioSource::AudioSource(const SourceParameters& params) : Source(params) {}

}  // namespace hisui::layout
