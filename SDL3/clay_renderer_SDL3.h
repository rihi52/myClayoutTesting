#ifndef CLAY_RENDERER_SDL3_H
#define CLAY_RENDERER_SDL3_H

#include <SDL3/SDL.h>
#include "renderincludes.h"
#include "../src/clay.h"

// Add only the functions you call from main.c
void SDL_Clay_RenderClayCommands(Clay_SDL3RendererData *rendererData, Clay_RenderCommandArray *rcommands);

#endif // CLAY_RENDERER_SDL3_H