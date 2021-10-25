#include "layout/overlap.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace hisui::layout {

bool operator==(MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& left,
                MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals const& right) {
  return left.max_number_of_overlap == right.max_number_of_overlap &&
         left.max_end_time == right.max_end_time &&
         left.trim_intervals == right.trim_intervals;
}

std::ostream& operator<<(
    std::ostream& os,
    const MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals& r) {
  os << "max_number_of_overlap: " << r.max_number_of_overlap
     << ", max_end_time: " << r.max_end_time << ", trim_intervals: [";
  for (const auto& i : r.trim_intervals) {
    os << " {" << i.start_time << ", " << i.end_time << "} ";
  }
  os << "]";
  return os;
}

MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals overlap_intervals(
    const OverlapIntervalsParameters& params) {
  std::vector<std::pair<double, std::uint64_t>> data;

  for (const auto& s : params.intervals) {
    /* for [start_time, end_time}: data(end_time).second < data(start_time).second */
    data.emplace_back(s.end_time, 0);
    data.emplace_back(s.start_time, 1);
  }

  sort(std::begin(data), std::end(data));

  std::vector<Interval> trim_intervals;

  std::uint32_t count = 0;
  std::uint32_t ret = 0;
  double trim_start = 0;
  double max_end_time = 0;
  for (const auto& d : data) {
    if (d.second == 0) {
      --count;
      if (count == 0) {
        trim_start = d.first;
      }
      max_end_time = d.first;
    }
    if (d.second == 1) {
      if (count == 0 && trim_start != d.first) {
        trim_intervals.push_back(
            {.start_time = trim_start, .end_time = d.first});
      }
      ++count;
    }
    ret = std::max(ret, count);
  }

  return {.max_number_of_overlap =
              params.reuse == Reuse::None
                  ? static_cast<std::uint32_t>(std::size(params.intervals))
                  : ret,
          .max_end_time = max_end_time,
          .trim_intervals = trim_intervals};
}

std::vector<Interval> overlap_2_trim_intervals(const std::vector<Interval>& l,
                                               const std::vector<Interval>& r) {
  std::vector<Interval> ret;
  std::size_t li = 0;
  std::size_t ri = 0;

  while (true) {
    if (li == std::size(l)) {
      break;
    }
    if (ri == std::size(r)) {
      break;
    }
    auto lp = l[li];
    auto rp = r[ri];

    if (rp.start_time > lp.end_time) {
      ++li;
      continue;
    }

    if (lp.start_time > rp.end_time) {
      ++ri;
      continue;
    }

    auto start = std::max(lp.start_time, rp.start_time);
    auto end = std::min(lp.end_time, rp.end_time);

    if (start < end) {
      ret.push_back({.start_time = start, .end_time = end});
    }

    if (lp.end_time == rp.end_time) {
      ++li;
      ++ri;
    } else if (rp.end_time > lp.end_time) {
      ++li;
    } else {
      ++ri;
    }
  }

  return ret;
}

TrimIntervals overlap_trim_intervals(
    const OverlapTrimIntervalsParameters& params) {
  const auto s = std::size(params.list_of_trim_intervals);
  if (s == 0) {
    return {.trim_intervals = {}};
  } else if (s == 1) {
    return {.trim_intervals = params.list_of_trim_intervals.front()};
  }
  auto list_of_trim_intervals = params.list_of_trim_intervals;
  const auto l = list_of_trim_intervals.front();
  list_of_trim_intervals.pop_front();
  const auto r = list_of_trim_intervals.front();
  list_of_trim_intervals.pop_front();

  list_of_trim_intervals.push_front(overlap_2_trim_intervals(l, r));
  return overlap_trim_intervals(
      {.list_of_trim_intervals = list_of_trim_intervals});
}

}  // namespace hisui::layout
