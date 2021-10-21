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
    os << " {" << i.first << ", " << i.second << "} ";
  }
  os << "]";
  return os;
}

MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals overlap_source_intervals(
    const OverlapSourceIntervalsParameters& params) {
  std::vector<std::pair<double, std::uint64_t>> data;

  for (const auto& s : params.sources) {
    /* for [start_time, end_time}: data(end_time).second < data(start_time).second */
    data.emplace_back(s.end_time, 0);
    data.emplace_back(s.start_time, 1);
  }

  sort(std::begin(data), std::end(data));

  std::vector<std::pair<double, double>> trim_intervals;

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
        trim_intervals.emplace_back(trim_start, d.first);
      }
      ++count;
    }
    ret = std::max(ret, count);
  }

  return {.max_number_of_overlap =
              params.reuse == Reuse::None
                  ? static_cast<std::uint32_t>(std::size(params.sources))
                  : ret,
          .max_end_time = max_end_time,
          .trim_intervals = trim_intervals};
}

std::vector<std::pair<double, double>> overlap_2_trim_intervals(
    const std::vector<std::pair<double, double>>& l,
    const std::vector<std::pair<double, double>>& r) {
  std::vector<std::pair<double, double>> ret;
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

    if (rp.first > lp.second) {
      ++li;
      continue;
    }

    if (lp.first > rp.second) {
      ++ri;
      continue;
    }

    auto start = std::max(lp.first, rp.first);
    auto end = std::min(lp.second, rp.second);

    if (start < end) {
      ret.emplace_back(start, end);
    }

    if (lp.second == rp.second) {
      ++li;
      ++ri;
    } else if (rp.second > lp.second) {
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
