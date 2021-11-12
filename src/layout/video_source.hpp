#pragma once

#include <memory>

#include "layout/source.hpp"
#include "video/yuv.hpp"

namespace hisui::layout {
class VideoSource : public Source {
 public:
  explicit VideoSource(const SourceParameters&);
  const std::shared_ptr<hisui::video::YUVImage> getYUV(const std::uint64_t);

 private:
  std::shared_ptr<hisui::video::Source> m_source;
};

}  // namespace hisui::layout
