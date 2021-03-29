#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

struct ResolutionWithTimestamp {
  std::uint64_t timestamp;
  std::uint32_t width;
  std::uint32_t height;
};

void tag_invoke(const boost::json::value_from_tag&,
                boost::json::value& jv,  // NOLINT
                const ResolutionWithTimestamp& rwt);

class Reporter {
 public:
  Reporter& operator=(const Reporter&) = delete;
  Reporter(Reporter&&) = delete;
  Reporter& operator=(Reporter&&) = delete;
  std::string makeSuccessReport();
  void registerResolutionChange(const std::string&,
                                const ResolutionWithTimestamp&);

  static void open();
  static bool hasInstance();
  static Reporter& getInstance();
  static void close();

 private:
  Reporter() = default;
  ~Reporter() = default;
  void collectVersions();
  std::map<std::string, std::vector<ResolutionWithTimestamp>>
      m_resolution_changes;

  boost::json::object report;
  inline static Reporter* m_reporter = nullptr;
};

}  // namespace hisui::report
