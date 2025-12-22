#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "modal.h"
#include "db_query.h"
#include "main_window.h"
#include "build_encounter.h"
#include "start_encounter.h"
#include "player_db.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Extern global variables
 *========================================================================* 
 */
BuildListMember BuildListMembers[BUILD_LIST_MAX] = {0};

/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */
static Clay_String FinnName = {0};
static Clay_String RaviName = {0};
static Clay_String PaxName = {0};
static Clay_String TheonName = {0};

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
void BuildEncounterChain(int position);
void AddToBuildChain(const char *ParticipantToAdd, bool IsCreature);
void StartButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void SaveEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void BuildEncounterWindow(AppState * state, int CallingScreen) {
    CLAY(CLAY_ID("BuildWindowOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)} ) {
        /* Sidebar for option buttons */
        CLAY(CLAY_ID("BuildEncounterSidebar"), {
            SidebarLayoutConfig,
            .backgroundColor = COLOR_GRAY_BG,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
        }) {
            
            CLAY(CLAY_ID("BuildPlayerSidebarTop"), {
                SidebarTopLayoutConfig,
                .backgroundColor = COLOR_TRANSPARENT,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                .clip = {true, true, Clay_GetScrollOffset()}
            }) {
                for (int i = 0; i < TotalPlayers; i++) {
                    if (PlayersToShow[i] != -1) {
                        MakePlayerHeader(i, BUILD_ENCOUNTER_SCREEN);
                    }
                }

                // CLAY(CLAY_ID("PlayerOne"), {PlayerButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                //     FinnName = CLAY_STRING("Finn");
                //     Clay_OnHover(PlayerBuildListCallback, &FinnName);
                //     CLAY_TEXT(CLAY_STRING("Finn"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                // };
                // CLAY(CLAY_ID("PlayerTwo"), {PlayerButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                //     RaviName = CLAY_STRING("Ravi");
                //     Clay_OnHover(PlayerBuildListCallback, &RaviName);
                //     CLAY_TEXT(CLAY_STRING("Ravi"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                // };
                // CLAY(CLAY_ID("PlayerThree"), {PlayerButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                //     PaxName = CLAY_STRING("Pax");
                //     Clay_OnHover(PlayerBuildListCallback, &PaxName);
                //     CLAY_TEXT(CLAY_STRING("Pax"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                // };
                // CLAY(CLAY_ID("PlayerFour"), {PlayerButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                //     TheonName = CLAY_STRING("Theon");
                //     Clay_OnHover(PlayerBuildListCallback, &TheonName);
                //     CLAY_TEXT(CLAY_STRING("Theon"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                // };
            };
            
            CLAY(CLAY_ID("PlayerSidebarBottom"), SidebarBottomLayoutConfig) {
                CLAY(CLAY_ID("PlayerSearchTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (state->focusedId.id == CLAY_ID("PlayerSearchTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .border = {
                        .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                        .color = COLOR_WHITE
                    }
                }){
                    Clay_OnHover(FocusWindowCallback, state);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("PlayerSearchTextBox"), CurrentFocus, &BuildPlayerSearch);
                }

                CLAY(CLAY_ID("BuildPlayerSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                }; 
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("BuildEncounterContentWindow"), {
            TTBBuildWindowLayoutConfig,
            .backgroundColor = COLOR_GRAY_BG,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
            //.clip = {true, true, Clay_GetScrollOffset()}
        }) {
            if ( MAIN_SCREEN == CallingScreen) {
                CLAY(CLAY_ID("NameEncounter"),{
                    BuildWindowRow,
                    .backgroundColor = COLOR_TRANSPARENT,
                }) {
                    CLAY_TEXT(CLAY_STRING("Encounter Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("EncounterNameTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (state->focusedId.id == CLAY_ID("EncounterNameTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .border = {
                        .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                        .color = COLOR_WHITE
                    }
                }){
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("EncounterNameTextBox"), CurrentFocus, &EncounterName);
                }
                }
            }
            CLAY(CLAY_ID("BuildListHeader"), {
                    BuildWindowRow
                }) {
                    CLAY_AUTO_ID(BuildWindowInitiativeHeader){
                        CLAY_TEXT(CLAY_STRING("Initiative"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    }
                    CLAY_AUTO_ID(BuildWindowDescriptionHeader){
                        CLAY_TEXT(CLAY_STRING("Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    }
                    CLAY_AUTO_ID(BuildWindowQuantityHeader){
                        CLAY_TEXT(CLAY_STRING("Quantity"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    }
                }
            CLAY(CLAY_ID("BuildEncounterListContainer"), {
                TTBBuildWindowLayoutConfig,
                .backgroundColor = COLOR_GRAY_BG,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                .clip = {true, true, Clay_GetScrollOffset()}
            }) {
                for (int i = 0; i < BUILD_LIST_MAX; i++) {
                    if (SDL_strlen(BuildListMembers[i].name) != 0) {
                        BuildEncounterChain(i);
                    }
                }
            }
            CLAY(CLAY_ID("BuildEncounterButtonContainer"), {
                TTBBuildListBottom,
                .backgroundColor = COLOR_GRAY_BG,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
            }) {
                CLAY(CLAY_ID("ExitButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, &WindowState);
                    CLAY_TEXT(CLAY_STRING("Exit"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                if ( MAIN_SCREEN == CallingScreen) {
                    CLAY(CLAY_ID("SaveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        Clay_OnHover(SaveEncounterButtonCallback, &WindowState);
                        CLAY_TEXT(CLAY_STRING("Save"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                }
                else if ( FIRST_START_ENCOUNTER_SCREEN == CallingScreen) {
                    CLAY(CLAY_ID("StartButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        Clay_OnHover(StartButtonCallback, &WindowState);
                        CLAY_TEXT(CLAY_STRING("Start"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                }
                
            } 
        };
        CLAY(CLAY_ID("BuildEncounterCreatureSidebar"), {
            SidebarLayoutConfig,
            .backgroundColor = COLOR_GRAY_BG,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
        }) {
            
            CLAY(CLAY_ID("BuildCreatureSidebarTop"), {SidebarTopLayoutConfig,
                .backgroundColor = COLOR_TRANSPARENT,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                .clip = {true, true, Clay_GetScrollOffset()}
            }) /* Build Creature Sidebar Top contents*/ {
                for (int i = 0; i < TotalCreatures; i++) {
                    if (HeadersToShow[i] != -1) {
                        MakeCreatureHeader(i, BUILD_ENCOUNTER_SCREEN);
                    }
                }
            };
            
            CLAY(CLAY_ID("CreatureSidebarBottom"), SidebarBottomLayoutConfig) {
                CLAY(CLAY_ID("CreatureSearchTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (state->focusedId.id == CLAY_ID("CreatureSearchTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .border = {
                        .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                        .color = COLOR_WHITE
                    }
                }){
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("CreatureSearchTextBox"), CurrentFocus, &BuildCreatureSearch);
                }

                CLAY(CLAY_ID("BuildCreatureSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, NULL);
                };              
            };
        };
    };
    if (BuildingEncounter == 1) {
        NewEncounterScreen();
    }
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */
void BuildEncounterChain(int position) {
    CLAY_AUTO_ID({
        BuildWindowRow,
        .backgroundColor = COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }){
        CLAY(CLAY_IDI("BuildListInitiative", position), {
            BuildInitiativeQuantityLayoutConfig,
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("BuildListInitiative", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
        }){
            Clay_OnHover(FocusWindowCallback, gAppState);
            uint32_t CurrentFocus = gAppState->focusedId.id;
            FocusAndWriteTextBox(CLAY_IDI("BuildListInitiative", position), CurrentFocus, &BuildListMemberInitiative[position]);
            BuildListMembers[position].initiative = SDL_atoi(BuildListMemberInitiative[position].TextBoxBuffer);
        }
        CLAY(CLAY_IDI("BuildListName", position), {BuildWindowDescriptionHeader}){
            if ('\0' != BuildListMembers[position].name[0]) {
                char NameBuffer[64] = {0};
                SDL_strlcpy(NameBuffer, BuildListMembers[position].name, 64);
                Clay_String NameAdd = {
                    .chars = BuildListMembers[position].name,
                    .length = (int32_t)SDL_strlen(BuildListMembers[position].name),
                    .isStaticallyAllocated = true
                };
                CLAY_TEXT(NameAdd, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }
        CLAY(CLAY_IDI("BuildListQuantity", position), {
            BuildInitiativeQuantityLayoutConfig,
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("BuildListQuantity", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
        }){
            Clay_OnHover(FocusWindowCallback, gAppState);
            uint32_t CurrentFocus = gAppState->focusedId.id;
            FocusAndWriteTextBox(CLAY_IDI("BuildListQuantity", position), CurrentFocus, &BuildListMemberQuantity[position]);
            BuildListMembers[position].Quantity = SDL_atoi(BuildListMemberQuantity[position].TextBoxBuffer);
        }
    }
}

void AddToBuildChain(const char *ParticipantToAdd, bool IsCreature) {
    for (int i = 0; i < BUILD_LIST_MAX; i++) {
        if ('\0' == BuildListMembers[i].name[0]) {
            SDL_strlcpy(BuildListMembers[i].name, ParticipantToAdd, 64);
            BuildListMembers[i].Quantity = 1;
            if (IsCreature) {
                BuildListMembers[i].IsCreature = true;
            }
            else {
                BuildListMembers[i].IsCreature = false;
            }
            break;
        }
        else if (SDL_strncasecmp(BuildListMembers[i].name, ParticipantToAdd, SDL_strlen(ParticipantToAdd)) == 0) {
            BuildListMembers[i].Quantity += 1;
            if (IsCreature) {
                BuildListMembers[i].IsCreature = true;
            }
            else {
                BuildListMembers[i].IsCreature = false;
            }
            break;
        }
    }    
}

void PlayerBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        Clay_String *NameToAdd = (Clay_String *) userData;
        AddToBuildChain(NameToAdd->chars, false);
    }
}

void CreatureBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        Clay_String *NameToAdd = (Clay_String *) userData;
        AddToBuildChain(NameToAdd->chars, true);
    }
}

void SaveEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->IsTextInputFocused = false;
        SaveEncounterToDB();

        /* Saving encounter name before reset to display in modal */
        SDL_strlcpy(gAppState->EncounterSaved, EncounterName.StringToDisplay.chars, sizeof(gAppState->EncounterSaved));

        /* Reset and return to main window after saving successfully */
        ClearFocus();
        ClearTextBoxes();
        WindowState = MAIN_SCREEN;
        ResetVisibleCreatureHeaders();
        FreeLinkedLists();
        ResetBuildListData();

        /* Building modal message */
        gAppState->IsModalOpen = true;
        gAppState->ModalParentId = CLAY_ID("OuterContainer");
        char temp[256];
        SDL_snprintf(gAppState->ModalMessage, sizeof(gAppState->ModalMessage), "Encounter %s saved successfully.", gAppState->EncounterSaved);
    }
}

void StartButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->IsTextInputFocused = false;
        StartEncounterState = START_NEW_ENCOUNTER_SCREEN;
        BuildingEncounter = 1;
    }
}