#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "audio/source.hpp"
#include "layout/interval.hpp"
#include "util/interval.hpp"
#include "video/source.hpp"

namespace hisui::layout {

struct TrimIntervals {
  std::vector<Interval> trim_intervals;
};

bool operator==(TrimIntervals const& left, TrimIntervals const& right);

std::ostream& operator<<(std::ostream& os, const TrimIntervals&);

struct SourceParameters {
  const std::filesystem::path& file_path;
  const std::string& connection_id;
  const double start_time;
  const double end_time;
};

struct Source {
  explicit Source(const SourceParameters&);
  virtual ~Source() {}
  std::filesystem::path file_path;
  std::string connection_id;
  Interval source_interval;
  hisui::util::Interval encoding_interval{0, 0};
  void substructTrimIntervals(const TrimIntervals&);
};

struct SubstructTrimIntervalsParameters {
  const Interval& interval;
  const std::vector<Interval>& trim_intervals;
};

Interval substruct_trim_intervals(const SubstructTrimIntervalsParameters&);

}  // namespace hisui::layout
