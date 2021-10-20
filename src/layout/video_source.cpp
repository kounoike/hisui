#include "layout/video_source.hpp"

#include <string>

#include "video/webm_source.hpp"

namespace hisui::layout {

VideoSource::VideoSource(const SourceParameters& params) : Source(params) {
  source = std::make_shared<hisui::video::WebMSource>(file_path.string());
}

}  // namespace hisui::layout
