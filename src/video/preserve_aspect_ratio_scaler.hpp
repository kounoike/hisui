#pragma once

#include <cstdint>

#include "libyuv/scale.h"

#include "video/scaler.hpp"

namespace boost {

template <typename IntType>
class rational;

}

namespace hisui::video {

class YUVImage;

class PreserveAspectRatioScaler : public Scaler {
 public:
  PreserveAspectRatioScaler(const std::uint32_t,
                            const std::uint32_t,
                            const libyuv::FilterMode);
  ~PreserveAspectRatioScaler();
  const YUVImage* scale(const YUVImage*);

 private:
  const libyuv::FilterMode m_filter_mode;
  YUVImage* m_intermediate;

  const YUVImage* simpleScale(const YUVImage*);
  const YUVImage* marginInHeightScale(const YUVImage*,
                                      const std::uint32_t,
                                      const boost::rational<std::uint32_t>&);
  const YUVImage* marginInWidthScale(const YUVImage*,
                                     const std::uint32_t,
                                     const boost::rational<std::uint32_t>&);
};

}  // namespace hisui::video
