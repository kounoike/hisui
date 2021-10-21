#include "layout/vpx_video_producer.hpp"

#include <cstdint>

#include <boost/rational.hpp>

#include "config.hpp"
#include "layout/metadata.hpp"
#include "metadata.hpp"
#include "muxer/video_producer.hpp"
#include "video/basic_sequencer.hpp"
#include "video/buffer_vpx_encoder.hpp"
#include "video/composer.hpp"
#include "video/grid_composer.hpp"
#include "video/parallel_grid_composer.hpp"
#include "video/sequencer.hpp"
#include "video/vpx.hpp"

namespace hisui::layout {

VPXVideoProducer::VPXVideoProducer(const hisui::Config& t_config,
                                   const Metadata& t_metadata,
                                   const std::uint64_t timescale)
    : VideoProducer({.show_progress_bar = t_config.show_progress_bar}) {
  auto resolution = t_metadata.getResolution();
  hisui::video::VPXEncoderConfig vpx_config(resolution.width, resolution.height,
                                            t_config);

  auto regions = t_metadata.getRegions();
  for (auto& r : regions) {
    r->setEncodingInterval();
  }
  // TODO(haruyama): Cell の設定
  // TODO(haruyama): composer の設定

  m_encoder =
      new hisui::video::BufferVPXEncoder(&m_buffer, vpx_config, timescale);

  m_max_stop_time_offset = static_cast<double>(t_metadata.getMaxEndTime());
  m_frame_rate = t_config.out_video_frame_rate;
}

// TODO(haruyama): produce()

}  // namespace hisui::layout
