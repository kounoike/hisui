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

add_executable(util_test
    main.cpp
    interval_test.cpp
    ../../src/util/interval.cpp
    )

set_target_properties(util_test PROPERTIES CXX_STANDARD 20 C_STANDARD 99)

target_compile_options(util_test
    PRIVATE
    -DBOOST_OVERRIDE=
    -DBOOST_NOEXCEPT_OR_NOTHROW=
    )

target_include_directories(util_test
    PRIVATE
    ../../src
    ${boost_test_SOURCE_DIR}/include
    )

add_test(NAME util COMMAND util_test)
