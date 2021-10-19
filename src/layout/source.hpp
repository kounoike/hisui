#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "audio/source.hpp"
#include "video/source.hpp"

namespace hisui::layout {

struct TrimIntervals {
  std::vector<std::pair<std::uint64_t, std::uint64_t>> trim_intervals;
};

bool operator==(TrimIntervals const& left, TrimIntervals const& right);

std::ostream& operator<<(std::ostream& os, const TrimIntervals&);

struct SourceInterval {
  std::uint64_t start_time;
  std::uint64_t end_time;
};

bool operator==(SourceInterval const& left, SourceInterval const& right);

std::ostream& operator<<(std::ostream& os, const SourceInterval&);

struct SourceParameters {
  const std::filesystem::path& file_path;
  const std::string& connection_id;
  const std::uint64_t start_time;
  const std::uint64_t end_time;
};

struct Source {
  explicit Source(const SourceParameters&);
  virtual ~Source() {}
  std::filesystem::path file_path;
  std::string connection_id;
  SourceInterval interval;
  void substructTrimIntervals(const TrimIntervals&);
};

struct VideoSource : public Source {
  explicit VideoSource(const SourceParameters&);
  std::shared_ptr<hisui::video::Source> source;
};

struct AudioSource : public Source {
  explicit AudioSource(const SourceParameters&);
  std::shared_ptr<hisui::audio::Source> source;
};

struct SubstructTrimIntervalsParameters {
  const SourceInterval& interval;
  const std::vector<std::pair<std::uint64_t, std::uint64_t>>& trim_intervals;
};

SourceInterval substruct_trim_intervals(
    const SubstructTrimIntervalsParameters&);

}  // namespace hisui::layout
