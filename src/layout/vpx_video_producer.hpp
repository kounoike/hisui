#pragma once

#include <cstdint>
#include <memory>

#include "constants.hpp"
#include "layout/cell_util.hpp"
#include "layout/composer.hpp"
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
  void produce();

 private:
  Resolution m_resolution;
  std::shared_ptr<Composer> m_layout_composer;
};

}  // namespace hisui::layout
