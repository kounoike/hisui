#!/usr/bin/env bash

set -ex

shopt -s globstar

targets=(src/**/*.[ch]pp test/**/*.[ch]pp)

cppcheck --language=c++ "${targets[@]}" || exit 1
cpplint --linelength=120 --filter=-build/include_subdir,-legal/copyright,-build/c++11 "${targets[@]}" || exit 1
# clang-tidy -p debug/compile_commands.json "${targets[@]}"
misspell -error "${targets[@]}"
shellcheck ./**/*.bash
