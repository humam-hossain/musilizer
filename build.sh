#!/bin/sh

set -xe
CFLAGS="-Wall -Wextra"
LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

mkdir -p ./bin/
g++ $CFLAGS -fPIC -shared -o ./libplug.so ./src/plug.cpp $LIBS

g++ $CFLAGS -o ./bin/musilizer ./src/musilizer.cpp -Isrc -L. $LIBS -lplug