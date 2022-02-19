cmake_minimum_required(VERSION 3.16)

# Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0054 NEW)
# MSVC runtime library flags are selected by an abraction.
cmake_policy(SET CMP0091 NEW)

include(cmake/env.cmake)

set_cache_string_from_env(BOOST_VERSION 1.78.0 "Boost のバージョン")
set_cache_string_from_env(CLI11_VERSION 2.1.2 "CLI11 のバージョン")
set_cache_string_from_env(CPP_MP4_VERSION 2022.1.0 "cpp-mp4 のバージョン")
set_cache_string_from_env(FMT_VERSION 8.0.1 "fmt のバージョン")
set_cache_string_from_env(OPUS_VERSION 1.3.1 "Opus のバージョン")
set_cache_string_from_env(SPDLOG_VERSION 1.9.2 "spdlog のバージョン")
set_cache_string_from_env(STB_VERSION af1a5bc352164740c1cc1354942b1c6b72eacb8a "stb のバージョン")
set_cache_string_from_env(LIBVPX_VERSION 1.11.0 "libvpx のバージョン")

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
# set(CMAKE_CXX_STANDARD 20)

add_compile_options(
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    )

set(CMAKE_CXX_FLAGS_NATIVE "-O3 -DNDEBUG -march=native -mtune=native")
set(CMAKE_C_FLAGS_NATIVE "-O3 -DNDEBUG -march=native -mtune=native")

set(CMAKE_CXX_FLAGS_PROFILE "-pg -O3 -DNDEBUG -march=native -mtune=native")
set(CMAKE_C_FLAGS_PROFILE "-pg -O3 -DNDEBUG -march=native -mtune=native")

include(cmake/tools.cmake)

CPMAddPackage(
    NAME boost_align
    GITHUB_REPOSITORY boostorg/align
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_assert
    GITHUB_REPOSITORY boostorg/assert
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_config
    GITHUB_REPOSITORY boostorg/config
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_container
    GITHUB_REPOSITORY boostorg/container
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_core
    GITHUB_REPOSITORY boostorg/core
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_exception
    GITHUB_REPOSITORY boostorg/exception
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_integer
    GITHUB_REPOSITORY boostorg/integer
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_intrusive
    GITHUB_REPOSITORY boostorg/intrusive
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_io
    GITHUB_REPOSITORY boostorg/io
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_json
    GITHUB_REPOSITORY boostorg/json
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_move
    GITHUB_REPOSITORY boostorg/move
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_mp11
    GITHUB_REPOSITORY boostorg/mp11
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_rational
    GITHUB_REPOSITORY boostorg/rational
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_smart_ptr
    GITHUB_REPOSITORY boostorg/smart_ptr
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_static_assert
    GITHUB_REPOSITORY boostorg/static_assert
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_system
    GITHUB_REPOSITORY boostorg/system
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_type_traits
    GITHUB_REPOSITORY boostorg/type_traits
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_throw_exception
    GITHUB_REPOSITORY boostorg/throw_exception
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME boost_utility
    GITHUB_REPOSITORY boostorg/utility
    GIT_TAG boost-${BOOST_VERSION}
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME cli11
    URL  https://github.com/CLIUtils/CLI11/archive/v${CLI11_VERSION}.zip
    DOWNLOAD_ONLY YES
    )

CPMAddPackage(
    NAME cpp-mp4
    GITHUB_REPOSITORY kounoike/cpp-mp4
    # GIT_TAG ${CPP_MP4_VERSION}
    GIT_TAG kounoike-patch-1
    OPTIONS "WITH_CLI Off" "WITH_EXAMPLE Off" "WITH_TEST Off"
    )

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG ${FMT_VERSION}
    )

CPMAddPackage(
    NAME opus
    GITHUB_REPOSITORY xiph/opus
    VERSION ${OPUS_VERSION}
    )

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    VERSION ${SPDLOG_VERSION}
    )

CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG ${STB_VERSION}
    DOWNLOAD_ONLY YES
    )

include(ExternalProject)
ExternalProject_Add(
    libvpx
    GIT_REPOSITORY https://chromium.googlesource.com/webm/libvpx.git
    GIT_TAG "v${LIBVPX_VERSION}"
    BUILD_IN_SOURCE FALSE
    GIT_SUBMODULES_RECURSE TRUE
    # PATCH_COMMAND
    #     COMMAND patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libwebm.patch" || true
    CONFIGURE_COMMAND ../libvpx/configure --disable-examples --disable-tools --disable-docs --disable-unit-tests --cpu=native --prefix=${CMAKE_CURRENT_BINARY_DIR}/install
    BUILD_COMMAND make -j$(nproc)
    )
