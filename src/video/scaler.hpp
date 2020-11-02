#pragma once

#include <cstdint>

namespace hisui::video {

class YUVImage;

class Scaler {
 public:
  Scaler(const std::uint32_t t_width, const std::uint32_t t_height);
  virtual ~Scaler();

  virtual const YUVImage* scale(const YUVImage* src) = 0;

 protected:
  YUVImage* m_scaled;
  std::uint32_t m_width;
  std::uint32_t m_height;
};

}  // namespace hisui::video
