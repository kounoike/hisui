#include "layout/region.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <stdexcept>

#include "constants.hpp"
#include "layout/overlap.hpp"
#include "layout/source.hpp"

namespace hisui::layout {

RegionInformation Region::getInfomation() const {
  return {.position = m_pos, .resolution = m_resolution};
}

std::int32_t Region::getZPos() const {
  return m_z_pos;
}

const RegionPrepareResult Region::prepare(
    const RegionPrepareParameters& params) {
  if (params.resolution.width < m_pos.x) {
    throw std::out_of_range(
        fmt::format("The x_pos({}) of region {} is out of parent width({})",
                    m_pos.x, m_name, params.resolution.width));
  }

  if (params.resolution.height < m_pos.y) {
    throw std::out_of_range(
        fmt::format("The y_pos({}) of region {} is out of parent height({})",
                    m_pos.y, m_name, params.resolution.height));
  }

  if (m_z_pos > 99 || m_z_pos < -99) {
    throw std::out_of_range(fmt::format(
        "The z_pos({}) of region {} is out of [-99, 99]", m_z_pos, m_name));
  }

  if (m_resolution.width != 0) {
    if (m_pos.x + m_resolution.width > params.resolution.width) {
      throw std::out_of_range(fmt::format(
          "The x_pos({}) & width({}) of region {} is out of parent width({})",
          m_pos.x, m_resolution.width, m_name, params.resolution.width));
    }
  } else {
    m_resolution.width = params.resolution.width - m_pos.x;
  }

  if (m_resolution.height != 0) {
    if (m_pos.y + m_resolution.height > params.resolution.height) {
      throw std::out_of_range(fmt::format(
          "The y_pos({}) & height({}) of region {} is out of parent height({})",
          m_pos.y, m_resolution.height, m_name, params.resolution.height));
    }
  } else {
    m_resolution.height = params.resolution.height - m_pos.y;
  }

  // TODO(haruyama): 2 の倍数でよさそうだが, 4 の倍数のほうがいいかも
  m_resolution.width = (m_resolution.width >> 1) << 1;
  m_resolution.height = (m_resolution.height >> 1) << 1;

  if (m_resolution.width < 16) {
    throw std::out_of_range(fmt::format("width({}) of region({}) is too small",
                                        m_resolution.width, m_name));
  }
  if (m_resolution.height < 16) {
    throw std::out_of_range(fmt::format("height({}) of region({}) is too small",
                                        m_resolution.height, m_name));
  }

  for (const auto& f : m_video_source_filenames) {
    auto archive = parse_archive(f);
    m_video_archives.push_back(archive);
    m_video_sources.push_back(
        std::make_shared<VideoSource>(archive->getSourceParameters()));
  }

  std::vector<SourceInterval> source_intervals;
  std::transform(
      std::begin(m_video_sources), std::end(m_video_sources),
      std::back_inserter(source_intervals),
      [](const auto& s) -> SourceInterval { return s->source_interval; });
  auto overlap_result =
      overlap_source_intervals({.sources = source_intervals, .reuse = m_reuse});

  m_max_end_time = overlap_result.max_end_time;

  for (const auto& i : overlap_result.trim_intervals) {
    spdlog::debug("    trim_interval: [{}, {}]", i.first, i.second);
  }

  std::sort(std::begin(m_cells_excluded), std::end(m_cells_excluded));

  m_grid_dimension = calc_grid_dimension(
      {.max_columns = m_max_columns,
       .max_rows = m_max_rows,
       .number_of_sources = add_number_of_excluded_cells({
           .number_of_sources = overlap_result.max_number_of_overlap,
           .cells_excluded = m_cells_excluded,
       })});
  return {.trim_intervals = overlap_result.trim_intervals};
}

double Region::getMaxEndTime() const {
  return m_max_end_time;
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
        (*it_min)->getEndTime() < video_source->encoding_interval.getUpper()) {
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
      m_video_source_filenames(params.video_sources),
      m_video_source_excluded_filenames(params.video_sources_excluded) {}

void Region::dump() const {
  spdlog::debug("  name: {}", m_name);
  spdlog::debug("  position: x: {} y: {}", m_pos.x, m_pos.y);
  spdlog::debug("  z_position: {} ", m_z_pos);
  spdlog::debug("  cells_excluded: [{}]", fmt::join(m_cells_excluded, ", "));
  spdlog::debug("  resolution: {}x{}", m_resolution.width, m_resolution.height);
  spdlog::debug("  max_columns: {}", m_max_columns);
  spdlog::debug("  max_rows: {}", m_max_rows);
  spdlog::debug("  video_sources: [{}]",
                fmt::join(m_video_source_filenames, ", "));
  spdlog::debug("  reuse: {}", m_reuse == Reuse::None         ? "none"
                               : m_reuse == Reuse::ShowOldest ? "show_oldest"
                                                              : "show_newest");
  if (!std::empty(m_video_sources)) {
    spdlog::debug("  grid_dimension: {}x{}", m_grid_dimension.columns,
                  m_grid_dimension.rows);
    for (const auto& a : m_video_sources) {
      spdlog::debug("    file_path: {}", a->file_path.string());
      spdlog::debug("    connection_id: {}", a->connection_id);
      spdlog::debug("    start_time: {}", a->source_interval.start_time);
      spdlog::debug("    end_time: {}", a->source_interval.end_time);
    }
  }
}

void Region::setEncodingInterval() {
  for (auto& s : m_video_sources) {
    s->encoding_interval.set(
        static_cast<std::uint64_t>(std::floor(s->source_interval.start_time *
                                              hisui::Constants::NANO_SECOND)),
        static_cast<std::uint64_t>(std::ceil(s->source_interval.end_time *
                                             hisui::Constants::NANO_SECOND)));
  }
}

}  // namespace hisui::layout
