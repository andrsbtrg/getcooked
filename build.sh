#!/bin/sh
bear --output compile_commands.json -- \
	gcc -Wextra -Wpedantic -Warray-bounds -o game src/entry.c raylib/lib/libraylib.a -Iraylib/include -O0 -g3 -lm -Wall
