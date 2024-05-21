C_SOURCE_FILES += $(wildcard ./src/*.c)
C_SOURCE_FILES += $(wildcard ./adaptor/sdlv2/*.c)

C_INCLUDES += ./src ./include ./adaptor/sdlv2

## `sdl2-config --cflags`
C_FLAGS += -IC:/lib/SDL2-2.30.2/x86_64-w64-mingw32/include/SDL2
C_FLAGS += -IC:/lib/SDL2_image-2.8.2/x86_64-w64-mingw32/include/SDL2
C_FLAGS += -Dmain=SDL_main

## `sdl2-config --libs`
LD_FLAGS += -LC:/lib/SDL2-2.30.2/x86_64-w64-mingw32/lib
LD_FLAGS += -LC:/lib/SDL2_image-2.8.2/x86_64-w64-mingw32/lib
LD_FLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

include ./cc-with-test.mk

