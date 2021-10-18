#include "layout/region.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <stdexcept>

#include "layout/source.hpp"

namespace hisui::layout {

RegionInformation Region::getInfomation() const {
  return {.position = m_pos, .z_pos = m_z_pos, .resolution = m_resolution};
}

void Region::validate(const Resolution& parent_resolution) {
  if (parent_resolution.width < m_pos.x) {
    throw std::out_of_range(
        fmt::format("The x_pos({}) of region {} is out of parent width({})",
                    m_pos.x, m_name, parent_resolution.width));
  }

  if (parent_resolution.height < m_pos.y) {
    throw std::out_of_range(
        fmt::format("The y_pos({}) of region {} is out of parent height({})",
                    m_pos.y, m_name, parent_resolution.height));
  }

  if (m_z_pos > 99 || m_z_pos < -99) {
    throw std::out_of_range(fmt::format(
        "The z_pos({}) of region {} is out of [-99, 99]", m_z_pos, m_name));
  }

  if (m_resolution.width != 0) {
    if (m_pos.x + m_resolution.width > parent_resolution.width) {
      if (parent_resolution.height < m_pos.y) {
        throw std::out_of_range(fmt::format(
            "The x_pos{} & width({}) of region {} is out of parent width({})",
            m_pos.x, m_resolution.width, m_name, parent_resolution.width));
      }
    }
  } else {
    m_resolution.width = parent_resolution.width - m_pos.x;
  }

  if (m_resolution.height != 0) {
    if (m_pos.y + m_resolution.height > parent_resolution.height) {
      if (parent_resolution.height < m_pos.y) {
        throw std::out_of_range(fmt::format(
            "The y_pos{} & height({}) of region {} is out of parent height({})",
            m_pos.y, m_resolution.height, m_name, parent_resolution.height));
      }
    }
  } else {
    m_resolution.height = parent_resolution.height - m_pos.y;
  }
}

void Region::substructTrimIntervals(const TrimIntervals& params) {
  for (auto s : m_video_sources) {
    s->substructTrimIntervals(params);
  }

  auto interval =
      substruct_trim_intervals({.interval = {0, m_max_end_time},
                                .trim_intervals = params.trim_intervals});
  m_max_end_time = interval.end_time;
}

void set_video_source_to_cells(const SetVideoSourceToCells& params) {
  auto video_source = params.video_source;
  auto reuse = params.reuse;
  auto cells = params.cells;

  auto it_connection_id = std::find_if(
      std::begin(cells), std::end(cells), [video_source](const auto& cell) {
        return cell->hasVideoSourceConnectionID(video_source->connection_id);
      });
  if (it_connection_id != std::end(cells)) {
    return;
  }
  auto it_fresh = std::find_if(std::begin(cells), std::end(cells),
                               [video_source](const auto& cell) {
                                 return cell->hasStatus(CellStatus::Fresh);
                               });
  if (it_fresh != std::end(cells)) {
    (*it_fresh)->setSource(video_source);
  }

  if (reuse == Reuse::None) {
    return;
  }

  auto it_idle = std::find_if(std::begin(cells), std::end(cells),
                              [video_source](const auto& cell) {
                                return cell->hasStatus(CellStatus::Idle);
                              });
  if (it_idle != std::end(cells)) {
    (*it_idle)->setSource(video_source);
  }

  if (reuse == Reuse::ShowOldest) {
    return;
  }

  auto it_min = std::min_element(std::begin(cells), std::end(cells),
                                 [](const auto& a, const auto& b) {
                                   return a->getEndTime() < b->getEndTime();
                                 });

  if (it_min != std::end(cells)) {
    if ((*it_min)->hasStatus(CellStatus::Used) &&
        (*it_min)->getEndTime() < video_source->interval.end_time) {
      (*it_min)->setSource(video_source);
    }
  }
}

Region::Region(const RegionParameters& params)
    : m_name(params.name),
      m_pos(params.pos),
      m_z_pos(params.z_pos),
      m_resolution(params.resolution),
      m_max_columns(params.max_columns),
      m_max_rows(params.max_rows),
      m_cells_excluded(params.cells_excluded),
      m_reuse(params.reuse),
      m_raw_video_sources(params.video_sources),
      m_raw_video_sources_excluded(params.video_sources_excluded) {}

void Region::dump() const {
  spdlog::debug("  name: {}", m_name);
  spdlog::debug("  position: x: {} y: {}", m_pos.x, m_pos.y);
  spdlog::debug("  z_position: {} ", m_z_pos);
  spdlog::debug("  cells_excluded: [{}]", fmt::join(m_cells_excluded, ", "));
  spdlog::debug("  resolution: {}x{}", m_resolution.width, m_resolution.height);
  spdlog::debug("  max_columns: {}", m_max_columns);
  spdlog::debug("  max_rows: {}", m_max_rows);
  spdlog::debug("  video_sources: [{}]", fmt::join(m_raw_video_sources, ", "));
  spdlog::debug("  reuse: {}", m_reuse == Reuse::None         ? "none"
                               : m_reuse == Reuse::ShowOldest ? "show_oldest"
                                                              : "show_newest");
}

}  // namespace hisui::layout
