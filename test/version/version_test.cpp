#include <boost/test/unit_test.hpp>

#include "version/version.hpp"

BOOST_AUTO_TEST_SUITE(version)

BOOST_AUTO_TEST_CASE(get_hisui_version) {
  BOOST_REQUIRE_EQUAL("2021.3", hisui::version::get_hisui_version());
}

BOOST_AUTO_TEST_SUITE_END()
