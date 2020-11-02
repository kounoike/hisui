#pragma once

#include <cstdint>
#include <cstdio>

#include "webm/input/context.hpp"

namespace hisui::webm::input {

class VideoContext : public Context {
 public:
  VideoContext();
  ~VideoContext();

  void reset();

  bool init(std::FILE*);

  std::uint32_t getFourcc() const;
  std::uint32_t getWidth() const;
  std::uint32_t getHeight() const;

 private:
  std::uint32_t m_fourcc;
  std::uint32_t m_width;
  std::uint32_t m_height;
};

}  // namespace hisui::webm::input
