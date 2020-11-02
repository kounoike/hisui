#include "video/buffer_vpx_encoder.hpp"

#include <bits/exception.h>
#include <vpx/vpx_encoder.h>

#include <algorithm>
#include <stdexcept>

#include "boost/rational.hpp"
#include "fmt/core.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"

#include "constants.hpp"
#include "video/vpx.hpp"

namespace hisui::video {

BufferVPXEncoder::BufferVPXEncoder(
    std::queue<hisui::webm::output::FrameTuple>* t_buffer,
    const VPXEncoderConfig& config)
    : m_buffer(t_buffer) {
  m_width = config.width;
  m_height = config.height;
  m_fps = config.fps;
  m_fourcc = config.fourcc;
  if (!::vpx_img_alloc(&m_raw_vpx_image, VPX_IMG_FMT_I420, m_width, m_height,
                       0)) {
    throw std::runtime_error("vpx_img_alloc() failed");
  }

  create_vpx_codec_ctx_t_for_encoding(&m_codec, config);
}

void BufferVPXEncoder::outputImage(const std::vector<unsigned char>& yuv) {
  update_vpx_image_by_yuv_data(&m_raw_vpx_image, yuv);
  encodeFrame(&m_codec, &m_raw_vpx_image, m_frame++, 0);
}

void BufferVPXEncoder::flush() {
  while (encodeFrame(&m_codec, nullptr, -1, 0)) {
  }
}

BufferVPXEncoder::~BufferVPXEncoder() {
  if (m_frame > 0) {
    spdlog::debug("VPXEncoder: number of frames: {}", m_frame);
    spdlog::debug("VPXEncoder: final average bitrate (kbps): {}",
                  m_sum_of_bits * m_fps.numerator() / m_fps.denominator() /
                      static_cast<std::uint64_t>(m_frame) / 1024);
  }
  ::vpx_img_free(&m_raw_vpx_image);
  ::vpx_codec_destroy(&m_codec);
}

bool BufferVPXEncoder::encodeFrame(::vpx_codec_ctx_t* codec,
                                   ::vpx_image_t* img,
                                   const int frame_index,
                                   const int flags) {
  const ::vpx_codec_err_t ret = ::vpx_codec_encode(codec, img, frame_index, 1,
                                                   flags, VPX_DL_GOOD_QUALITY);
  if (ret != VPX_CODEC_OK) {
    throw std::runtime_error(fmt::format("Failed to encode frame: error='{}'",
                                         ::vpx_codec_err_to_string(ret)));
  }

  ::vpx_codec_iter_t iter = nullptr;
  const ::vpx_codec_cx_pkt_t* pkt = nullptr;
  bool got_pkts = false;
  while ((pkt = ::vpx_codec_get_cx_data(codec, &iter)) != nullptr) {
    got_pkts = true;

    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      std::uint64_t pts_ns = static_cast<std::uint64_t>(pkt->data.frame.pts) *
                             hisui::Constants::NANO_SECOND *
                             m_fps.denominator() / m_fps.numerator();
      std::uint8_t* buf = static_cast<uint8_t*>(pkt->data.frame.buf);
      std::uint8_t* data = new std::uint8_t[pkt->data.frame.sz];
      std::copy(buf, buf + pkt->data.frame.sz, data);
      m_buffer->emplace(pts_ns, data, pkt->data.frame.sz,
                        pkt->data.frame.flags & VPX_FRAME_IS_KEY);

      m_sum_of_bits += pkt->data.frame.sz * 8;

      if (m_frame > 0 && m_frame % 100 == 0 && frame_index > 0) {
        spdlog::trace("VPXEncoder: frame index: {}", m_frame);
        spdlog::trace("VPXEncoder: average bitrate (kbps): {}",
                      m_sum_of_bits * m_fps.numerator() / m_fps.denominator() /
                          static_cast<std::uint64_t>(m_frame) / 1024);
      }
    }
  }

  return got_pkts;
}

std::uint32_t BufferVPXEncoder::getFourcc() const {
  return m_fourcc;
}

}  // namespace hisui::video
