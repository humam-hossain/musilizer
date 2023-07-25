#!/bin/sh

set -xe
g++ -Wall -Wextra -o musilizer main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11