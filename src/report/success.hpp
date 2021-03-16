#pragma once

#include <string>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

class SuccessReporter {
 public:
  SuccessReporter() = default;
  std::string make();

 private:
  void getVersions();

  boost::json::object report;
};

}  // namespace hisui::report
