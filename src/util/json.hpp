#pragma once

#include <string>

#include <boost/json/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/string.hpp>
#include <boost/json/value.hpp>

namespace hisui::util {

boost::json::string get_string_from_json_object(boost::json::object o,
                                                const std::string& key);
double get_double_from_json_object(boost::json::object o,
                                   const std::string& key);
}  // namespace hisui::util
