#pragma once

#include <memory>

#include "layout/source.hpp"
#include "video/yuv.hpp"

namespace hisui::layout {

struct VideoSource : public Source {
  explicit VideoSource(const SourceParameters&);
  const std::shared_ptr<video::YUVImage> getYUV(const std::uint64_t);

  std::shared_ptr<hisui::video::Source> source;
};

}  // namespace hisui::layout
