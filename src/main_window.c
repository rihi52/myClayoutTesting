#include "main_window.h"
#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"
#include "db_query.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */

static void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);

void BuildEncounterWindow(AppState * state);
void CreatureDatabaseWindow(AppState * state);
void PlayerDatabaseWindow(AppState * state);
void MakeCreatureHeader(int i);

/*========================================================================* 
 *  SECTION - Style declarations
 *========================================================================* 
 */

char * CreatureNames[7] = {"Aboleth", "Acolyte", "Adult Black Dragon", "Adult Blue Dragon", "Air Elemental", "Ape", "Azer"};
char * CreatureCR[7] = {"CR 10", "CR 10", "CR 10", "CR 10", "CR 10", "CR 10", "CR 10"};
char * CreatureSize[7] = {"Large", "Large", "Large", "Large", "Large", "Large", "Large"};
char * CreatureType[7] = {"Aberration", "Aberration", "Aberration", "Aberration", "Aberration", "Aberration", "Aberration"};
char * CreatureSource[7] = {"SRD", "SRD", "SRD", "SRD", "SRD", "SRD", "SRD"};

/*========================================================================* 
 *  SECTION - Global Functions 
 *========================================================================* 
 */
Clay_RenderCommandArray MainWindow(AppState * state)
{
    Clay_BeginLayout();

    // Define one element that covers the whole screen
    CLAY(CLAY_ID("OuterContainer"), {MainScreenLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        switch (WindowState){
            case 0:
            /* Main label */
            CLAY_TEXT(CLAY_STRING("GUIDNBATTER"), CLAY_TEXT_CONFIG(MainLabelTextConfig));
            /* Start button */
            CLAY(CLAY_ID("StartButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Start Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(StartEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Build button */
            CLAY(CLAY_ID("BuildButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Build Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig)); 
                Clay_OnHover(BuildEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Creature DB button */
            CLAY(CLAY_ID("CreatureDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Creature Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(CreatureDatabaseButtonCallback, (intptr_t)WindowState);
            };

            /* Player DB button */ 
            CLAY(CLAY_ID("PlayerDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Player Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(PlayerDatabaseButtonCallback, (intptr_t)WindowState);
                
            };
            break;

        case START_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("StartEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_BLUE}) {
                Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
            };
            break;
        
        case BUILD_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("BuildEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_ORANGE}) {
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

void BuildEncounterWindow(AppState * state) {

}

void CreatureDatabaseWindow(AppState * state) {
    /* Creature database window*/
    
    CLAY(CLAY_ID("CreatureDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("CreatureDBSidebar"), {SidebarLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("CreatureDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomLayoutConfig ) {
                CLAY(CLAY_ID("CreatureTextBox"), {
                                                    SingleLineInputLayoutConfig,
                                                    .backgroundColor = COLOR_GRAY_BG,
                                                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                                                    .border = {
                                                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                                                .color = COLOR_WHITE
                                                            }
                                                }){
                    /* Create char* and set equal to the overall buffer that reads keyboard input */
                    char * SearchText = &TextBuffer[0];
                    /* Custom clay_string to allow for a dynamically changing char* */
                    Clay_String SomeTextMaybe = {.isStaticallyAllocated = true, .length = SDL_strlen(SearchText), .chars = SearchText};
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(SomeTextMaybe, CLAY_TEXT_CONFIG(InputTextConfig));
                };

                CLAY(CLAY_ID("CreatureDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("CreatureDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            // TODO: Fill with statblocks with info pulled from sqlite db
            // 1. Design a stat block
            // 2. Find a way to collapse and expand it

            /* Container for Creature Header Information */
            // TODO: Make scrollable
            CLAY(CLAY_ID("CreatureHeaderContainer"), { CreatureButtonContainerLayoutConfig,
                                        .backgroundColor = COLOR_TRANSPARENT,
                                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                                        .clip = {true, true, Clay_GetScrollOffset()}
                                    }) {
                int number = 7;
                // TODO: This just rewrites everything to be the most recent query
                // Maybe separate function using an array of structs the query writes to?
                // Then loop through that array with MakeClayString and fill text that way
                for (int i = 0; i < number; i++) {
                    MakeCreatureHeader(i);
                    // LoadCreatureHeaderAlphabetical(i + 1);

                    // //Clay_String CreatureSourceText = {.isStaticallyAllocated = true, .length = SDL_strlen(LookUpHeader->CreatureSource), .chars = LookUpHeader->CreatureSource};
                    // CLAY(CLAY_IDI("CreatureHeader", i), {CreatureButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)}) {
                    //     Clay_ElementId id = CLAY_IDI("CreatureHeader", 0);
                    //     Clay_ElementData HeaderData = Clay_GetElementData(id);
                    //         CLAY_AUTO_ID({NameContainerLayoutConfig}){
                    //             CLAY_TEXT(DBPageHeaders[i].CreatureName, CLAY_TEXT_CONFIG(ButtonTextConfig));
                    //         };

                    //     if (146 <= HeaderData.boundingBox.width ) {
                    //         CLAY_AUTO_ID({SizeCRContainerLayoutConfig}) {
                    //             /* change to be just size and type */
                    //             CLAY_AUTO_ID({CRContainerLayoutConfig}){
                    //                 CLAY_TEXT(DBPageHeaders[i].CreatureCR, CLAY_TEXT_CONFIG(ButtonTextConfig));
                    //             };
                    //             CLAY_AUTO_ID({SizeContainerLayoutConfig}){
                    //                 CLAY_TEXT(DBPageHeaders[i].CreatureSize, CLAY_TEXT_CONFIG(ButtonTextConfig));
                    //             };
                    //         };
                    //         CLAY_AUTO_ID({TypeSrcContainerLayoutConfig}) {
                    //             CLAY_AUTO_ID({TypeContainerLayoutConfig}) {
                    //                 CLAY_TEXT(DBPageHeaders[i].CreatureType, CLAY_TEXT_CONFIG(ButtonTextConfig));
                    //             };
                    //             CLAY_AUTO_ID({SourceContainerLayoutConfig}) {
                    //                 //CLAY_TEXT(CreatureSourceText, CLAY_TEXT_CONFIG(ButtonTextConfig));
                    //             };
                    //         };
                    //     }
                                
                        
                    //     /* TODO: I think this flickers in and out so fast you can't see it because nothing is holding it on screen */
                    //     Clay_OnHover(CallStatBlockCallback, (intptr_t)WindowState);
                    // };
                }

            }
        };
    };
}

void PlayerDatabaseWindow(AppState * state) {
    /* Player database window*/
    CLAY(CLAY_ID("PlayerDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)} ) {
        
        /* Sidebar for option buttons */
        CLAY(CLAY_ID("PlayerDBSidebar"), SidebarLayoutConfig) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("PlayerDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomLayoutConfig) {
                CLAY(CLAY_ID("PlayerTextBox"), {
                                                    SingleLineInputLayoutConfig,
                                                    .backgroundColor = COLOR_GRAY_BG,
                                                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                                                    .border = {
                                                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                                                .color = COLOR_WHITE
                                                            }
                                                }){

                    /* Create char* and set equal to the overall buffer that reads keyboard input */
                    char * SearchText = &TextBuffer[0];
                    /* Custom clay_string to allow for a dynamically changing char* */
                    Clay_String SomeTextMaybe = {.isStaticallyAllocated = true, .length = SDL_strlen(SearchText), .chars = SearchText};
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(SomeTextMaybe, CLAY_TEXT_CONFIG(InputTextConfig));       
                };

                CLAY(CLAY_ID("PlayerDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("PlayerDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}){
            CLAY(CLAY_ID("PlayerDBHeader"), { HeadLabelWindow, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_RED}) {
            };          
        };
    };
}

void MakeCreatureHeader(int i) {
    

    //Clay_String CreatureSourceText = {.isStaticallyAllocated = true, .length = SDL_strlen(LookUpHeader->CreatureSource), .chars = LookUpHeader->CreatureSource};
    CLAY(CLAY_IDI("CreatureHeader", i), {CreatureButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)}) {
        Clay_ElementId id = CLAY_IDI("CreatureHeader", 0);
        Clay_ElementData HeaderData = Clay_GetElementData(id);
            CLAY_AUTO_ID({NameContainerLayoutConfig}){
                CLAY_TEXT(DBPageHeaders[i].CreatureName, CLAY_TEXT_CONFIG(ButtonTextConfig));
            };

        if (146 <= HeaderData.boundingBox.width ) {
            CLAY_AUTO_ID({SizeCRContainerLayoutConfig}) {
                /* change to be just size and type */
                CLAY_AUTO_ID({CRContainerLayoutConfig}){
                    CLAY_TEXT(DBPageHeaders[i].CreatureCR, CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY_AUTO_ID({SizeContainerLayoutConfig}){
                    CLAY_TEXT(DBPageHeaders[i].CreatureSize, CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            CLAY_AUTO_ID({TypeSrcContainerLayoutConfig}) {
                CLAY_AUTO_ID({TypeContainerLayoutConfig}) {
                    CLAY_TEXT(DBPageHeaders[i].CreatureType, CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY_AUTO_ID({SourceContainerLayoutConfig}) {
                    //CLAY_TEXT(CreatureSourceText, CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
        }
                
        
        /* TODO: I think this flickers in and out so fast you can't see it because nothing is holding it on screen */
        Clay_OnHover(CallStatBlockCallback, (intptr_t)WindowState);
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

static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        CLAY_AUTO_ID({TTBParentWindowLayoutConfig, .backgroundColor = COLOR_GREEN});
    }
}