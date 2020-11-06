#pragma once

#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>

#include <cstdint>

#include "video/decoder.hpp"

namespace hisui::webm::input {

class VideoContext;

}

namespace hisui::video {

class YUVImage;

class VPXDecoder : public Decoder {
 public:
  explicit VPXDecoder(hisui::webm::input::VideoContext*);

  ~VPXDecoder();

  const YUVImage* getImage(const std::uint64_t);

 private:
  ::vpx_codec_ctx_t m_codec;
  std::uint64_t m_current_timestamp = 0;
  std::uint64_t m_next_timestamp = 0;
  ::vpx_image_t* m_current_vpx_image = nullptr;
  ::vpx_image_t* m_next_vpx_image = nullptr;
  YUVImage* m_current_yuv_image = nullptr;

  void updateVPXImage(const std::uint64_t);

  void updateCurrentYUVImage();

  void updateVPXImageByTimestamp(const std::uint64_t);
};

}  // namespace hisui::video
