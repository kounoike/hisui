#include <boost/test/unit_test.hpp>

#include "layout/source.hpp"

BOOST_AUTO_TEST_SUITE(source)

BOOST_AUTO_TEST_CASE(minimum_video_start_time) {
  BOOST_REQUIRE_EQUAL(
      0,
      hisui::layout::minimum_video_start_time({
          .sources =
              {
                  hisui::layout::VideoSource({.start_time = 0, .end_time = 1}),
              },
      }));

  BOOST_REQUIRE_EQUAL(
      1,
      hisui::layout::minimum_video_start_time({
          .sources =
              {
                  hisui::layout::VideoSource({.start_time = 2, .end_time = 2}),
                  hisui::layout::VideoSource({.start_time = 1, .end_time = 2}),
                  hisui::layout::VideoSource({.start_time = 3, .end_time = 2}),
              },
      }));

  BOOST_REQUIRE_EQUAL(
      5,
      hisui::layout::minimum_video_start_time({
          .sources =
              {
                  hisui::layout::VideoSource({.start_time = 10, .end_time = 2}),
                  hisui::layout::VideoSource({.start_time = 12, .end_time = 2}),
                  hisui::layout::VideoSource({.start_time = 5, .end_time = 2}),
              },
      }));
}

BOOST_AUTO_TEST_CASE(minimum_audio_start_time) {
  BOOST_REQUIRE_EQUAL(
      0,
      hisui::layout::minimum_audio_start_time({
          .sources =
              {
                  hisui::layout::AudioSource({.start_time = 0, .end_time = 1}),
              },
      }));

  BOOST_REQUIRE_EQUAL(
      1,
      hisui::layout::minimum_audio_start_time({
          .sources =
              {
                  hisui::layout::AudioSource({.start_time = 2, .end_time = 2}),
                  hisui::layout::AudioSource({.start_time = 1, .end_time = 2}),
                  hisui::layout::AudioSource({.start_time = 3, .end_time = 2}),
              },
      }));

  BOOST_REQUIRE_EQUAL(
      5,
      hisui::layout::minimum_audio_start_time({
          .sources =
              {
                  hisui::layout::AudioSource({.start_time = 10, .end_time = 2}),
                  hisui::layout::AudioSource({.start_time = 12, .end_time = 2}),
                  hisui::layout::AudioSource({.start_time = 5, .end_time = 2}),
              },
      }));
}

BOOST_AUTO_TEST_SUITE_END()
