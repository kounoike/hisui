#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

class Reporter {
 public:
  Reporter& operator=(const Reporter&) = delete;
  Reporter(Reporter&&) = delete;
  Reporter& operator=(Reporter&&) = delete;
  std::string makeSuccessReport();
  void registerResolutionChange(
      const std::string& filename,
      const std::tuple<std::uint64_t, std::uint32_t, std::uint32_t>& value);

  static void open();
  static bool hasInstance();
  static Reporter& getInstance();
  static void close();

 private:
  Reporter() = default;
  ~Reporter() = default;
  void collectVersions();
  std::map<std::string,
           std::vector<std::tuple<std::uint64_t, std::uint32_t, std::uint32_t>>>
      m_resolution_changes;

  boost::json::object report;
  inline static Reporter* m_reporter = nullptr;
};

}  // namespace hisui::report