ExternalProject_Get_Property(libvpx SOURCE_DIR BINARY_DIR)
set(libvpx_SOURCE_DIR "${SOURCE_DIR}")
set(libvpx_BINARYE_DIR "${BINARY_DIR}")

message("v${LIBVPX_VERSION}")
message("libvpx_SOURCE_DIR: ${libvpx_SOURCE_DIR} SOURCE_DIR: ${SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
    libyuv
    GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv.git
    GIT_TAG stable
    EXCLUDE_FROM_ALL TRUE
)

FetchContent_Declare(
    libwebm
    GIT_REPOSITORY https://chromium.googlesource.com/webm/libwebm
    GIT_TAG libwebm-1.0.0.28
    # PATCH_COMMAND
    #     COMMAND echo "add_library(libwebm::webm ALIAS webm)" >> CMakeLists.txt
    #     COMMAND echo "export(TARGETS webm mkvparser NAMESPACE libwebm FILE \${PROJECT_BINARY_DIR}/libwebm-targets.cmake)" >> CMakeLists.txt
)
FetchContent_MakeAvailable(libyuv libwebm)

project(hisui C CXX)

add_library(hisuilib)
set_property(TARGET hisuilib PROPERTY OUTPUT_NAME hisui)

add_executable(hisui)

add_dependencies(hisuilib libvpx)

option(USE_FDK_AAC "Use fdk-aac" OFF)

if(USE_FDK_AAC)
    target_compile_definitions(hisui
        PRIVATE
        USE_FDK_AAC
        )

    target_link_libraries(hisui
        PRIVATE
        fdk-aac
        m
        )

    target_sources(hisuilib
        PRIVATE
        src/audio/buffer_fdk_aac_encoder.cpp
        src/audio/fdk_aac.cpp
        src/muxer/fdk_aac_audio_producer.cpp
        )
endif()

target_sources(hisui
    PRIVATE
    src/hisui.cpp
)

target_sources(hisuilib
    PRIVATE
    src/archive_item.cpp
    src/audio/basic_sequencer.cpp
    src/audio/buffer_opus_encoder.cpp
    src/audio/mixer.cpp
    src/audio/opus.cpp
    src/audio/opus_decoder.cpp
    src/audio/webm_source.cpp
    src/config.cpp
    src/datetime.cpp
    src/layout/archive.cpp
    src/layout/cell.cpp
    src/layout/cell_util.cpp
    src/layout/compose.cpp
    src/layout/composer.cpp
    src/layout/grid.cpp
    src/layout/interval.cpp
    src/layout/metadata.cpp
    src/layout/overlap.cpp
    src/layout/region.cpp
    src/layout/source.cpp
    src/layout/video_source.cpp
    src/layout/vpx_video_producer.cpp
    src/metadata.cpp
    src/muxer/async_webm_muxer.cpp
    src/muxer/audio_producer.cpp
    src/muxer/faststart_mp4_muxer.cpp
    src/muxer/mp4_muxer.cpp
    src/muxer/multi_channel_vpx_video_producer.cpp
    src/muxer/muxer.cpp
    src/muxer/no_video_producer.cpp
    src/muxer/opus_audio_producer.cpp
    src/muxer/simple_mp4_muxer.cpp
    src/muxer/video_producer.cpp
    src/muxer/vpx_video_producer.cpp
    src/report/reporter.cpp
    src/util/file.cpp
    src/util/interval.cpp
    src/util/json.cpp
    src/util/wildcard.cpp
    src/version/version.cpp
    src/video/basic_sequencer.cpp
    src/video/buffer_vpx_encoder.cpp
    src/video/composer.cpp
    src/video/decoder.cpp
    src/video/grid_composer.cpp
    src/video/image_source.cpp
    src/video/multi_channel_sequencer.cpp
    src/video/openh264.cpp
    src/video/openh264_decoder.cpp
    src/video/openh264_handler.cpp
    src/video/parallel_grid_composer.cpp
    src/video/preserve_aspect_ratio_scaler.cpp
    src/video/scaler.cpp
    src/video/sequencer.cpp
    src/video/simple_scaler.cpp
    src/video/vpx.cpp
    src/video/vpx_decoder.cpp
    src/video/webm_source.cpp
    src/video/yuv.cpp
    src/webm/input/audio_context.cpp
    src/webm/input/context.cpp
    src/webm/input/video_context.cpp
    src/webm/output/context.cpp
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/cpu_id.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/planar_functions.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_any.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_common.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_gcc.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_msa.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_neon.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_neon64.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/row_win.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_any.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_common.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_gcc.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_msa.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_neon.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_neon64.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/scale_win.cc
    # ${libvpx_SOURCE_DIR}/third_party/libyuv/source/convert.cc
    # ${libvpx_SOURCE_DIR}/third_party/libwebm/mkvparser/mkvparser.cc
    # ${libvpx_SOURCE_DIR}/third_party/libwebm/mkvparser/mkvreader.cc
    # ${libvpx_SOURCE_DIR}/third_party/libwebm/mkvmuxer/mkvmuxer.cc
    # ${libvpx_SOURCE_DIR}/third_party/libwebm/mkvmuxer/mkvwriter.cc
    # ${libvpx_SOURCE_DIR}/third_party/libwebm/mkvmuxer/mkvmuxerutil.cc
    )

