#include "layout/source.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <limits>

#include "audio/webm_source.hpp"
#include "layout/interval.hpp"
#include "layout/overlap.hpp"

namespace hisui::layout {

bool operator==(TrimIntervals const& left, TrimIntervals const& right) {
  return left.trim_intervals == right.trim_intervals;
}

std::ostream& operator<<(std::ostream& os, const TrimIntervals& r) {
  os << "[";
  for (const auto& i : r.trim_intervals) {
    os << " {" << i.first << ", " << i.second << "} ";
  }
  os << "]";
  return os;
}

Source::Source(const SourceParameters& params) {
  file_path = params.file_path;
  connection_id = params.connection_id;
  source_interval.start_time = params.start_time;
  source_interval.end_time = params.end_time;
  encoding_interval.set(static_cast<std::uint64_t>(params.start_time),
                        static_cast<std::uint64_t>(params.end_time));
}

void Source::substructTrimIntervals(const TrimIntervals& params) {
  source_interval = substruct_trim_intervals(
      {.interval = source_interval, .trim_intervals = params.trim_intervals});
}

Interval substruct_trim_intervals(
    const SubstructTrimIntervalsParameters& params) {
  auto interval = params.interval;
  auto trims = params.trim_intervals;
  if (std::empty(trims)) {
    return interval;
  }
  for (std::int64_t i = static_cast<std::int64_t>(std::size(trims)) - 1; i >= 0;
       --i) {
    auto s = static_cast<std::size_t>(i);

    if (trims[s].first >= interval.end_time) {
      continue;
    }
    auto t = trims[s].second - trims[s].first;
    interval.start_time -= t;
    interval.end_time -= t;
  }
  return interval;
}

}  // namespace hisui::layout
