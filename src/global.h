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

#define BACKSPACE_KEY           8

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

extern Clay_String TypedText;

extern Clay_String StatId;
extern Clay_String StatName;
extern Clay_String StatCr;
extern Clay_String StatType;
extern Clay_String StatSize;

extern Clay_String StatArmorClass;
extern Clay_String StatHitpointsAvg;
extern Clay_String StatHitDice;
extern Clay_String StatHitpointsRoll;

extern Clay_String StatSpeedType;
extern Clay_String StatSpeedWalk;
extern Clay_String StatSpeedFly;
extern Clay_String StatSpeedSwim;
extern Clay_String StatSpeedClimb;
extern Clay_String StatSpeedBurrow;

extern Clay_String StatAlignment;
extern Clay_String StatLegendary;

extern Clay_String StatStr;
extern Clay_String StatDex;
extern Clay_String StatCon;
extern Clay_String StatInt;
extern Clay_String StatWis;
extern Clay_String StatCha;

extern Clay_String StatProfBonus;

extern Clay_String StatThrowStr;
extern Clay_String StatThrowDex;
extern Clay_String StatThrowCon;
extern Clay_String StatThrowInt;
extern Clay_String StatThrowWis;
extern Clay_String StatThrowCha;

extern Clay_String StatSavingThrows;

extern Clay_String StatSkills;
extern Clay_String StatLanguages;
extern Clay_String StatSenses;

extern Clay_String StatRangeDarkvision;
extern Clay_String StatRangeTremorsense;
extern Clay_String StatRangeBlindsight;
extern Clay_String StatRangeTruesight;

extern Clay_String StatSpecialAbilityOne;
extern Clay_String StatSpecialAbilityOneDesc;
extern Clay_String StatSpecialAbilityTwo;
extern Clay_String StatSpecialAbilityTwoDesc;
extern Clay_String StatSpecialAbilityThree;
extern Clay_String StatSpecialAbilityThreeDesc;
extern Clay_String StatSpecialAbilityFour;
extern Clay_String StatSpecialAbilityFourDesc;

extern Clay_String StatAttack1;
extern Clay_String StatAttack1Desc;
extern Clay_String StatAttack2;
extern Clay_String StatAttack2Desc;
extern Clay_String StatAttack3;
extern Clay_String StatAttack3Desc;
extern Clay_String StatAttack4;
extern Clay_String StatAttack4Desc;

extern Clay_String StatAttack5;
extern Clay_String StatAttack5Desc;

extern Clay_String StatAttack6;
extern Clay_String StatAttack6Desc;

extern Clay_String StatActionLeg;
extern Clay_String StatActionLeg1;
extern Clay_String StatActionLeg1Desc;
extern Clay_String StatActionLeg2;
extern Clay_String StatActionLeg2Desc;
extern Clay_String StatActionLeg3;
extern Clay_String StatActionLeg3Desc;

extern Clay_String StatActionLair;
extern Clay_String StatActionLair1;
extern Clay_String StatActionLair1Desc;
extern Clay_String StatActionLair2;
extern Clay_String StatActionLair2Desc;
extern Clay_String StatActionLair3;
extern Clay_String StatActionLair3Desc;

extern Clay_String StatRegionalEffect;
extern Clay_String StatRegionalEffect1;
extern Clay_String StatRegionalEffect2;
extern Clay_String StatRegionalEffect3;
extern Clay_String StatEndRegionalEffect;

extern Clay_String StatEnvironment;

extern Clay_String StatBa1;
extern Clay_String StatBa1Desc;
extern Clay_String StatBa2;
extern Clay_String StatBa2Desc;
extern Clay_String StatBa3;
extern Clay_String StatBa3Desc;
extern Clay_String StatBa4;
extern Clay_String StatBa4Desc;

extern Clay_String StatReaction1;
extern Clay_String StatReaction1Desc;
extern Clay_String StatReaction2;
extern Clay_String StatReaction2Desc;
extern Clay_String StatReaction3;
extern Clay_String StatReaction3Desc;

extern Clay_String StatVillAction;
extern Clay_String StatVillAction1;
extern Clay_String StatVillAction1Desc;
extern Clay_String StatVillAction2;
extern Clay_String StatVillAction2Desc;
extern Clay_String StatVillAction3;
extern Clay_String StatVillAction3Desc;

extern Clay_String StatUtilitySpells;
extern Clay_String StatUtilitySpellsList;

extern Clay_String StatFeature1;
extern Clay_String StatFeature1Desc;
extern Clay_String StatFeature2;
extern Clay_String StatFeature2Desc;
extern Clay_String StatFeature3;
extern Clay_String StatFeature3Desc;
extern Clay_String StatFeature4;
extern Clay_String StatFeature4Desc;
extern Clay_String StatFeature5;
extern Clay_String StatFeature5Desc;

#endif /* GLOBAL_H */