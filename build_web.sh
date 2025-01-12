#!/bin/sh
emcc -o web/game.html src/entry.c \
	-Os -Wall ./raylib/lib/web/libraylib.a \
	-I. -Iraylib/include \
	-L. \
	-L./raylib/lib/web/libraylib.a \
	-Iraylib/include/raymath.h \
	-Iraylib/include/rlgl.h \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	--shell-file $HOME/src/raylib/src/minshell.html \
	-DPLATFORM_WEB \
	--preload-file assets
	
