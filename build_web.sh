#!/bin/sh
emcc -o web/game.html src/entry.c \
	src/web_flags.c \
	-Os -Wall ./raylib/lib/web/libraylib.a \
	-I. -Iraylib/include \
	-L. \
	-L./raylib/lib/web/libraylib.a \
	-Iraylib/include/raymath.h \
	-Iraylib/include/rlgl.h \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	-Wall \
	--shell-file ./web/minshell.html \
	-DPLATFORM_WEB \
	--preload-file assets
	
