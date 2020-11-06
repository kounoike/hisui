#pragma once

#include <cstdint>

namespace hisui::webm::input {
class VideoContext;
}

namespace hisui::video {

class YUVImage;

class Decoder {
 public:
  explicit Decoder(hisui::webm::input::VideoContext*);

  virtual ~Decoder();
  virtual const YUVImage* getImage(const std::uint64_t) = 0;

  std::uint32_t getWidth() const;
  std::uint32_t getHeight() const;

 protected:
  hisui::webm::input::VideoContext* m_webm;
  std::uint64_t m_duration;
  bool m_is_time_over = false;
  bool m_finished_webm = false;
  std::uint32_t m_width;
  std::uint32_t m_height;
  YUVImage* m_black_yuv_image;
};

}  // namespace hisui::video
