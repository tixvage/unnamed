#!/bin/sh

set -xe

CFLAGS+="-std=c11 -O0 -ggdb "
CFLAGS+="-lraylib -lm "
CFLAGS+="-Wall -Werror "

gcc main.c -o main.out $CFLAGS
