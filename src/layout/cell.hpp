#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "layout/source.hpp"

namespace hisui::layout {

struct Position {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
};

struct Resolution {
  std::uint64_t width = 0;
  std::uint64_t height = 0;
};

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

struct CalcCellLength {
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

LengthAndPositions calc_cell_length_and_positions(const CalcCellLength&);

}  // namespace hisui::layout
