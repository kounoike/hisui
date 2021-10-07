#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "audio/source.hpp"
#include "video/source.hpp"

namespace hisui::layout {

struct SourceParameters {
  const std::string name;
  const std::uint64_t start_time;
  const std::uint64_t end_time;
};

struct Source {
  explicit Source(const SourceParameters&);
  virtual ~Source() {}
  std::string name;
  std::uint64_t start_time;
  std::uint64_t end_time;
  void SubstructTime(const std::uint64_t);
};

struct VideoSource : public Source {
  explicit VideoSource(const SourceParameters&);
  std::shared_ptr<hisui::video::Source> source;
};

struct AudioSource : public Source {
  explicit AudioSource(const SourceParameters&);
  std::shared_ptr<hisui::audio::Source> source;
};

}  // namespace hisui::layout
