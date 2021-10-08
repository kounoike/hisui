#include <boost/test/unit_test.hpp>

#include "layout/overlap.hpp"
#include "layout/source.hpp"

BOOST_AUTO_TEST_SUITE(overlap)

BOOST_AUTO_TEST_CASE(overlap) {
  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 1,
            .max_end_time = 1,
            .trim_intervals = {},
        };

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 1}),
                                },
                            .reuse = hisui::layout::Reuse::None,
                        }));
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 1}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 1}),
                                },
                            .reuse = hisui::layout::Reuse::ShowNewest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 2,
            .max_end_time = 2,
            .trim_intervals = {},
        };
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 1, .end_time = 2}),
                                },
                            .reuse = hisui::layout::Reuse::None,
                        }));

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 1, .end_time = 2}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 1, .end_time = 2}),
                                },
                            .reuse = hisui::layout::Reuse::ShowNewest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 2,
            .max_end_time = 3,
            .trim_intervals = {},
        };

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 3}),
                                },
                            .reuse = hisui::layout::Reuse::None,
                        }));
  }
  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 1,
            .max_end_time = 3,
            .trim_intervals = {},
        };

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 3}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 3}),
                                },
                            .reuse = hisui::layout::Reuse::ShowNewest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 2,
            .max_end_time = 6,
            .trim_intervals = {},
        };

    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 4}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 3, .end_time = 6}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 3,
            .max_end_time = 8,
            .trim_intervals = {},
        };
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 0, .end_time = 8}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 5}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 5, .end_time = 6}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 3, .end_time = 7}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 1,
            .max_end_time = 7,
            .trim_intervals = {{0, 1}, {4, 5}},
        };
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 1, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 2, .end_time = 4}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 5, .end_time = 6}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 6, .end_time = 7}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));
  }

  {
    auto expected =
        hisui::layout::MaxNumberOfOverlapAndMaxEndTimeAndTrimIntervals{
            .max_number_of_overlap = 2,
            .max_end_time = 7,
            .trim_intervals = {{0, 1}, {2, 3}, {4, 5}},
        };
    BOOST_REQUIRE_EQUAL(expected,
                        hisui::layout::overlap({
                            .sources =
                                {
                                    hisui::layout::SourceInterval(
                                        {.start_time = 1, .end_time = 2}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 3, .end_time = 4}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 5, .end_time = 7}),
                                    hisui::layout::SourceInterval(
                                        {.start_time = 6, .end_time = 7}),
                                },
                            .reuse = hisui::layout::Reuse::ShowOldest,
                        }));
  }
}

BOOST_AUTO_TEST_CASE(overlap_trim_intervals) {
  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {}}));
  }
  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{0, 100}, {200, 300}},
    };
    BOOST_REQUIRE_EQUAL(
        expected, hisui::layout::overlap_trim_intervals(
                      {.list_of_trim_intervals = {{{0, 100}, {200, 300}}}}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{0, 100}, {200, 300}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 300}},
                                           {{0, 200}, {200, 400}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{0, 100}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 300}},
                                           {{0, 200}},
                                           {{0, 400}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 300}},
                                           {{0, 200}},
                                           {},
                                           {{0, 400}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{0, 100}, {250, 300}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 300}},
                                           {{0, 200}, {250, 300}},
                                           {{0, 400}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{200, 300}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 350}},
                                           {{200, 300}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{200, 300}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{200, 300}},
                                           {{0, 100}, {200, 350}},
                                       }}));
  }

  {
    auto expected = hisui::layout::TrimIntervals{
        .trim_intervals = {{250, 300}},
    };
    BOOST_REQUIRE_EQUAL(expected, hisui::layout::overlap_trim_intervals(
                                      {.list_of_trim_intervals = {
                                           {{0, 100}, {200, 350}},
                                           {{200, 300}},
                                           {{250, 400}},
                                       }}));
  }
}

BOOST_AUTO_TEST_SUITE_END()
