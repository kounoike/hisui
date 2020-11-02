#pragma once

#include <cstdint>

#include "libyuv/scale.h"

#include "video/scaler.hpp"

namespace hisui::video {

class YUVImage;

class SimpleScaler : public Scaler {
 public:
  SimpleScaler(const std::uint32_t,
               const std::uint32_t,
               const libyuv::FilterMode);
  const YUVImage* scale(const YUVImage* src);

  const libyuv::FilterMode m_filter_mode;
};

}  // namespace hisui::video