set(HEADER_DIRS
    src
    ${boost_align_SOURCE_DIR}/include
    ${boost_assert_SOURCE_DIR}/include
    ${boost_config_SOURCE_DIR}/include
    ${boost_container_SOURCE_DIR}/include
    ${boost_core_SOURCE_DIR}/include
    ${boost_exception_SOURCE_DIR}/include
    ${boost_integer_SOURCE_DIR}/include
    ${boost_intrusive_SOURCE_DIR}/include
    ${boost_io_SOURCE_DIR}/include
    ${boost_json_SOURCE_DIR}/include
    ${boost_move_SOURCE_DIR}/include
    ${boost_mp11_SOURCE_DIR}/include
    ${boost_rational_SOURCE_DIR}/include
    ${boost_smart_ptr_SOURCE_DIR}/include
    ${boost_static_assert_SOURCE_DIR}/include
    ${boost_system_SOURCE_DIR}/include
    ${boost_throw_exception_SOURCE_DIR}/include
    ${boost_type_traits_SOURCE_DIR}/include
    ${boost_utility_SOURCE_DIR}/include
    ${cli11_SOURCE_DIR}/include
    ${cpp-mp4_SOURCE_DIR}/include
    ${fmt_SOURCE_DIR}/include
    ${opus_SOURCE_DIR}/include
    ${spdlog_SOURCE_DIR}/include
    ${stb_SOURCE_DIR}
    third_party/openh264
    third_party/progress-cpp/include
    ${PROJECT_BINARY_DIR}/libvpx-prefix/src/libvpx/third_party/libyuv/include
    ${PROJECT_BINARY_DIR}/libvpx-prefix/src/libvpx/third_party/libwebm
    ${PROJECT_BINARY_DIR}/libvpx-prefix/src/libvpx
)

target_include_directories(hisuilib
    PRIVATE
    ${HEADER_DIRS}
    )

target_include_directories(hisui
    PRIVATE
    ${HEADER_DIRS}
    )

set_target_properties(hisui PROPERTIES CXX_STANDARD 20 C_STANDARD 11)
set_target_properties(hisuilib PROPERTIES CXX_STANDARD 20 C_STANDARD 11)

target_link_libraries(hisuilib
    PRIVATE
    dl
    opus
    pthread
    fmt
    shiguredo-mp4
    spdlog
    ${libyuv_BINARY_DIR}/libyuv.a
    ${libwebm_BINARY_DIR}/libwebm.a
    ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libvpx.a
    )


target_link_libraries(hisui
    PRIVATE
    hisuilib
    )

if(WITH_TEST)
    enable_testing()

    CPMAddPackage(
        NAME boost_algorithm
        GITHUB_REPOSITORY boostorg/algorithm
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_bind
        GITHUB_REPOSITORY boostorg/bind
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_container_hash
        GITHUB_REPOSITORY boostorg/container_hash
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_detail
        GITHUB_REPOSITORY boostorg/detail
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_exception
        GITHUB_REPOSITORY boostorg/exception
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_function
        GITHUB_REPOSITORY boostorg/function
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_io
        GITHUB_REPOSITORY boostorg/io
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_iterator
        GITHUB_REPOSITORY boostorg/iterator
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_move
        GITHUB_REPOSITORY boostorg/move
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_mpl
        GITHUB_REPOSITORY boostorg/mpl
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_numeric_conversion
        GITHUB_REPOSITORY boostorg/numeric_conversion
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_preprocessor
        GITHUB_REPOSITORY boostorg/preprocessor
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_range
        GITHUB_REPOSITORY boostorg/range
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_smart_ptr
        GITHUB_REPOSITORY boostorg/smart_ptr
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_test
        GITHUB_REPOSITORY boostorg/test
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    CPMAddPackage(
        NAME boost_type_index
        GITHUB_REPOSITORY boostorg/type_index
        GIT_TAG boost-${BOOST_VERSION}
        DOWNLOAD_ONLY YES
        )

    add_subdirectory(test)
endif()
