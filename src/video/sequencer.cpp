#include "video/sequencer.hpp"

namespace hisui::video {

std::uint32_t Sequencer::getMaxWidth() const {
  return m_max_width;
}

std::uint32_t Sequencer::getMaxHeight() const {
  return m_max_height;
}

std::size_t Sequencer::getSize() const {
  return m_size;
}

}  // namespace hisui::video
