#pragma once

#include <cstdint>

#include "constants.hpp"
#include "muxer/video_producer.hpp"

namespace hisui {

class Config;
class Metadata;

}  // namespace hisui

namespace hisui::muxer {

class VPXVideoProducer : public VideoProducer {
 public:
  VPXVideoProducer(
      const hisui::Config&,
      const hisui::Metadata& i,
      const std::uint64_t timescale = hisui::Constants::NANO_SECOND);
};

}  // namespace hisui::muxer
