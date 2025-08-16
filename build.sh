#!/bin/sh

set -xe

CFLAGS+="-std=c11 -O0 -ggdb "
CFLAGS+="-lraylib -lm "
CFLAGS+="-Wall -Wextra "

gcc main.c -o main.out $CFLAGS
