#ifndef GLOBAL_H
#define GLOBAL_H

#include "clay.h"
#include "stdio.h"
#include "stdint.h"
#include "../SDL3/renderincludes.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_events.h>

/*========================================================================* 
 *  SECTION - Defines
 *========================================================================* 
 */
#define MAIN_SCREEN                     0
#define FIRST_START_ENCOUNTER_SCREEN    1
#define BUILD_ENCOUNTER_SCREEN          2
#define CREATURE_DB_SCREEN              3
#define PLAYER_DB_SCREEN                4
#define SHOW_STAT_BLOCK                 5
#define ENCOUNTER_MAIN_SCREEN           6
#define START_NEW_ENCOUNTER_SCREEN      7
#define START_NEW_WITH_STATS_SCREEN     8
#define ADD_TO_ENCOUNTER                9
#define BUILD_NEW_ENCOUNTER_SCREEN      10
#define SELECT_EXISTING_SCREEN          11
#define NEW_STATBLOCK_SCREEN            12
#define ENCOUNTER_DB_SCREEN             13
#define SHOW_ENCOUNTER_DETAILS          14

#define BACKSPACE_KEY                   8
#define SPACEBAR_KEY                    32

#define COPY_TEXT                       0
#define WRITE_TEXT                      1

#define MAX_TEXT                        2048
#define BUILD_LIST_MAX                  50

/*========================================================================* 
 *  SECTION - Global struct typedefs
 *========================================================================* 
 */
typedef struct TextBox {
    Clay_String StringToDisplay;
    char TextBoxBuffer[MAX_TEXT];
    bool IsInitialized;
}TextBox;

 typedef struct DisplayListMember {
    Clay_String Name;
    int Initiative;
    char InitiativeBuffer[10];
    int ArmorClass;
    char ArmorClassBuffer[10];
    int HitPoints;
    char HitPointsBuffer[10];
    bool IsCreature;
    int SqliteDbId;
    TextBox HitPointsTextBox;
    int TurnOrder;
    struct DisplayListMember * Next;
}DisplayListMember;

typedef struct StatBlock {
    Clay_String StatId;
    Clay_String StatName;
    Clay_String StatCr;
    Clay_String StatType;
    Clay_String StatSize;

    Clay_String StatArmorClass;
    Clay_String StatHitpointsAvg;
    Clay_String StatHitDice;
    Clay_String StatHitpointsRoll;

    Clay_String StatSpeedType;
    Clay_String StatSpeedWalk;
    Clay_String StatSpeedFly;
    Clay_String StatSpeedSwim;
    Clay_String StatSpeedClimb;
    Clay_String StatSpeedBurrow;

    Clay_String StatAlignment;
    Clay_String StatLegendary;

    Clay_String StatStr;
    Clay_String StatDex;
    Clay_String StatCon;
    Clay_String StatInt;
    Clay_String StatWis;
    Clay_String StatCha;

    Clay_String StatProfBonus;

    Clay_String StatThrowStr;
    Clay_String StatThrowDex;
    Clay_String StatThrowCon;
    Clay_String StatThrowInt;
    Clay_String StatThrowWis;
    Clay_String StatThrowCha;

    Clay_String StatSavingThrows;

    Clay_String StatSkills;
    Clay_String StatLanguages;
    Clay_String StatSenses;

    Clay_String StatRangeDarkvision;
    Clay_String StatRangeTremorsense;
    Clay_String StatRangeBlindsight;
    Clay_String StatRangeTruesight;

    Clay_String StatSpecialAbilityOne;
    Clay_String StatSpecialAbilityOneDesc;
    Clay_String StatSpecialAbilityTwo;
    Clay_String StatSpecialAbilityTwoDesc;
    Clay_String StatSpecialAbilityThree;
    Clay_String StatSpecialAbilityThreeDesc;
    Clay_String StatSpecialAbilityFour;
    Clay_String StatSpecialAbilityFourDesc;

    Clay_String StatAttack1;
    Clay_String StatAttack1Desc;
    Clay_String StatAttack2;
    Clay_String StatAttack2Desc;
    Clay_String StatAttack3;
    Clay_String StatAttack3Desc;
    Clay_String StatAttack4;
    Clay_String StatAttack4Desc;

    Clay_String StatAttack5;
    Clay_String StatAttack5Desc;

    Clay_String StatAttack6;
    Clay_String StatAttack6Desc;

    Clay_String StatActionLeg;
    Clay_String StatActionLeg1;
    Clay_String StatActionLeg1Desc;
    Clay_String StatActionLeg2;
    Clay_String StatActionLeg2Desc;
    Clay_String StatActionLeg3;
    Clay_String StatActionLeg3Desc;

    Clay_String StatActionLair;
    Clay_String StatActionLair1;
    Clay_String StatActionLair1Desc;
    Clay_String StatActionLair2;
    Clay_String StatActionLair2Desc;
    Clay_String StatActionLair3;
    Clay_String StatActionLair3Desc;

    Clay_String StatRegionalEffect;
    Clay_String StatRegionalEffect1;
    Clay_String StatRegionalEffect2;
    Clay_String StatRegionalEffect3;
    Clay_String StatEndRegionalEffect;

    Clay_String StatEnvironment;

    Clay_String StatBa1;
    Clay_String StatBa1Desc;
    Clay_String StatBa2;
    Clay_String StatBa2Desc;
    Clay_String StatBa3;
    Clay_String StatBa3Desc;
    Clay_String StatBa4;
    Clay_String StatBa4Desc;

    Clay_String StatReaction1;
    Clay_String StatReaction1Desc;
    Clay_String StatReaction2;
    Clay_String StatReaction2Desc;
    Clay_String StatReaction3;
    Clay_String StatReaction3Desc;

    Clay_String StatVillAction;
    Clay_String StatVillAction1;
    Clay_String StatVillAction1Desc;
    Clay_String StatVillAction2;
    Clay_String StatVillAction2Desc;
    Clay_String StatVillAction3;
    Clay_String StatVillAction3Desc;

    Clay_String StatUtilitySpells;
    Clay_String StatUtilitySpellsList;

    Clay_String StatFeature1;
    Clay_String StatFeature1Desc;
    Clay_String StatFeature2;
    Clay_String StatFeature2Desc;
    Clay_String StatFeature3;
    Clay_String StatFeature3Desc;
    Clay_String StatFeature4;
    Clay_String StatFeature4Desc;
    Clay_String StatFeature5;
    Clay_String StatFeature5Desc;
}StatBlock;

