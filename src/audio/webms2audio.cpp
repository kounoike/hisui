#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iterator>
#include <map>
#include <string>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <nlohmann/json.hpp>

#include "audio/basic_processor.hpp"
#include "config.hpp"
#include "metadata.hpp"

int main(int argc, char** argv) {
  CLI::App app{"hisui audio mixer"};
  hisui::Config config;

  hisui::set_cli_options(&app, &config);

  CLI11_PARSE(app, argc, argv);

  spdlog::set_level(config.log_level);
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

  hisui::audio::BasicProcessor processor(config, metadata);
  processor.run();

  return 0;
}
