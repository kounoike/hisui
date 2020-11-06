#pragma once

#include <libyuv/scale.h>
#include <spdlog/common.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include <boost/rational.hpp>

#include "constants.hpp"

namespace CLI {

class App;

}

namespace hisui {

namespace config {

enum struct AudioMixer {
  Simple,
  Vttoth,
};

enum OutVideoCodec {
  VP8 = hisui::Constants::VP8_FOURCC,
  VP9 = hisui::Constants::VP9_FOURCC,
};

enum struct VideoComposer {
  Grid,
  ParallelGrid,
};

enum struct VideoScaler {
  Simple,
  PreserveAspectRatio,
};

}  // namespace config

class Config {
 public:
  std::string in_metadata_filename;
  config::OutVideoCodec out_video_codec = config::OutVideoCodec::VP9;
  std::uint32_t out_video_bit_rate = 0;

  boost::rational<std::uint64_t> out_video_frame_rate =
      boost::rational<std::uint64_t>(25, 1);
  std::uint32_t libvpx_cq_level = 10;
  std::uint32_t libvpx_min_q = 3;
  std::uint32_t libvpx_max_q = 40;

  std::string out_webm_filename = "";

  std::size_t max_columns = 3;

  bool verbose = false;

  bool show_progress_bar = true;

#ifdef NDEBUG
  spdlog::level::level_enum log_level = spdlog::level::info;
#else
  spdlog::level::level_enum log_level = spdlog::level::debug;
#endif
  std::uint32_t scaling_width = 320;
  std::uint32_t scaling_height = 240;

  std::uint32_t libvpx_threads = 0;
  std::int32_t libvpx_cpu_used = 4;
  std::uint32_t libvp9_frame_parallel = 1;

  libyuv::FilterMode libyuv_filter_mode = libyuv::kFilterBox;

  config::VideoComposer video_composer = config::VideoComposer::Grid;
  config::VideoScaler video_scaler = config::VideoScaler::PreserveAspectRatio;

  config::AudioMixer audio_mixer = config::AudioMixer::Simple;
};

void set_cli_options(CLI::App* app, Config* config);

}  // namespace hisui
