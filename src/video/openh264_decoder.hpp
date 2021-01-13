#pragma once

#include <codec/api/svc/codec_def.h>

#include <cstdint>

#include "video/decoder.hpp"

class ISVCDecoder;

namespace hisui::webm::input {

class VideoContext;

}

namespace hisui::video {

class YUVImage;

class OpenH264Decoder : public Decoder {
 public:
  explicit OpenH264Decoder(hisui::webm::input::VideoContext*);
  ~OpenH264Decoder();

  const YUVImage* getImage(const std::uint64_t);

 private:
  ::ISVCDecoder* m_decoder = nullptr;
  std::uint64_t m_current_timestamp = 0;
  std::uint64_t m_next_timestamp = 0;
  YUVImage* m_current_yuv_image = nullptr;
  ::SBufferInfo m_current_buffer_info;
  ::SBufferInfo m_next_buffer_info;
  std::uint8_t* m_tmp_yuv[3];

  void updateImage(const std::uint64_t);

  void updateImageByTimestamp(const std::uint64_t);
};

}  // namespace hisui::video
