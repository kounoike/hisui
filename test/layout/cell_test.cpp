#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "layout/cell.hpp"

BOOST_AUTO_TEST_SUITE(cell)

BOOST_AUTO_TEST_CASE(calc_cell_length_and_positions) {
  BOOST_REQUIRE_THROW(
      hisui::layout::calc_cell_length_and_positions({.number_of_cells = 0}),
      std::invalid_argument);
  {
    auto expected =
        hisui::layout::LengthAndPositions{.length = 240, .positions = {1}};
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::calc_cell_length_and_positions(
                                      {.number_of_cells = 1,
                                       .region_length = 242,
                                       .min_frame_length = 1}));
  }
  {
    auto expected =
        hisui::layout::LengthAndPositions{.length = 236, .positions = {1}};
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::calc_cell_length_and_positions(
                                      {.number_of_cells = 1,
                                       .region_length = 240,
                                       .min_frame_length = 1}));
  }
  {
    auto expected =
        hisui::layout::LengthAndPositions{.length = 240, .positions = {1, 242}};
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::calc_cell_length_and_positions(
                                      {.number_of_cells = 2,
                                       .region_length = 483,
                                       .min_frame_length = 1}));
  }
  {
    auto expected = hisui::layout::LengthAndPositions{
        .length = 156, .positions = {1, 158, 315}};
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::calc_cell_length_and_positions(
                                      {.number_of_cells = 3,
                                       .region_length = 480,
                                       .min_frame_length = 1}));
  }
  {
    auto expected = hisui::layout::LengthAndPositions{
        .length = 156, .positions = {2, 160, 318}};
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::calc_cell_length_and_positions(
                                      {.number_of_cells = 3,
                                       .region_length = 480,
                                       .min_frame_length = 2}));
  }
}

BOOST_AUTO_TEST_SUITE_END()
