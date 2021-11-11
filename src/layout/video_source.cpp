#include "layout/video_source.hpp"

#include <string>

#include "video/webm_source.hpp"
#include "video/yuv.hpp"

namespace hisui::layout {

VideoSource::VideoSource(const SourceParameters& params) : Source(params) {
  m_source = std::make_shared<hisui::video::WebMSource>(m_file_path.string());
}

const std::shared_ptr<hisui::video::YUVImage> VideoSource::getYUV(
    const std::uint64_t t) {
  return m_source->getYUV(m_encoding_interval.getSubstructLower(t));
}

}  // namespace hisui::layout
