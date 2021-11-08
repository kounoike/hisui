#include "layout/compose.hpp"

#include <spdlog/spdlog.h>

#include <memory>

#include "config.hpp"
#include "constants.hpp"
#include "layout/metadata.hpp"
#include "layout/vpx_video_producer.hpp"
#include "muxer/async_webm_muxer.hpp"
#include "muxer/faststart_mp4_muxer.hpp"
#include "muxer/muxer.hpp"
#include "muxer/no_video_producer.hpp"
#include "muxer/simple_mp4_muxer.hpp"

namespace hisui::layout {

int compose(const hisui::Config& t_config) {
  auto config = t_config;
  auto metadata = hisui::layout::parse_metadata(config.layout);
  metadata.copyToConfig(&config);

  std::shared_ptr<hisui::muxer::Muxer> muxer;
  std::shared_ptr<muxer::VideoProducer> video_producer;
  if (config.audio_only) {
    video_producer = std::make_shared<muxer::NoVideoProducer>();
  } else {
    video_producer = std::make_shared<VPXVideoProducer>(
        config, VPXVideoProducerParameters{
                    .regions = metadata.getRegions(),
                    .resolution = metadata.getResolution(),
                    .duration = metadata.getMaxEndTime(),
                    .timescale = config.out_container ==
                                         hisui::config::OutContainer::WebM
                                     ? hisui::Constants::NANO_SECOND
                                     : 16000,  // TODO(haruyama): 整理する
                });
  }

  auto audio_archive_items = metadata.getAudioArchiveItems();

  if (config.out_container == hisui::config::OutContainer::WebM) {
    muxer = std::make_shared<hisui::muxer::AsyncWebMMuxer>(
        config, hisui::muxer::AsyncWebMMuxerParametersForLayout{
                    .audio_archive_items = audio_archive_items,
                    .video_producer = video_producer,
                    .duration = metadata.getMaxEndTime()});

  } else if (config.out_container == hisui::config::OutContainer::MP4) {
    auto params = hisui::muxer::MP4MuxerParametersForLayout{
        .audio_archive_items = audio_archive_items,
        .video_producer = video_producer,
        .duration = metadata.getMaxEndTime()};
    if (config.mp4_muxer == hisui::config::MP4Muxer::Simple) {
      muxer = std::make_shared<hisui::muxer::SimpleMP4Muxer>(config, params);
    } else if (config.mp4_muxer == hisui::config::MP4Muxer::Faststart) {
      muxer = std::make_shared<hisui::muxer::FaststartMP4Muxer>(config, params);

    } else {
      throw std::runtime_error("config.mp4_muxer is invalid");
    }
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

  return 0;
}

}  // namespace hisui::layout
