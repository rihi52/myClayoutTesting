#ifndef GLOBAL_H
#define GLOBAL_H

#include "clay.h"
#include "stdio.h"
#include "stdint.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_events.h>

#define MAIN_SCREEN 0
#define START_ENCOUNTER_SCREEN  1
#define BUILD_ENCOUNTER_SCREEN  2
#define CREATURE_DB_SCREEN      3
#define PLAYER_DB_SCREEN        4

#define MAX_TEXT 256
extern char TextBuffer[MAX_TEXT];

//char textBuffer[MAX_TEXT] = {0};

extern int WindowState;

extern int WindowWidth;
extern int WindowHeight;
extern float ScrollOffset;
extern uint16_t TotalCreatures;

extern const int FONT_ID_BODY_16;
extern const int FONT_ID_BODY_32;

typedef struct {
    SDL_Renderer *renderer;
    TTF_TextEngine *textEngine;
    TTF_Font **fonts;
} Clay_SDL3RendererData;

typedef struct app_state {
    SDL_Window *window;
    Clay_SDL3RendererData rendererData;
} AppState;

#endif /* GLOBAL_H */