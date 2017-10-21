#!/bin/sh
set -vx
rm -rf ./build
mkdir -p ./build
cd build
cmake .. -DWITH_LIBCPUID=OFF -DWITH_AEON=OFF -DWITH_HTTPD=OFF
make

