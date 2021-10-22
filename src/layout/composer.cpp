#include <layout/composer.hpp>

#include <spdlog/spdlog.h>

namespace hisui::layout {

Composer::Composer(const ComposerParameters& params)
    : m_regions(params.regions), m_resolution(params.resolution) {
  m_plane_sizes[0] = m_resolution.width * m_resolution.height;
  m_plane_sizes[1] = (m_plane_sizes[0] + 3) >> 2;
  m_plane_sizes[2] = m_plane_sizes[1];

  m_planes[0] = new unsigned char[m_plane_sizes[0]];
  m_planes[1] = new unsigned char[m_plane_sizes[1]];
  m_planes[2] = new unsigned char[m_plane_sizes[2]];

  m_plane_default_values[0] = 0;
  m_plane_default_values[1] = 128;
  m_plane_default_values[2] = 128;
}

void Composer::compose(std::vector<unsigned char>* composed,
                       const std::uint64_t t) {
  for (std::size_t p = 0; p < 3; ++p) {
    std::fill_n(m_planes[p], m_plane_sizes[p], m_plane_default_values[p]);
  }

  for (auto region : m_regions) {
    auto yuv_image = region->getYUV(t);
    auto info = region->getInformation();
    for (std::size_t p = 0; p < 3; ++p) {
      if (p == 0) {
        hisui::video::overlay_yuv_planes(
            m_planes[p], yuv_image->yuv[p], m_resolution.width, info.pos.x,
            info.pos.y, info.resolution.width, info.resolution.height);
      } else {
        hisui::video::overlay_yuv_planes(
            m_planes[p], yuv_image->yuv[p], m_resolution.width >> 1,
            info.pos.x >> 1, info.pos.y >> 1, info.resolution.width >> 1,
            info.resolution.height >> 1);
      }
    }
  }
  auto base = 0;
  for (std::size_t p = 0; p < 3; ++p) {
    std::copy_n(m_planes[p], m_plane_sizes[p], composed->data() + base);
    base += m_plane_sizes[p];
  }
}

Composer::~Composer() {
  for (std::size_t p = 0; p < 3; ++p) {
    delete[] m_planes[p];
  }
}

}  // namespace hisui::layout
