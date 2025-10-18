#include "main_window.h"
#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
void StylesInit(void);

static void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);

void BuildEncounterWindow(AppState * state);
void CreatureDatabaseWindow(AppState * state);
void PlayerDatabaseWindow(AppState * state);

/*========================================================================* 
 *  SECTION - Style declarations
 *========================================================================* 
 */
static Clay_ElementDeclaration SidebarTopPartWindowStyle;
static Clay_ElementDeclaration SidebarBottomPartWindowStyle;
static Clay_ElementDeclaration MainScreenStyle;
static Clay_ElementDeclaration LTRParentWindowStyle;
static Clay_ElementDeclaration ContentWindowStyle;
static Clay_ElementDeclaration SidebarWindowStyle;
static Clay_ElementDeclaration MainButtonStyle;
static Clay_ElementDeclaration SingleLineInputTextStyle;
static Clay_ElementDeclaration StatBlockContainerStyle;
static Clay_ElementDeclaration DBContentWindowStyle;
static Clay_ElementDeclaration CreatureNameHeader;

static Clay_TextElementConfig ButtonLabelTextConfig;
static Clay_TextElementConfig InputTextTextConfig;
static Clay_TextElementConfig WindowLabelTextConfig;
static Clay_TextElementConfig MainLabelTextConfig;

char * CreatureNames[7] = {"Aboleth", "Acolyte", "AdultBlackDragon", "AdultBlueDragon", "AirElemental", "Ape", "Azer"};

/*========================================================================* 
 *  SECTION - Global Functions 
 *========================================================================* 
 */
