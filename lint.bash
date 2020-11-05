#!/usr/bin/env bash

set -ex

shopt -s globstar

targets=(src/**/*.[ch]pp test/**/*.[ch]pp)

cppcheck --language=c++ "${targets[@]}" || exit 1
#cpplint --linelength=120 --filter=-legal/copyright,-build/c++11 "${targets[@]}" || exit 1
cpplint --linelength=120 --filter=-legal/copyright "${targets[@]}" || exit 1
misspell -error "${targets[@]}" || exit 1
shellcheck ./**/*.bash || exit 1
