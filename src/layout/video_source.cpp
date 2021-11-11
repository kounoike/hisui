#include "layout/video_source.hpp"

#include <string>

#include "video/webm_source.hpp"
#include "video/yuv.hpp"

namespace hisui::layout {

VideoSource::VideoSource(const SourceParameters& params) : Source(params) {
  source = std::make_shared<hisui::video::WebMSource>(file_path.string());
}

const std::shared_ptr<hisui::video::YUVImage> VideoSource::getYUV(
    const std::uint64_t t) {
  return source->getYUV(encoding_interval.getSubstructLower(t));
}

}  // namespace hisui::layout
