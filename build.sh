#!/bin/sh
set -vx
SCRIPT=$(readlink -f "$0")
DIR=$(dirname "$SCRIPT")
BUILD_DIR=$DIR/build
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake .. -DWITH_LIBCPUID=OFF -DWITH_AEON=OFF -DWITH_HTTPD=OFF
make

