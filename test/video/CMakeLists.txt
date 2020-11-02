cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_compile_options(
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wunused
    -Wold-style-cast
    -Wcast-align
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -pedantic)

add_executable(video_test
    main.cpp
    vpx_test.cpp
    yuv_test.cpp
    ../../src/video/yuv.cpp
    ../../src/video/vpx.cpp
    )

set_target_properties(video_test PROPERTIES CXX_STANDARD 20 C_STANDARD 99)

target_compile_options(video_test
    PRIVATE
    -DBOOST_OVERRIDE=
    -DBOOST_NOEXCEPT_OR_NOTHROW=
    )

target_include_directories(video_test
    PRIVATE
    ${boost_test_SOURCE_DIR}/include
    ${fmt_SOURCE_DIR}/include
    ${spdlog_SOURCE_DIR}/include
    ../../src
    ../../src/third_party/openh264/codec/api/svc
    ../../third_party/libvpx/third_party/libwebm
    ../../third_party/libvpx/third_party/libyuv/include
    )

target_link_libraries(video_test
    PRIVATE
    vpx
    fmt
    )

add_test(NAME video COMMAND video_test)
