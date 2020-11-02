#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace hisui::video {

class YUVImage {
 public:
  std::array<std::uint8_t*, 3> yuv;

  YUVImage(const std::uint32_t, const std::uint32_t);
  ~YUVImage();

  bool checkWidthAndHeight(const std::uint32_t, const std::uint32_t) const;
  void setWidthAndHeight(const std::uint32_t, const std::uint32_t);

  std::uint32_t getWidth(const int) const;
  std::uint32_t getHeight(const int) const;

  void setBlack();

 private:
  std::uint32_t m_width;
  std::uint32_t m_height;
};

YUVImage* create_black_yuv_image(const std::uint32_t, const std::uint32_t);

void merge_yuv_planes_from_top_left(unsigned char*,
                                    const std::size_t,
                                    const std::size_t,
                                    const std::vector<const unsigned char*>&,
                                    const std::size_t,
                                    const std::uint32_t,
                                    const std::uint32_t,
                                    const unsigned char);

}  // namespace hisui::video
