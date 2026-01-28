#ifndef RENDERER_TYPES_H
#define RENDERER_TYPES_H

#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
    SDL_Renderer *renderer;
    TTF_TextEngine *textEngine;
    TTF_Font **fonts;
} Clay_SDL3RendererData;


#endif