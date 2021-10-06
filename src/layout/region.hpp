#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "layout/grid.hpp"
#include "video/source.hpp"

namespace hisui::layout {

struct Position {
  std::uint64_t x;
  std::uint64_t y;
};

struct Resolution {
  std::uint64_t width;
  std::uint64_t height;
};

struct VideoSource {
  std::string name;
  std::shared_ptr<hisui::video::Source> source;
  std::uint64_t start_time;
  std::uint64_t end_time;
};

enum Reuse {
  None,
  ShowOldest,
  ShowNewest,
};

class Region {
 private:
  std::string m_name;
  Position m_pos;
  std::uint64_t m_z_pos;
  Resolution m_resolution;
  std::uint64_t m_max_columns;
  std::uint64_t m_max_rows;
  std::vector<std::uint64_t> m_cells_excluded;
  Reuse m_reuse;
  std::vector<std::string> m_raw_video_sources;
  std::vector<std::string> m_raw_video_sources_excluded;

  // computed
  GridDimension m_grid_dimension;
  std::vector<VideoSource> m_video_sources;
};

}  // namespace hisui::layout
