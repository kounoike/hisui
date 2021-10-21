#include "layout/compose.hpp"

#include <spdlog/spdlog.h>

#include "config.hpp"
#include "layout/async_webm_muxer.hpp"
#include "layout/metadata.hpp"
#include "muxer/muxer.hpp"

namespace hisui::layout {

int compose(const hisui::Config& t_config) {
  auto config = t_config;
  auto metadata = hisui::layout::parse_metadata(config.layout);
  metadata.copyToConfig(&config);

  hisui::muxer::Muxer* muxer = nullptr;
  if (config.out_container == hisui::config::OutContainer::WebM) {
    muxer = new AsyncWebMMuxer(config, metadata);
    // } else if (config.out_container == hisui::config::OutContainer::MP4) {
    //   if (config.mp4_muxer == hisui::config::MP4Muxer::Simple) {
    //     muxer = new hisui::muxer::SimpleMP4Muxer(config, metadata_set);
    //   } else if (config.mp4_muxer == hisui::config::MP4Muxer::Faststart) {
    //     muxer = new hisui::muxer::FaststartMP4Muxer(config, metadata_set);
    //   } else {
    //     throw std::runtime_error("config.mp4_muxer is invalid");
    //   }
  } else {
    throw std::runtime_error("config.out_container is invalid");
  }
  try {
    muxer->setUp();
    muxer->run();
  } catch (const std::exception& e) {
    spdlog::error("muxing failed: {}", e.what());
    muxer->cleanUp();
    // if (config.enabledFailureReport()) {
    //   std::ofstream os(std::filesystem::path(config.failure_report) /
    //                    fmt::format("{}_{}_failure.json",
    //                                hisui::datetime::get_current_utc_string(),
    //                                metadata_set.getNormal().getRecordingID()));
    //   os << hisui::report::Reporter::getInstance().makeFailureReport(e.what());
    //   hisui::report::Reporter::close();
    // }
    return 1;
  }
  delete muxer;

  return 0;
}

}  // namespace hisui::layout