Clay_RenderCommandArray MainWindow(AppState * state)
{
    Clay_BeginLayout();
    StylesInit();

    // Clay_Sizing layoutExpand = {
    // .width = CLAY_SIZING_GROW(0),
    // .height = CLAY_SIZING_GROW(0)
    // };

    // Define one element that covers the whole screen
    CLAY(CLAY_ID("OuterContainer"), MainScreenStyle) {

        switch (WindowState){
            case 0:
            /* Main label */
            CLAY_TEXT(CLAY_STRING("GUIDNBATTER"), CLAY_TEXT_CONFIG(MainLabelTextConfig));
            /* Start button */
            CLAY(CLAY_ID("StartButton"), MainButtonStyle) {
                CLAY_TEXT(CLAY_STRING("Start Encounter"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                Clay_OnHover(StartEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Build button */
            CLAY(CLAY_ID("BuildButton"), MainButtonStyle) {
                CLAY_TEXT(CLAY_STRING("Build Encounter"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig)); 
                Clay_OnHover(BuildEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Creature DB button */
            CLAY(CLAY_ID("CreatureDatabaseButton"), MainButtonStyle) {
                CLAY_TEXT(CLAY_STRING("Creature Database"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                Clay_OnHover(CreatureDatabaseButtonCallback, (intptr_t)WindowState);
            };

            /* Player DB button */ 
            CLAY(CLAY_ID("PlayerDatabaseButton"), MainButtonStyle) {
                CLAY_TEXT(CLAY_STRING("Player Database"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                Clay_OnHover(PlayerDatabaseButtonCallback, (intptr_t)WindowState);
                
            };
            break;

        case START_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("StartEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(10), .backgroundColor = COLOR_BLUE}) {
                CLAY_TEXT(CLAY_STRING("Start Encounter"), CLAY_TEXT_CONFIG(WindowLabelTextConfig));
                Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
            };
            break;
        
        case BUILD_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("BuildEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(10), .backgroundColor = COLOR_ORANGE}) {
                CLAY_TEXT(CLAY_STRING("Build Encounter"), CLAY_TEXT_CONFIG(WindowLabelTextConfig));
                Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
            };
            break;
        
        case CREATURE_DB_SCREEN:
            CreatureDatabaseWindow(state);
            break;

        case PLAYER_DB_SCREEN:
            PlayerDatabaseWindow(state);            
            break;

        default:
            break;
        }
    };

    return Clay_EndLayout();
}

/*========================================================================* 
 *  SECTION - Local Functions 
 *========================================================================*
 */

 void StylesInit(void) {    
    MainScreenStyle                 = MakeParentWindowStyle(8, 40, CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER, CLAY_TOP_TO_BOTTOM, COLOR_BLACK);
    DBContentWindowStyle            = MakeParentWindowStyle(8, 8, CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER, CLAY_LEFT_TO_RIGHT, COLOR_BLACK);
    LTRParentWindowStyle            = MakeParentWindowStyle(0, 8, CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER, CLAY_LEFT_TO_RIGHT, COLOR_BLACK);
    SidebarTopPartWindowStyle       = MakeFixedWidthLimitHeightWindow(SIDEBAR_WIDTH_PX, WindowHeight/2, 8, 16, CLAY_ALIGN_Y_TOP, CLAY_TOP_TO_BOTTOM);
    SidebarBottomPartWindowStyle    = MakeFixedWidthLimitHeightWindow(SIDEBAR_WIDTH_PX, WindowHeight/2, 8, 16, CLAY_ALIGN_Y_BOTTOM, CLAY_TOP_TO_BOTTOM);
    ContentWindowStyle              = MakeFixedWidthGrowHeightWindow(DB_LIST_WIDTH_PX, 0, 8, CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER, CLAY_LEFT_TO_RIGHT, COLOR_BLACK);
    SidebarWindowStyle              = MakeSidebarStyle(SIDEBAR_WIDTH_PX, WindowHeight, 0, 0, CLAY_ALIGN_Y_CENTER, CLAY_TOP_TO_BOTTOM, COLOR_GRAY_BG);
    MainButtonStyle                 = FixedContainerTTBStyle(200, 50, 8, 16, COLOR_BUTTON_GRAY);
    ButtonLabelTextConfig           = TextConfig(COLOR_WHITE, 0, 16, CLAY_TEXT_ALIGN_CENTER, CLAY_TEXT_WRAP_WORDS);
    InputTextTextConfig             = TextConfig(COLOR_WHITE, 0, 16, CLAY_TEXT_ALIGN_LEFT, CLAY_TEXT_WRAP_WORDS);
    WindowLabelTextConfig           = TextConfig(COLOR_WHITE, 0, 16, CLAY_TEXT_ALIGN_CENTER, CLAY_TEXT_WRAP_NONE);
    MainLabelTextConfig             = TextConfig(COLOR_WHITE, 0, 64, CLAY_TEXT_ALIGN_CENTER, CLAY_TEXT_WRAP_NONE);
    SingleLineInputTextStyle        = SingleLineTextContainerStyle(214, 32, 8, 16, COLOR_GRAY_BG, 5, 2, COLOR_WHITE);
    StatBlockContainerStyle         = MakeFixedWidthGrowHeightWindow(DB_LIST_WIDTH_PX, 8, 8, CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP, CLAY_TOP_TO_BOTTOM, COLOR_GRAY_BG);
    CreatureNameHeader              = FixedContainerTTBStyle(300, 150, 8, 8, COLOR_BUTTON_GRAY);
}

void BuildEncounterWindow(AppState * state) {

}

void CreatureDatabaseWindow(AppState * state) {
    /* Creature database window*/
    
    CLAY(CLAY_ID("CreatureDBOuterContainer"), LTRParentWindowStyle) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("CreatureDBSidebar"), SidebarWindowStyle) {
            
            CLAY(CLAY_ID("SidebarTop"), SidebarTopPartWindowStyle) {

                CLAY(CLAY_ID("CreatureDBHomeButton"), MainButtonStyle) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBAddButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBRemoveButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBEditButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomPartWindowStyle ) {
                CLAY(CLAY_ID("CreatureTextBox"), SingleLineInputTextStyle){

                    /* Create char* and set equal to the overall buffer that reads keyboard input */
                    char * SearchText = &TextBuffer[0];
                    /* Custom clay_string to allow for a dynamically changing char* */
                    Clay_String SomeTextMaybe = {.isStaticallyAllocated = true, .length = SDL_strlen(SearchText), .chars = SearchText};
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(SomeTextMaybe, CLAY_TEXT_CONFIG(InputTextTextConfig));
                };

                CLAY(CLAY_ID("CreatureDBSearchButton"), MainButtonStyle) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("CreatureDBContentWindow"), ContentWindowStyle) {
            // TODO: Fill with statblocks with info pulled from sqlite db
            // 1. Design a stat block
            // 2. Find a way to collapse and expand it

            /* Container for Creature Header Information */
            // TODO: Make scrollable
            CLAY(CLAY_ID("StatBlock"), StatBlockContainerStyle) {
                int number = 7;
                for (int i = 0; i < number; i++) {
                    CLAY(CLAY_IDI("CreatureHeader", i), CreatureNameHeader) {
                        CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                    };
                }

            }
        };
    };
}

void PlayerDatabaseWindow(AppState * state) {
    /* Player database window*/
    CLAY(CLAY_ID("PlayerDBOuterContainer"), LTRParentWindowStyle ) {
        
        /* Sidebar for option buttons */
        CLAY(CLAY_ID("PlayerDBSidebar"), SidebarWindowStyle) {
            
            CLAY(CLAY_ID("SidebarTop"), SidebarTopPartWindowStyle) {

                CLAY(CLAY_ID("PlayerDBHomeButton"), MainButtonStyle) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBAddButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBRemoveButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBEditButton"), MainButtonStyle) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomPartWindowStyle) {
                CLAY(CLAY_ID("PlayerTextBox"), SingleLineInputTextStyle){

                    /* Create char* and set equal to the overall buffer that reads keyboard input */
                    char * SearchText = &TextBuffer[0];
                    /* Custom clay_string to allow for a dynamically changing char* */
                    Clay_String SomeTextMaybe = {.isStaticallyAllocated = true, .length = SDL_strlen(SearchText), .chars = SearchText};
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(SomeTextMaybe, CLAY_TEXT_CONFIG(InputTextTextConfig));       
                };

                CLAY(CLAY_ID("PlayerDBSearchButton"), MainButtonStyle) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonLabelTextConfig));
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("PlayerDBContentWindow"), ContentWindowStyle){
            CLAY(CLAY_ID("PlayerDBHeader"), { HeadLabelWindow, .cornerRadius = CLAY_CORNER_RADIUS(10), .backgroundColor = COLOR_RED}) {
                CLAY_TEXT(CLAY_STRING("Player DB"), CLAY_TEXT_CONFIG(WindowLabelTextConfig));
            };          
        };
    };
}

/*-------------------------------------------------------------------------------------------*
*                                    Button Callbacks                                        *
*--------------------------------------------------------------------------------------------*/

static void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        SDL_memset(TextBuffer, 0, sizeof(TextBuffer));
        WindowState = MAIN_SCREEN;
    }
}

static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = START_ENCOUNTER_SCREEN;
    }
}

static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = BUILD_ENCOUNTER_SCREEN;
    }
}

static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = CREATURE_DB_SCREEN;
    }
}

static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = PLAYER_DB_SCREEN;
    }
}