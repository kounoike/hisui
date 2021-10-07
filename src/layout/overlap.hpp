#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "layout/region.hpp"
#include "layout/source.hpp"

namespace hisui::layout {

struct OverlapParameters {
  const std::vector<SourceInterval>& sources;
  Reuse reuse;
};

struct MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals {
  std::uint64_t max_number_of_overlap;
  std::uint64_t max_end_time;
  std::vector<std::pair<std::uint64_t, std::uint64_t>> trim_intervals;
};

bool operator==(MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& left,
                MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& right);

std::ostream& operator<<(
    std::ostream& os,
    const MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals&);

MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals overlap(
    const OverlapParameters&);
}  // namespace hisui::layout
