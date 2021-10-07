#include "layout/region.hpp"

#include "layout/source.hpp"

namespace hisui::layout {

std::uint64_t Region::GetMinimumStartTime() {
  return minimum_video_start_time({.sources = m_video_sources});
}

}  // namespace hisui::layout
