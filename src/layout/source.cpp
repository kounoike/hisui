#include "layout/source.hpp"

#include <fmt/core.h>

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

}  // namespace hisui::layout