typedef struct BuildListMember {
    int initiative;
    char name[64];
    int Quantity;
    bool IsAdded;
    bool IsCreature;
}BuildListMember;

typedef struct Encounter {
    Clay_String EncounterName;
    BuildListMember Creatures[BUILD_LIST_MAX];
    BuildListMember Players[BUILD_LIST_MAX];
} Encounter;

typedef struct app_state {
    SDL_Window *window;
    Clay_SDL3RendererData rendererData;
    Clay_ElementId focusedId;
    Clay_ElementId SliderId;
    bool IsTextInputFocused;
    Clay_String StringToModify;
    Clay_String EncounterToDisplay;
    char EncounterToDisplayBuffer[256];
    DisplayListMember **SortedListArray;
    int SortedListCount;
    StatBlock CurrentStatBlock;
    bool ModifyCurrentEncounter;
    int ActiveScreen;
    bool IsModalOpen;
    Clay_ElementId ModalParentId;
    char ModalMessage[256];
    char EncounterSaved[256];
    char CreatureSaved[256];
} AppState;

extern BuildListMember BuildListMembers[BUILD_LIST_MAX];

/*========================================================================*
 *  SECTION - Global variables
 *========================================================================*
 */
extern double deltaTime;

extern float MouseX;
extern float MouseY;
extern DisplayListMember * NewMember;
extern DisplayListMember * Head;
extern DisplayListMember * Tail;

extern int ListStarted;

extern int StartEncounterState;

extern TextBox EncounterName;
extern TextBox BuildCreatureSearch;
extern TextBox BuildPlayerSearch;
extern TextBox DBCreatureSearch;
extern TextBox DBPlayerSearch;

extern TextBox BuildListMemberQuantity[BUILD_LIST_MAX];
extern TextBox BuildListMemberInitiative[BUILD_LIST_MAX];

extern AppState *gAppState;

extern char TextBuffer[MAX_TEXT];

extern uint32_t PreviousFocusId;

//char textBuffer[MAX_TEXT] = {0};

extern bool MouseDown;

extern int WindowState;

extern int WindowWidth;
extern int WindowHeight;
extern uint16_t TotalCreatures;
extern uint16_t TotalPlayers;
extern uint16_t TotalEncounters;

extern const int FONT_ID_BODY_16;
extern const int FONT_ID_BODY_32;

/*========================================================================* 
 *  SECTION - Global prototypes
 *========================================================================* 
 */
void InitializeTextBoxes();
void ModifyTextBoxText(TextBox * TextToModify, uint32_t CopyOrWrite);
void FocusAndWriteTextBox(Clay_ElementId IdToFocus, uint32_t CurrentFocus, TextBox * TextToModify);
void InitStatBlock(StatBlock *sb);
void FreeStatBlock(StatBlock *sb);
void ClearClayString(Clay_String *s);
void InitializeOneTextBox(TextBox * TextBoxToInit);
void FreeDisplayList(DisplayListMember *head);
void ClearTextBoxes();
void FreeLinkedLists();
void ResetVisibleCreatureHeaders();
void ClearFocus();
void ResetBuildListData();

void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void FocusWindowCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData);
void SearchButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void FocusWindowAndCallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData);
void ClearFocusCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData);

#endif /* GLOBAL_H */