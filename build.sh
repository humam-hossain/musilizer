#!/bin/sh

set -xe
CFLAGS="-Wall -Wextra"
LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

mkdir -p ./build/
g++ $CFLAGS -o ./build/musilizer ./src/musilizer.cpp $LIBS