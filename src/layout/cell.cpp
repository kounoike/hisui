#include "layout/cell.hpp"

#include <stdexcept>

namespace hisui::layout {

bool operator==(LengthAndPositions const& left,
                LengthAndPositions const& right) {
  return left.length == right.length && left.positions == right.positions;
}

std::ostream& operator<<(std::ostream& os, const LengthAndPositions& lp) {
  os << "length: " << lp.length;
  os << " positions: [";
  for (const auto i : lp.positions) {
    os << " " << i << " ";
  }
  os << "]";
  return os;
}

Cell::Cell() {
  m_status = CellStatus::Fresh;
}

LengthAndPositions calc_cell_length_and_positions(
    const CalcCellLength& params) {
  if (params.number_of_cells == 0) {
    throw std::invalid_argument("number_of_cells should be grater than 0");
  }
  auto allLength = params.region_length -
                   (params.number_of_cells + 1) * params.min_frame_length;
  auto length = ((allLength / params.number_of_cells) >> 2) << 2;
  std::vector<std::uint64_t> positions;
  for (std::uint64_t i = 0; i < params.number_of_cells; ++i) {
    positions.emplace_back(params.min_frame_length * (i + 1) + length * i);
  }
  return {.length = length, .positions = positions};
}

}  // namespace hisui::layout
