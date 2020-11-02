#include <boost/test/unit_test.hpp>

#include "util/interval.hpp"

BOOST_AUTO_TEST_SUITE(interval)

BOOST_AUTO_TEST_CASE(mix_sample_simple) {
  hisui::util::Interval i(10, 15);

  BOOST_REQUIRE(!i.isIn(0));
  BOOST_REQUIRE(!i.isIn(9));
  BOOST_REQUIRE(i.isIn(10));
  BOOST_REQUIRE(i.isIn(14));
  BOOST_REQUIRE(!i.isIn(15));

  BOOST_REQUIRE_EQUAL(0, i.getSubstructLower(10));
  BOOST_REQUIRE_EQUAL(4, i.getSubstructLower(14));
}

BOOST_AUTO_TEST_SUITE_END()
