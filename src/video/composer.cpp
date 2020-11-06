#include "video/composer.hpp"

namespace hisui::video {

std::uint32_t Composer::getWidth() const {
  return m_width;
}

std::uint32_t Composer::getHeight() const {
  return m_height;
}

}  // namespace hisui::video
