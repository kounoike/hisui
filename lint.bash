#!/usr/bin/env bash

set -ex

shopt -s globstar

targets=(src/**/*.[ch]pp test/**/*.[ch]pp)

cpplint --linelength=120 --filter=-build/include_subdir,-legal/copyright,-build/c++11 "${targets[@]}" || exit 1
misspell -error "${targets[@]}" || exit 1
shellcheck ./**/*.bash || exit 1
