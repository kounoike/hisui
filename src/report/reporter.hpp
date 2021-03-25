#pragma once

#include <string>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

class Reporter {
 public:
  Reporter& operator=(const Reporter&) = delete;
  Reporter(Reporter&&) = delete;
  Reporter& operator=(Reporter&&) = delete;
  std::string makeSuccessReport();

  static void open();
  static bool hasInstance();
  static Reporter& getInstance();
  static void close();

 private:
  Reporter() = default;
  ~Reporter() = default;
  void collectVersions();

  boost::json::object report;
  inline static Reporter* m_reporter = nullptr;
};

}  // namespace hisui::report
