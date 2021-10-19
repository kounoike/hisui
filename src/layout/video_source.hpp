#pragma once

#include <memory>

#include "layout/source.hpp"

namespace hisui::layout {

struct VideoSource : public Source {
  explicit VideoSource(const SourceParameters&);
  std::shared_ptr<hisui::video::Source> source;
};

}  // namespace hisui::layout
