#include "layout/compose.hpp"

#include "config.hpp"
#include "layout/metadata.hpp"

namespace hisui::layout {

int compose(const hisui::Config& t_config) {
  auto config = t_config;
  auto metadata = hisui::layout::parse_metadata(config.layout);
  metadata.copyToConfig(&config);

  return 0;
}

}  // namespace hisui::layout
