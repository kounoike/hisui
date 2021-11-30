#include <cstdint>
#include <limits>
#include <memory>

#include <boost/test/unit_test.hpp>

#include "layout/cell.hpp"
#include "layout/region.hpp"
#include "layout/source.hpp"

BOOST_AUTO_TEST_SUITE(region)

BOOST_AUTO_TEST_CASE(set_video_source_to_cells) {
  {
    auto video_source0 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 0,
                                        .connection_id = "connection0",
                                        .start_time = 0,
                                        .end_time = 10,
                                        .testing = true});
    auto cells = std::vector<std::shared_ptr<hisui::layout::Cell>>{};
    cells.push_back(
        std::make_shared<hisui::layout::Cell>(hisui::layout::CellParameters{
            .index = 0,
            .pos = {.x = 0, .y = 0},
            .resolution = {.width = 240, .height = 160},
        }));
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source0,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());

    auto video_source1 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 1,
                                        .connection_id = "connection1",
                                        .start_time = 10,
                                        .end_time = 20,
                                        .testing = true});

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source1,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source1,
         .reuse = hisui::layout::Reuse::ShowOldest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source1,
         .reuse = hisui::layout::Reuse::ShowNewest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(20, cells[0]->getEndTime());

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source0,
         .reuse = hisui::layout::Reuse::ShowNewest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(20, cells[0]->getEndTime());

    hisui::layout::reset_cells_source({.cells = cells, .time = 20});

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source0,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(std::numeric_limits<std::uint64_t>::max(),
                        cells[0]->getEndTime());

    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source0,
         .reuse = hisui::layout::Reuse::ShowOldest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());
  }
  {
    auto cells = std::vector<std::shared_ptr<hisui::layout::Cell>>{};
    cells.push_back(
        std::make_shared<hisui::layout::Cell>(hisui::layout::CellParameters{
            .index = 0,
            .pos = {.x = 1, .y = 1},
            .resolution = {.width = 240, .height = 160},
        }));
    cells.push_back(
        std::make_shared<hisui::layout::Cell>(hisui::layout::CellParameters{
            .index = 1,
            .pos = {.x = 1, .y = 1},
            .resolution = {.width = 240, .height = 160},
        }));
    cells.push_back(
        std::make_shared<hisui::layout::Cell>(hisui::layout::CellParameters{
            .index = 1,
            .pos = {.x = 1, .y = 1},
            .resolution = {.width = 240, .height = 160},
            .status = hisui::layout::CellStatus::Excluded,
        }));
    cells.push_back(
        std::make_shared<hisui::layout::Cell>(hisui::layout::CellParameters{
            .index = 2,
            .pos = {.x = 1, .y = 1},
            .resolution = {.width = 240, .height = 160},
        }));

    auto video_source0 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 0,
                                        .connection_id = "connection0",
                                        .start_time = 0,
                                        .end_time = 10,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source0,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());

    auto video_source1 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 1,
                                        .connection_id = "connection1",
                                        .start_time = 0,
                                        .end_time = 20,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source1,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());
    BOOST_REQUIRE_EQUAL(20, cells[1]->getEndTime());

    auto video_source2 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 2,
                                        .connection_id = "connection2",
                                        .start_time = 10,
                                        .end_time = 15,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source2,
         .reuse = hisui::layout::Reuse::None,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(10, cells[0]->getEndTime());
    BOOST_REQUIRE_EQUAL(20, cells[1]->getEndTime());
    BOOST_REQUIRE_EQUAL(15, cells[3]->getEndTime());

    auto video_source3 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 3,
                                        .connection_id = "connection3",
                                        .start_time = 10,
                                        .end_time = 25,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source3,
         .reuse = hisui::layout::Reuse::ShowNewest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(25, cells[0]->getEndTime());
    BOOST_REQUIRE_EQUAL(20, cells[1]->getEndTime());
    BOOST_REQUIRE_EQUAL(15, cells[3]->getEndTime());

    auto video_source4 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 4,
                                        .connection_id = "connection4",
                                        .start_time = 10,
                                        .end_time = 30,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source4,
         .reuse = hisui::layout::Reuse::ShowNewest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(25, cells[0]->getEndTime());
    BOOST_REQUIRE_EQUAL(30, cells[1]->getEndTime());
    BOOST_REQUIRE_EQUAL(15, cells[3]->getEndTime());

    auto video_source5 = std::make_shared<hisui::layout::VideoSource>(
        hisui::layout::SourceParameters{.file_path = "",
                                        .index = 5,
                                        .connection_id = "connection5",
                                        .start_time = 11,
                                        .end_time = 40,
                                        .testing = true});
    hisui::layout::set_video_source_to_cells(
        {.video_source = video_source5,
         .reuse = hisui::layout::Reuse::ShowNewest,
         .cells = cells});
    BOOST_REQUIRE_EQUAL(25, cells[0]->getEndTime());
    BOOST_REQUIRE_EQUAL(30, cells[1]->getEndTime());
    BOOST_REQUIRE_EQUAL(40, cells[3]->getEndTime());
  }
}

BOOST_AUTO_TEST_SUITE_END()
