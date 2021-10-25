#include "layout/region.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <stdexcept>

#include "constants.hpp"
#include "layout/overlap.hpp"
#include "layout/source.hpp"
#include "video/yuv.hpp"

namespace hisui::layout {

RegionInformation Region::getInformation() const {
  return {.pos = m_pos, .resolution = m_resolution};
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

  std::size_t index = 0;
  for (const auto& f : m_video_source_filenames) {
    auto archive = parse_archive(f);
    m_video_archives.push_back(archive);
    m_video_sources.push_back(
        std::make_shared<VideoSource>(archive->getSourceParameters(index++)));
  }

  std::vector<Interval> source_intervals;
  std::transform(std::begin(m_video_sources), std::end(m_video_sources),
                 std::back_inserter(source_intervals),
                 [](const auto& s) -> Interval { return s->source_interval; });
  auto overlap_result =
      overlap_intervals({.intervals = source_intervals, .reuse = m_reuse});

  m_max_end_time = overlap_result.max_end_time;

  for (const auto& i : overlap_result.trim_intervals) {
    spdlog::debug("    trim_interval: [{}, {}]", i.start_time, i.end_time);
  }

  std::sort(std::begin(m_cells_excluded), std::end(m_cells_excluded));
  auto max_cells = add_number_of_excluded_cells({
      .number_of_sources = overlap_result.max_number_of_overlap,
      .cells_excluded = m_cells_excluded,
  });

  m_grid_dimension = calc_grid_dimension({.max_columns = m_max_columns,
                                          .max_rows = m_max_rows,
                                          .number_of_sources = max_cells});
  auto cell_resolution_and_posiitons = calc_cell_resolution_and_positions({
      .grid_dimension = m_grid_dimension,
      .region_resolution = m_resolution,
      .is_width_frame_on_ends =
          !(m_resolution.width == params.resolution.width),
      .is_height_frame_on_ends =
          !(m_resolution.height == params.resolution.height),
  });

  for (std::size_t i = 0; i < m_grid_dimension.rows * m_grid_dimension.columns;
       ++i) {
    if (i >= max_cells) {
      break;
    }
    CellStatus status = CellStatus::Fresh;
    auto it =
        std::find(std::begin(m_cells_excluded), std::end(m_cells_excluded), i);
    if (it != std::end(m_cells_excluded)) {
      status = CellStatus::Excluded;
    }
    m_cells.push_back(std::make_shared<Cell>(
        CellParameters{.index = i,
                       .pos = cell_resolution_and_posiitons.positions[i],
                       .resolution = cell_resolution_and_posiitons.resolution,
                       .status = status}));
    auto info = m_cells[i]->getInformation();
    spdlog::debug("    cell[{}]: x: {}, y:{}, w:{}, h:{}", i, info.pos.x,
                  info.pos.y, info.resolution.width, info.resolution.height);
  }
  spdlog::debug("    cell size: {}", std::size(m_cells));

  m_plane_sizes[0] = m_resolution.width * m_resolution.height;
  m_plane_sizes[1] = (m_plane_sizes[0] + 3) >> 2;
  m_plane_sizes[2] = m_plane_sizes[1];

  m_yuv_image =
      new hisui::video::YUVImage(m_resolution.width, m_resolution.height);

  m_plane_default_values[0] = 0;
  m_plane_default_values[1] = 128;
  m_plane_default_values[2] = 128;

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
        // return cell->hasVideoSourceConnectionID(video_source->connection_id);
        return cell->hasVideoSourceIndex(video_source->index);
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

const hisui::video::YUVImage* Region::getYUV(const std::uint64_t t) {
  reset_cells_source({.cells = m_cells, .time = t});

  for (auto video_source : m_video_sources) {
    if (video_source->encoding_interval.isIn(t)) {
      set_video_source_to_cells(
          {.video_source = video_source, .reuse = m_reuse, .cells = m_cells});
    }
  }

  for (std::size_t p = 0; p < 3; ++p) {
    std::fill_n(m_yuv_image->yuv[p], m_plane_sizes[p],
                m_plane_default_values[p]);
  }

  for (auto& cell : m_cells) {
    if (cell->hasStatus(CellStatus::Used)) {
      auto yuv_image = cell->getYUV(t);
      auto info = cell->getInformation();
      for (std::size_t p = 0; p < 3; ++p) {
        if (p == 0) {
          hisui::video::overlay_yuv_planes(
              m_yuv_image->yuv[p], yuv_image->yuv[p], m_resolution.width,
              info.pos.x, info.pos.y, info.resolution.width,
              info.resolution.height);
        } else {
          hisui::video::overlay_yuv_planes(
              m_yuv_image->yuv[p], yuv_image->yuv[p], m_resolution.width >> 1,
              info.pos.x >> 1, info.pos.y >> 1, info.resolution.width >> 1,
              info.resolution.height >> 1);
        }
      }
    }
  }

  return m_yuv_image;
}

Region::~Region() {
  delete m_yuv_image;
}

}  // namespace hisui::layout
