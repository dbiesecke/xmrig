#!/bin/sh
set -vx
SCRIPT=$(readlink -f "$0")
DIR=$(dirname "$SCRIPT")

apk --no-cache upgrade 
apk --no-cache add git cmake libuv-dev build-base libmicrohttpd-dev
source build.sh

