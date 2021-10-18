#include "layout/cell.hpp"

#include <limits>
#include <stdexcept>

namespace hisui::layout {

bool operator==(Position const& left, Position const& right) {
  return left.x == right.x && left.y == right.y;
}

std::ostream& operator<<(std::ostream& os, const Position& p) {
  os << "x: " << p.x << " y: " << p.y;
  return os;
}

bool operator==(Resolution const& left, Resolution const& right) {
  return left.width == right.width && left.height == right.height;
}

std::ostream& operator<<(std::ostream& os, const Resolution& r) {
  os << "width: " << r.width << " height: " << r.height;
  return os;
}

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

bool operator==(ResolutionAndPositions const& left,
                ResolutionAndPositions const& right) {
  return left.resolution == right.resolution &&
         left.positions == right.positions;
}

std::ostream& operator<<(std::ostream& os, const ResolutionAndPositions& rp) {
  os << "resolution: " << rp.resolution;
  os << " positions: [";
  for (const auto i : rp.positions) {
    os << " {" << i << "} ";
  }
  os << "]";
  return os;
}

Cell::Cell(const CellParameters& params)
    : m_index(params.index),
      m_pos(params.pos),
      m_resolution(params.resolution),
      m_status(CellStatus::Fresh) {
  m_end_time = std::numeric_limits<std::uint64_t>::max();
}

LengthAndPositions calc_cell_length_and_positions(
    const CalcCellLengthAndPositions& params) {
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

ResolutionAndPositions calc_cell_resolution_and_positions(
    const CalcCellResolutionAndPositions& params) {
  auto side = calc_cell_length_and_positions(
      {.number_of_cells = params.grid_dimension.columns,
       .region_length = params.region_resolution.width,
       .min_frame_length = params.min_frame_width});
  auto vert = calc_cell_length_and_positions(
      {.number_of_cells = params.grid_dimension.rows,
       .region_length = params.region_resolution.height,
       .min_frame_length = params.min_frame_height});

  std::vector<Position> positions;
  for (auto y = 0u; y < params.grid_dimension.rows; ++y) {
    for (auto x = 0u; x < params.grid_dimension.columns; ++x) {
      positions.push_back({.x = side.positions[x], .y = vert.positions[y]});
    }
  }
  return {.resolution = {.width = side.length, .height = vert.length},
          .positions = positions};
}

bool Cell::hasVideoSourceConnectionID(const std::string& connection_id) {
  return m_source && m_source->connection_id == connection_id;
}

bool Cell::hasStatus(const CellStatus status) {
  return m_status == status;
}

void Cell::setSource(std::shared_ptr<VideoSource> source) {
  m_status = CellStatus::Used;
  m_source = source;
  m_end_time = source->interval.end_time;
}

void Cell::resetSource(const std::uint64_t time) {
  if (time >= m_end_time) {
    m_status = CellStatus::Idle;
    m_source = nullptr;
    m_end_time = std::numeric_limits<std::uint64_t>::max();
  }
}

std::uint64_t Cell::getEndTime() const {
  return m_end_time;
}

void Cell::setExcludedStatus() {
  m_status = CellStatus::Excluded;
}

void reset_cells_source(const ResetCellsSource& params) {
  for (auto cell : params.cells) {
    cell->resetSource(params.time);
  }
}

}  // namespace hisui::layout
