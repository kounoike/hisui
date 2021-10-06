#include "layout/grid.hpp"

#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace hisui::layout {

GridDimension calc_grid_dimension_unconstrained_grid(
    const CalcGridDimensionParameters& params) {
  auto columns = static_cast<std::uint64_t>(
      std::ceil(std::sqrt(params.number_of_sources)));
  auto rows = static_cast<std::uint64_t>(
      std::ceil(static_cast<double>(params.number_of_sources) /
                static_cast<double>(columns)));
  return {.columns = columns, .rows = rows};
}

GridDimension calc_grid_dimension_unconstrained_dimension_rows(
    const CalcGridDimensionParameters& params) {
  if (params.max_columns >= params.number_of_sources) {
    return calc_grid_dimension_unconstrained_grid(params);
  }
  auto columns = params.max_columns;
  auto rows = static_cast<std::uint64_t>(
      std::ceil(static_cast<double>(params.number_of_sources) /
                static_cast<double>(columns)));
  return {.columns = columns, .rows = rows};
}

GridDimension calc_grid_dimension_unconstrained_dimension_columns(
    const CalcGridDimensionParameters& params) {
  if (params.max_rows >= params.number_of_sources) {
    return calc_grid_dimension_unconstrained_grid(params);
  }
  auto rows = params.max_rows;
  auto columns = static_cast<std::uint64_t>(
      std::ceil(static_cast<double>(params.number_of_sources) /
                static_cast<double>(rows)));
  return {.columns = columns, .rows = rows};
}

GridDimension calc_grid_dimension_constrained_grid(
    const CalcGridDimensionParameters& params) {
  if (params.max_columns * params.max_rows <= params.number_of_sources) {
    return {.columns = params.max_columns, .rows = params.max_rows};
  }
  if (params.max_rows >= params.max_columns) {
    auto columns = std::min(params.number_of_sources, params.max_columns);
    auto rows = std::min(params.max_rows,
                         static_cast<std::uint64_t>(std::ceil(
                             static_cast<double>(params.number_of_sources) /
                             static_cast<double>(columns))));
    return {.columns = columns, .rows = rows};
  }
  auto rows = std::min(params.number_of_sources, params.max_rows);
  auto columns = std::min(params.max_columns,
                          static_cast<std::uint64_t>(std::ceil(
                              static_cast<double>(params.number_of_sources) /
                              static_cast<double>(rows))));
  return {.columns = columns, .rows = rows};
}

GridDimension calc_grid_dimension(const CalcGridDimensionParameters& params) {
  if (params.number_of_sources == 0) {
    throw std::invalid_argument("number_of_sources should be greater than 0");
  }

  if (params.max_rows == 0) {
    if (params.max_columns == 0) {
      return calc_grid_dimension_unconstrained_grid(params);
    }
    return calc_grid_dimension_unconstrained_dimension_rows(params);
  }

  if (params.max_columns == 0) {
    return calc_grid_dimension_unconstrained_dimension_columns(params);
  }

  return calc_grid_dimension_constrained_grid(params);
}

bool operator==(GridDimension const& left, GridDimension const& right) {
  return left.columns == right.columns && left.rows == right.rows;
}

std::ostream& operator<<(std::ostream& os, const GridDimension& gd) {
  os << gd.columns << " x " << gd.rows;
  return os;
}

}  // namespace hisui::layout
