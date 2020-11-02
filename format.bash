#!/usr/bin/env bash

# src/ test/ 以下のファイルに clang-format を適用する

set -e

shopt -s globstar

for file in src/**/*.[ch]pp test/**/*.[ch]pp; do
    if [[ "$file" =~ "/third_party/" ]]; then
        continue
    fi
  echo applying "$file"
  clang-format -i -style=file "$file"
done
