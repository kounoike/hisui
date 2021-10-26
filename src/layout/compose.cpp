#include "layout/compose.hpp"

#include <spdlog/spdlog.h>

#include "config.hpp"
#include "layout/metadata.hpp"
#include "layout/vpx_video_producer.hpp"
#include "muxer/async_webm_muxer.hpp"
#include "muxer/muxer.hpp"
#include "muxer/no_video_producer.hpp"

namespace hisui::layout {

int compose(const hisui::Config& t_config) {
  auto config = t_config;
  auto metadata = hisui::layout::parse_metadata(config.layout);
  metadata.copyToConfig(&config);

  // hisui::muxer::Muxer* muxer = nullptr;
  hisui::muxer::AsyncWebMMuxer* muxer = nullptr;
  hisui::muxer::VideoProducer* video_producer = nullptr;
  if (config.audio_only) {
    video_producer = new muxer::NoVideoProducer();
  } else {
    video_producer = new VPXVideoProducer(config, metadata);
  }

  if (config.out_container == hisui::config::OutContainer::WebM) {
    muxer = new hisui::muxer::AsyncWebMMuxer(config,
                                             metadata.getMaxStopTimeOffset());
    muxer->setVideoProducer(video_producer);
    muxer->setAudioArchives(metadata.getAudioArchives());

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
  // delete video_producer;
  delete muxer;

  return 0;
}

}  // namespace hisui::layout
