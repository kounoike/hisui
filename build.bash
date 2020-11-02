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
if [ "$type" = "native" ]; then
    CXX=clang++ CC=clang ./configure --cpu=native --disable-examples --disable-tools --disable-docs --disable-unit-tests
else
    CXX=clang++ CC=clang ./configure --disable-examples --disable-tools --disable-docs --disable-unit-tests
fi
make

cd ../..
if [ "$type" = "native" ]; then
    rm -rf native
    mkdir native
    cd native || exit 1
    cmake -DWITHOUT_TEST=On -DCMAKE_BUILD_TYPE=Native ..
else
    rm -rf release
    mkdir release
    cd release || exit 1
    cmake  -DWITHOUT_TEST=On -DCMAKE_BUILD_TYPE=Release ..
fi
cmake --build .

tar cvf hisui-${HISUI_VERSION}_ubuntu-20.04_x86_64.tar.gz hisui -C .. LICENSE NOTICE.md
