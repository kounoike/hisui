#include <boost/test/unit_test.hpp>

#include "audio/mixer.hpp"

BOOST_AUTO_TEST_SUITE(mix)

BOOST_AUTO_TEST_CASE(mix_sample_simple) {
  BOOST_REQUIRE_EQUAL(3, hisui::audio::mix_sample_simple(1, 2));
  BOOST_REQUIRE_EQUAL(1, hisui::audio::mix_sample_simple(-1, 2));
  BOOST_REQUIRE_EQUAL(32767, hisui::audio::mix_sample_simple(32767, 1));
  BOOST_REQUIRE_EQUAL(-32768, hisui::audio::mix_sample_simple(-32767, -1));
  BOOST_REQUIRE_EQUAL(-32768, hisui::audio::mix_sample_simple(-32767, -2));
}

BOOST_AUTO_TEST_CASE(mix_sample_toth) {
  BOOST_REQUIRE_EQUAL(3, hisui::audio::mix_sample_vttoth(1, 2));
  BOOST_REQUIRE_EQUAL(0, hisui::audio::mix_sample_vttoth(-1, 2));
  BOOST_REQUIRE_EQUAL(32767, hisui::audio::mix_sample_vttoth(32767, 1));
  BOOST_REQUIRE_EQUAL(-32768, hisui::audio::mix_sample_vttoth(-32767, -1));
  BOOST_REQUIRE_EQUAL(-32768, hisui::audio::mix_sample_vttoth(-32767, -2));
}

BOOST_AUTO_TEST_SUITE_END()
