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

#define MAIN_SCREEN             0
#define START_ENCOUNTER_SCREEN  1
#define BUILD_ENCOUNTER_SCREEN  2
#define CREATURE_DB_SCREEN      3
#define PLAYER_DB_SCREEN        4
#define ADD_STAT_SCREEN         5

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

extern Clay_String StatName;
extern Clay_String StatSize;
extern Clay_String StatType;
extern Clay_String StatAC;
extern Clay_String StatHP;
extern Clay_String StatSpeed;
extern Clay_String StatStr;
extern Clay_String StatDex;
extern Clay_String StatCon;
extern Clay_String StatInt;
extern Clay_String StatWis;
extern Clay_String StatCha;

extern Clay_String StatSaves;
extern Clay_String StatSkills;
extern Clay_String StatSenses;
extern Clay_String StatLanguages;
extern Clay_String StatCR;

extern Clay_String StatRacial1;
extern Clay_String StatRacial2;
extern Clay_String StatRacial3;

extern Clay_String StatAction1;
extern Clay_String StatAction2;
extern Clay_String StatAction3;
extern Clay_String StatAction4;

#endif /* GLOBAL_H */