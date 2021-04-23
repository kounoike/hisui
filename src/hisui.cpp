#include <bits/exception.h>
#include <spdlog/common.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <stdexcept>
#include <string>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "config.hpp"
#include "datetime.hpp"
#include "metadata.hpp"
#include "muxer/async_webm_muxer.hpp"
#include "muxer/faststart_mp4_muxer.hpp"
#include "muxer/muxer.hpp"
#include "muxer/simple_mp4_muxer.hpp"
#include "report/reporter.hpp"
#include "video/openh264_handler.hpp"

int main(int argc, char** argv) {
  CLI::App app{"hisui"};
  hisui::Config config;

  hisui::set_cli_options(&app, &config);

  CLI11_PARSE(app, argc, argv);

  config.validate();

  if (config.verbose) {
    spdlog::set_level(spdlog::level::debug);
  } else {
    spdlog::set_level(config.log_level);
  }
  spdlog::debug("log level={}", config.log_level);

  if (!config.openh264.empty()) {
    try {
      hisui::video::OpenH264Handler::open(config.openh264);
    } catch (const std::exception& e) {
      spdlog::warn("failed to open openh264 library: {}", e.what());
    }
  }

  if (config.enabledReport()) {
    hisui::report::Reporter::open();
  }

  hisui::MetadataSet metadata_set(
      hisui::parse_metadata(config.in_metadata_filename));

  if (!config.screen_capture_metadata_filename.empty()) {
    metadata_set.setPrefered(
        hisui::parse_metadata(config.screen_capture_metadata_filename));
  } else if (!config.screen_capture_connection_id.empty()) {
    metadata_set.split(config.screen_capture_connection_id);
  }

  hisui::muxer::Muxer* muxer = nullptr;
  if (config.out_container == hisui::config::OutContainer::WebM) {
    muxer = new hisui::muxer::AsyncWebMMuxer(config, metadata_set);
  } else if (config.out_container == hisui::config::OutContainer::MP4) {
    if (config.mp4_muxer == hisui::config::MP4Muxer::Simple) {
      muxer = new hisui::muxer::SimpleMP4Muxer(config, metadata_set);
    } else if (config.mp4_muxer == hisui::config::MP4Muxer::Faststart) {
      muxer = new hisui::muxer::FaststartMP4Muxer(config, metadata_set);
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
    if (config.enabledFailureReport()) {
      std::ofstream os(std::filesystem::path(config.failure_report) /
                       fmt::format("{}_{}_failure.json",
                                   hisui::datetime::get_current_utc_string(),
                                   metadata_set.getNormal().getRecordingID()));
      os << hisui::report::Reporter::getInstance().makeFailureReport(e.what());
      hisui::report::Reporter::close();
    }
    return 1;
  }
  delete muxer;

  if (config.enabledSuccessReport()) {
    std::ofstream os(std::filesystem::path(config.success_report) /
                     fmt::format("{}_{}_success.json",
                                 hisui::datetime::get_current_utc_string(),
                                 metadata_set.getNormal().getRecordingID()));
    os << hisui::report::Reporter::getInstance().makeSuccessReport();
    hisui::report::Reporter::close();
  }

  if (!config.openh264.empty()) {
    hisui::video::OpenH264Handler::close();
  }

  return 0;
}
