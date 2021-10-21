#pragma once

#include <cstdint>

#include "constants.hpp"
#include "layout/metadata.hpp"
#include "muxer/video_producer.hpp"

namespace hisui {

class Config;

}  // namespace hisui

namespace hisui::layout {

class VPXVideoProducer : public hisui::muxer::VideoProducer {
 public:
  VPXVideoProducer(
      const hisui::Config&,
      const Metadata&,
      const std::uint64_t timescale = hisui::Constants::NANO_SECOND);
};

}  // namespace hisui::layout
