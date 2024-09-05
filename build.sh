#!/bin/sh
bear --output compile_commands.json -- \
	g++ src/entry.c raylib/lib/libraylib.a -Iraylib/include -O0 -g3 -o game -Wmissing-field-initializers
