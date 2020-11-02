#pragma once

#include <cstdint>
#include <vector>

namespace hisui::video {

class YUVImage;

class Composer {
 public:
  virtual ~Composer() = default;
  virtual void compose(std::vector<unsigned char>*,
                       const std::vector<const YUVImage*>&) = 0;

  std::uint32_t getWidth() const;
  std::uint32_t getHeight() const;

 protected:
  std::uint32_t m_width;
  std::uint32_t m_height;
};

}  // namespace hisui::video
