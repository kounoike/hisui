#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "layout/archive.hpp"
#include "layout/cell.hpp"
#include "layout/grid.hpp"
#include "layout/source.hpp"
#include "layout/video_source.hpp"

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

struct RegionParameters {
  const std::string name;
  const Position pos;
  const std::int64_t z_pos;
  const Resolution resolution;
  const std::uint64_t max_columns;
  const std::uint64_t max_rows;
  const std::vector<std::uint64_t>& cells_excluded = {};
  const Reuse reuse;
  const std::vector<std::string>& video_sources = {};
  const std::vector<std::string>& video_sources_excluded = {};
};

struct RegionPrepareParameters {
  const Resolution& resolution;
};

struct RegionPrepareResult {
  std::vector<std::pair<std::uint64_t, std::uint64_t>> trim_intervals;
};

class Region {
 public:
  explicit Region(const RegionParameters&);

  void dump() const;
  RegionInformation getInfomation() const;
  const RegionPrepareResult prepare(const RegionPrepareParameters&);
  void substructTrimIntervals(const TrimIntervals&);
  std::uint64_t getMaxEndTime() const;

 private:
  std::string m_name;
  Position m_pos;
  std::int64_t m_z_pos;
  Resolution m_resolution;
  std::uint64_t m_max_columns;
  std::uint64_t m_max_rows;
  std::vector<std::uint64_t> m_cells_excluded;
  Reuse m_reuse;
  std::vector<std::string> m_video_source_filenames;
  std::vector<std::string> m_video_source_excluded_filenames;

  // computed
  GridDimension m_grid_dimension;
  std::vector<std::shared_ptr<Archive>> m_video_archives;
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
