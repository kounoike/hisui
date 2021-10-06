#pragma once

#include <cstdint>
#include <ostream>

namespace hisui::layout {

struct GridDimension {
  std::uint64_t columns;
  std::uint64_t rows;
};

bool operator==(GridDimension const& left, GridDimension const& right);

std::ostream& operator<<(std::ostream& os, const GridDimension& gd);

struct CalcGridDimensionParameters {
  std::uint64_t max_columns = 0;  // 0: unconstrained
  std::uint64_t max_rows = 0;     // 0: unconstrained
  std::uint64_t number_of_sources =
      0;  // TODO(haruyama): consider cells_excluded
};

GridDimension calc_grid_dimension(const CalcGridDimensionParameters&);

}  // namespace hisui::layout
