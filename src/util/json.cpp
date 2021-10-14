#include "util/json.hpp"

#include <fmt/core.h>

#include <stdexcept>
#include <string>

#include <boost/json/array.hpp>
#include <boost/json/impl/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>

namespace hisui::util {

boost::json::string get_string_from_json_object(boost::json::object o,
                                                const std::string& key) {
  if (auto p = o[key].if_string()) {
    return *p;
  }
  throw std::runtime_error(fmt::format("o[{}].if_string() failed", key));
}

double get_double_from_json_object(boost::json::object o,
                                   const std::string& key) {
  if (o[key].is_number()) {
    boost::json::error_code ec;
    auto value = o[key].to_number<double>(ec);
    if (ec) {
      throw std::runtime_error(
          fmt::format("o[{}].to_number() failed: {}", key, ec.message()));
    }
    return value;
  }
  throw std::runtime_error(fmt::format("o[{}] is not number", key));
}

}  // namespace hisui::util
