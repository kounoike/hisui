#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <initializer_list>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <boost/json.hpp>

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/async_webm_muxer.hpp"
#include "muxer/faststart_mp4_muxer.hpp"
#include "muxer/muxer.hpp"
#include "muxer/simple_mp4_muxer.hpp"
#include "report/success.hpp"
#include "video/openh264_handler.hpp"

int main(int argc, char** argv) {
  CLI::App app{"hisui"};
  hisui::Config config;

  hisui::set_cli_options(&app, &config);

  CLI11_PARSE(app, argc, argv);

  if (config.out_container == hisui::config::OutContainer::WebM &&
      config.out_audio_codec == hisui::config::OutAudioCodec::FDK_AAC) {
    spdlog::error("hisui does not support AAC output in WebM");
    return 1;
  }

  if (config.verbose) {
    spdlog::set_level(spdlog::level::debug);
  } else {
    spdlog::set_level(config.log_level);
  }
  spdlog::debug("log level={}", config.log_level);

  std::ifstream i(config.in_metadata_filename);
  if (!i.is_open()) {
    spdlog::error("failed to open metadata json file: {}",
                  config.in_metadata_filename);
    return 1;
  }
  std::string string_json((std::istreambuf_iterator<char>(i)),
                          std::istreambuf_iterator<char>());
  boost::json::error_code ec;
  boost::json::value jv = boost::json::parse(string_json, ec);
  if (ec) {
    spdlog::error("failed to parse metadata json file: message", ec.message());
    return 1;
  }

  const hisui::Metadata metadata =
      hisui::parse_metadata(config.in_metadata_filename, jv);

  if (!config.openh264.empty()) {
    try {
      hisui::video::OpenH264Handler::open(config.openh264);
    } catch (const std::exception& e) {
      spdlog::warn("failed to open openh264 library: {}", e.what());
    }
  }

  hisui::muxer::Muxer* muxer = nullptr;
  if (config.out_container == hisui::config::OutContainer::WebM) {
    muxer = new hisui::muxer::AsyncWebMMuxer(config, metadata);
  } else if (config.out_container == hisui::config::OutContainer::MP4) {
    if (config.mp4_muxer == hisui::config::MP4Muxer::Simple) {
      muxer = new hisui::muxer::SimpleMP4Muxer(config, metadata);
    } else if (config.mp4_muxer == hisui::config::MP4Muxer::Faststart) {
      muxer = new hisui::muxer::FaststartMP4Muxer(config, metadata);
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
    return 1;
  }
  delete muxer;

  if (config.out_success_report != "") {
    std::ofstream os(std::filesystem::path(config.out_success_report) /
                     fmt::format("{}_success.json", metadata.getRecordingID()));
    hisui::report::SuccessReporter reporter;
    os << reporter.make();
  }

  return 0;
}
