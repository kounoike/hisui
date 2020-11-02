#!/usr/bin/env bash

targets=(src/*.[ch]pp src/audio/*.[ch]pp src/muxer/*.[ch]pp src/util/*.[ch]pp src/video/*.[ch]pp src/webm/input/*.[ch]pp src/webm/output/*.[ch]pp test/*/*.cpp)

cppcheck --language=c++ "${targets[@]}" || exit 1
cpplint --linelength=120 --filter=-build/include_subdir,-legal/copyright,-build/c++11 "${targets[@]}" || exit 1
# clang-tidy -p debug/compile_commands.json "${targets[@]}"
misspell -error "${targets[@]}"
