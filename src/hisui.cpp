#include <bits/exception.h>

#include <fstream>
#include <iterator>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/async_webm_muxer.hpp"
#include "muxer/webm_muxer.hpp"

int main(int argc, char** argv) {
  CLI::App app{"hisui"};
  hisui::Config config;

  hisui::set_cli_options(&app, &config);

  CLI11_PARSE(app, argc, argv);

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
  nlohmann::json j;
  i >> j;

  if (j["archives"] == nullptr) {
    spdlog::error("not metadata json file: {}", config.in_metadata_filename);
    return 1;
  }

  if (std::size(j["archives"]) == 0) {
    spdlog::error("metadata json file does not include archives: {}",
                  config.in_metadata_filename);
    return 1;
  }

  const hisui::Metadata metadata =
      hisui::parse_metadata(config.in_metadata_filename, j);

  hisui::muxer::WebMMuxer* muxer =
      new hisui::muxer::AsyncWebMMuxer(config, metadata);
  muxer->run();
  delete muxer;

  return 0;
}
