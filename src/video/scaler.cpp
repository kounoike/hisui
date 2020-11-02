#include "video/scaler.hpp"

#include "video/yuv.hpp"

namespace hisui::video {

Scaler::Scaler(const std::uint32_t t_width, const std::uint32_t t_height)
    : m_width(t_width), m_height(t_height) {
  m_scaled = new YUVImage(m_width, m_height);
}

Scaler::~Scaler() {
  delete m_scaled;
}

}  // namespace hisui::video
