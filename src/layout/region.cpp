#include "layout/region.hpp"

#include <fmt/core.h>

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

void Region::SubstructTrimIntervals(const TrimIntervals& params) {
  for (auto s : m_video_sources) {
    s.SubstructTrimIntervals(params);
  }

  auto interval =
      substruct_trim_intervals({.interval = {0, m_max_end_time},
                                .trim_intervals = params.trim_intervals});
  m_max_end_time = interval.end_time;
}

}  // namespace hisui::layout
