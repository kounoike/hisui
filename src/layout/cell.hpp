#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "layout/grid.hpp"
#include "layout/source.hpp"

namespace hisui::layout {

struct Position {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
};

bool operator==(Position const& left, Position const& right);

std::ostream& operator<<(std::ostream& os, const Position&);

struct Resolution {
  std::uint64_t width = 0;
  std::uint64_t height = 0;
};

bool operator==(Resolution const& left, Resolution const& right);

std::ostream& operator<<(std::ostream& os, const Resolution&);

enum CellStatus {
  Fresh,
  Used,
  Idle,
  Excluded,
};

class Cell {
 public:
  Cell();

 private:
  std::uint64_t m_index;
  Position m_pos;
  Resolution m_resolution;
  std::shared_ptr<VideoSource> m_source;
  CellStatus m_status;
};

struct CalcCellLengthAndPositions {
  const std::uint64_t number_of_cells;
  const std::uint64_t region_length;
  const std::uint64_t min_frame_length;
};

struct LengthAndPositions {
  std::uint64_t length;
  std::vector<std::uint64_t> positions;
};

bool operator==(LengthAndPositions const& left,
                LengthAndPositions const& right);

std::ostream& operator<<(std::ostream& os, const LengthAndPositions&);

LengthAndPositions calc_cell_length_and_positions(
    const CalcCellLengthAndPositions&);

struct CalcCellResolutionAndPositions {
  const GridDimension grid_dimension;
  const Resolution region_resolution;
  const std::uint64_t min_frame_width = 1;
  const std::uint64_t min_frame_height = 1;
};

struct ResolutionAndPositions {
  Resolution resolution;
  std::vector<Position> positions;
};

bool operator==(ResolutionAndPositions const& left,
                ResolutionAndPositions const& right);

std::ostream& operator<<(std::ostream& os, const ResolutionAndPositions&);

ResolutionAndPositions calc_cell_resolution_and_positions(
    const CalcCellResolutionAndPositions&);

}  // namespace hisui::layout
