#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

#include "webm/input/context.hpp"

namespace hisui::webm::input {

class VideoContext : public Context {
 public:
  VideoContext();
  ~VideoContext();

  void reset();

  bool init(std::FILE*);

  void setFilename(const std::string&);
  std::string getFilename() const;
  std::uint32_t getFourcc() const;
  std::uint32_t getWidth() const;
  std::uint32_t getHeight() const;

 private:
  std::string m_filename = "";
  std::uint32_t m_fourcc;
  std::uint32_t m_width;
  std::uint32_t m_height;
};

}  // namespace hisui::webm::input
