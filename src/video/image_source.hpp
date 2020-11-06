#pragma once

#include <cstdint>
#include <string>

#include "video/source.hpp"

namespace hisui::video {

class YUVImage;

class ImageSource : public Source {
 public:
  explicit ImageSource(const std::string&);
  ~ImageSource();
  const YUVImage* getYUV(const std::uint64_t);
  std::uint32_t getWidth() const;
  std::uint32_t getHeight() const;

 private:
  std::uint32_t m_width;
  std::uint32_t m_height;
  YUVImage* m_yuv_image;
};

}  // namespace hisui::video
