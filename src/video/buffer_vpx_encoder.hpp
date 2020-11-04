#pragma once

#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>

#include <cstdint>
#include <queue>
#include <vector>

#include <boost/rational.hpp>

#include "video/encoder.hpp"
#include "webm/output/context.hpp"

namespace hisui::video {

class VPXEncoderConfig;

class BufferVPXEncoder : public Encoder {
 public:
  BufferVPXEncoder(std::queue<hisui::webm::output::FrameTuple>*,
                   const VPXEncoderConfig& config);
  void outputImage(const std::vector<unsigned char>&);
  void flush();
  ~BufferVPXEncoder();

  std::uint32_t getFourcc() const;

 private:
  std::queue<hisui::webm::output::FrameTuple>* m_buffer;
  std::uint32_t m_width;
  std::uint32_t m_height;
  boost::rational<std::uint64_t> m_fps;
  std::uint32_t m_fourcc;
  int m_frame = 0;
  ::vpx_codec_ctx_t m_codec;
  ::vpx_image_t m_raw_vpx_image;
  std::uint64_t m_sum_of_bits = 0;

  bool encodeFrame(::vpx_codec_ctx_t*, ::vpx_image_t*, const int, const int);
};

}  // namespace hisui::video
