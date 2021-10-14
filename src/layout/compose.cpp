#include "layout/compose.hpp"

#include "config.hpp"
#include "layout/metadata.hpp"

namespace hisui::layout {

int compose(const hisui::Config& config) {
  hisui::layout::parse_metadata(config.layout);
  return 0;
}

}  // namespace hisui::layout
