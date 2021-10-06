#include <boost/test/unit_test.hpp>

#include "layout/overlap.hpp"
#include "layout/region.hpp"

BOOST_AUTO_TEST_SUITE(overlap)

BOOST_AUTO_TEST_CASE(maximum_number_of_overlap) {
  BOOST_REQUIRE_EQUAL(1, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 1},
                                 },
                             .reuse = hisui::layout::Reuse::None,
                         }));

  BOOST_REQUIRE_EQUAL(1, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 1},
                                 },
                             .reuse = hisui::layout::Reuse::ShowOldest,
                         }));

  BOOST_REQUIRE_EQUAL(1, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 1},
                                 },
                             .reuse = hisui::layout::Reuse::ShowNewest,
                         }));

  BOOST_REQUIRE_EQUAL(2, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 1, .end_time = 2},
                                 },
                             .reuse = hisui::layout::Reuse::None,
                         }));

  BOOST_REQUIRE_EQUAL(2, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 1, .end_time = 2},
                                 },
                             .reuse = hisui::layout::Reuse::ShowOldest,
                         }));

  BOOST_REQUIRE_EQUAL(2, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 1, .end_time = 2},
                                 },
                             .reuse = hisui::layout::Reuse::ShowNewest,
                         }));

  BOOST_REQUIRE_EQUAL(2, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 2, .end_time = 3},
                                 },
                             .reuse = hisui::layout::Reuse::None,
                         }));

  BOOST_REQUIRE_EQUAL(1, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 2, .end_time = 3},
                                 },
                             .reuse = hisui::layout::Reuse::ShowOldest,
                         }));
  BOOST_REQUIRE_EQUAL(1, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 2, .end_time = 3},
                                 },
                             .reuse = hisui::layout::Reuse::ShowNewest,
                         }));

  BOOST_REQUIRE_EQUAL(2, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 2},
                                     {.start_time = 2, .end_time = 4},
                                     {.start_time = 3, .end_time = 6},
                                 },
                             .reuse = hisui::layout::Reuse::ShowOldest,
                         }));

  BOOST_REQUIRE_EQUAL(3, hisui::layout::maximum_number_of_overlap({
                             .sources =
                                 {
                                     {.start_time = 0, .end_time = 8},
                                     {.start_time = 2, .end_time = 5},
                                     {.start_time = 5, .end_time = 6},
                                     {.start_time = 3, .end_time = 7},
                                 },
                             .reuse = hisui::layout::Reuse::ShowOldest,
                         }));
}

BOOST_AUTO_TEST_SUITE_END()
