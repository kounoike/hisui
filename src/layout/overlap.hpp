#pragma once

#include <vector>

#include "layout/region.hpp"

namespace hisui::layout {

struct MaximumNumberOfOverlapParameters {
  const std::vector<VideoSource>& sources;
  Reuse reuse;
};

std::uint64_t maximum_number_of_overlap(
    const MaximumNumberOfOverlapParameters&);
}  // namespace hisui::layout
