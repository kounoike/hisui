#pragma once

#include <memory>

#include "layout/source.hpp"

namespace hisui::layout {

struct AudioSource : public Source {
  explicit AudioSource(const SourceParameters&);
  std::shared_ptr<hisui::audio::Source> source;
};

}  // namespace hisui::layout
