#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "layout/cell.hpp"
#include "layout/grid.hpp"
#include "layout/source.hpp"
#include "video/source.hpp"

namespace hisui::layout {

enum Reuse {
  None,
  ShowOldest,
  ShowNewest,
};

struct RegionInformation {
  const Position& position;
  const std::int64_t z_pos;
  const Resolution& resolution;
};

class Region {
 public:
  RegionInformation getInfomation() const;
  void validate(const Resolution&);
  void SubstructTrimIntervals(const TrimIntervals&);

 private:
  std::string m_name;
  Position m_pos;
  std::int64_t m_z_pos;
  Resolution m_resolution;
  std::uint64_t m_max_columns;
  std::uint64_t m_max_rows;
  std::vector<std::uint64_t> m_cells_excluded;
  Reuse m_reuse;
  std::vector<std::string> m_raw_video_sources;
  std::vector<std::string> m_raw_video_sources_excluded;

  // computed
  GridDimension m_grid_dimension;
  std::vector<std::shared_ptr<VideoSource>> m_video_sources;
  std::vector<std::shared_ptr<Cell>> m_cells;
  std::uint64_t m_max_end_time;
};

struct SetVideoSourceToCells {
  const std::shared_ptr<VideoSource> video_source;
  Reuse reuse;
  const std::vector<std::shared_ptr<Cell>>& cells;
};

void set_video_source_to_cells(const SetVideoSourceToCells&);

}  // namespace hisui::layout
