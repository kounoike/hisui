#pragma once

#include <cstdint>
#include <list>
#include <utility>
#include <vector>

#include "layout/interval.hpp"
#include "layout/reuse.hpp"
#include "layout/source.hpp"

namespace hisui::layout {

struct OverlapIntervalsParameters {
  const std::vector<Interval>& intervals;
  Reuse reuse;
};

struct MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals {
  std::uint32_t max_number_of_overlap;
  double max_end_time;
  std::vector<std::pair<double, double>> trim_intervals;
};

bool operator==(MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& left,
                MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& right);

std::ostream& operator<<(
    std::ostream& os,
    const MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals&);

MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals overlap_intervals(
    const OverlapIntervalsParameters&);

struct OverlapTrimIntervalsParameters {
  const std::list<std::vector<std::pair<double, double>>>&
      list_of_trim_intervals;
};

TrimIntervals overlap_trim_intervals(const OverlapTrimIntervalsParameters&);

}  // namespace hisui::layout
