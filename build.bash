#!/usr/bin/env bash

set -ex

cd "$(dirname "$0")" || exit 1

source ./VERSION

type=$1

[ -d third_party ] || mkdir third_party
cd third_party || exit 1

[ -d libvpx ] || git clone https://chromium.googlesource.com/webm/libvpx
cd libvpx || exit 1
git checkout v"${LIBVPX_VERSION}"
if [ "$type" = "docker" ]; then
    CXX=clang++ CC=clang ./configure --disable-examples --disable-tools --disable-docs --disable-unit-tests # Docker 向けには --cpu を付けない
else
    CXX=clang++ CC=clang ./configure --cpu=native --disable-examples --disable-tools --disable-docs --disable-unit-tests
fi
make

cd ../..
if [ "$type" = "docker" ]; then
    [ -d release ] || mkdir release
    cd release || exit 1
    cmake -DCMAKE_BUILD_TYPE=Release ..
else
    [ -d native ] || mkdir native
    cd native || exit 1
    cmake -DCMAKE_BUILD_TYPE=Native ..
fi
cmake --build .

# TODO: docker build
