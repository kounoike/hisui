#include "layout/overlap.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace hisui::layout {

std::uint64_t maximum_number_of_overlap_show(
    const MaximumNumberOfOverlapParameters& params) {
  std::vector<std::pair<std::uint64_t, std::uint64_t>> data;

  for (const auto& s : params.sources) {
    data.emplace_back(
        s.end_time,
        0);  // for [start_time, end_time}: data(end_time).second < data(start_time).second
    data.emplace_back(s.start_time, 1);
  }

  sort(std::begin(data), std::end(data));

  std::uint64_t count = 0;
  std::uint64_t ret = 0;
  for (const auto& d : data) {
    if (d.second == 0) {
      --count;
    }
    if (d.second == 1) {
      ++count;
    }
    ret = std::max(ret, count);
  }
  return ret;
}

std::uint64_t maximum_number_of_overlap(
    const MaximumNumberOfOverlapParameters& params) {
  if (params.reuse == Reuse::None) {
    return std::size(params.sources);
  }

  return maximum_number_of_overlap_show(params);
}

}  // namespace hisui::layout

