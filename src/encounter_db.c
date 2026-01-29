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
#include "encounter_db.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Extern global variables
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
static void FillEncounter();
static void DisplayEncounterChain(int position);

static void CallEncounterDetailsCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void CancelNewEncounterCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void AddEncounterCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void EncounterDatabaseWindow(AppState * state) {
    /* Player database window*/
    
    CLAY(CLAY_ID("EncounterDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("EncounterDBSidebar"), {SidebarLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("EncounterDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, &WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("EncounterDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(AddEncounterCallback, &WindowState);
                    CLAY_TEXT(CLAY_STRING("Add New"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("EncounterDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Remove Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("EncounterDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Edit Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomLayoutConfig ) {
                CLAY(CLAY_ID("PlayerTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (state->focusedId.id == CLAY_ID("PlayerTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .border = {
                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                .color = COLOR_WHITE
                            }
                }){
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("PlayerTextBox"), CurrentFocus, &DBPlayerSearch);       
                };

                CLAY(CLAY_ID("EncounterDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, &WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Name Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, &WindowState);
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        
        CLAY(CLAY_ID("EncounterDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            if (WindowState == NEW_STATBLOCK_SCREEN) {
                    //NewStatblockPage();
            }
            else {
                CLAY(CLAY_ID("PlayerHeaderContainer"), {
                    CreatureButtonContainerLayoutConfig,
                    .backgroundColor = COLOR_TRANSPARENT,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                    .clip = {true, true, Clay_GetScrollOffset()}
                }) {
                    for (int i = 0; i < TotalEncounters; i++) {
                        if (EncountersToShow[i] != -1) {
                            MakeEncounterHeader(i, CREATURE_DB_SCREEN);
                        }                    
                    }
                }
                CLAY(CLAY_ID("HeaderScrollBarContainer"), {
                    .layout = {
                            .sizing = { CLAY_SIZING_PERCENT(.01), CLAY_SIZING_PERCENT(1.0) },
                            .padding = { 0, 0, 0, 0},
                            .childGap = 2,
                            .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        },
                        .backgroundColor = COLOR_TRANSPARENT,
                }){
                    CLAY(CLAY_ID("HeaderScrollBar"), {
                        .layout = {
                            .sizing = { CLAY_SIZING_PERCENT(1), CLAY_SIZING_FIXED(10) },
                            .padding = { 0, 0, 0, 0},
                            .childGap = 2,
                            .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        },
                        .backgroundColor = COLOR_BUTTON_GRAY,
                        .floating = {
                            .parentId = CLAY_ID("PlayerHeaderContainer").id,
                        },
                    }) {

                    }
                }
                if (WindowState == SHOW_ENCOUNTER_DETAILS) {
                    FillEncounter();                
                } 
            }
        };
        if (gAppState->IsModalOpen) {
            Clay_String ModalText = { true, SDL_strlen(gAppState->ModalMessage), gAppState->ModalMessage };

            CreateBottomModal(gAppState->ModalParentId, ModalText);
            ModalTimer -= deltaTime;
            if (ModalTimer <= 0.0) {
                gAppState->IsModalOpen = false;
                gAppState->ModalParentId = CLAY_ID("NULL");
                memset(gAppState->ModalMessage, 0, sizeof(gAppState->ModalMessage));
                memset(gAppState->EncounterSaved, 0, sizeof(gAppState->EncounterSaved));
                ModalTimer = MODAL_TIMER;
            }
        }
    };
}

void MakeEncounterHeader(int i, int CallingWindow) {
    CLAY(CLAY_IDI("EncounterHeader", i), {
        PlayerButtonLayoutConfig,
        .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("EncounterHeader", i).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
    }) {

        CLAY_AUTO_ID({NameContainerLayoutConfig}){
            CLAY_TEXT(DBEncounterPageHeaders[i].EncounterName, CLAY_TEXT_CONFIG(ButtonTextConfig));
        };
    };
    Clay_OnHover(CallEncounterDetailsCallback, &DBEncounterPageHeaders[i].EncounterName);
}

static void FillEncounter() {
    CLAY(CLAY_ID("StatPage"), {
        StatPageContainer,
        .backgroundColor = (WindowState == CREATURE_DB_SCREEN) ? COLOR_TRANSPARENT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
        .clip = {false, true, Clay_GetScrollOffset()}
    }) {

        CLAY(CLAY_ID("EncounterNameContainer"), {
            StatPageDivider,
            .backgroundColor = COLOR_TRANSPARENT,
            .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }
        }) {
            CLAY_TEXT(gAppState->CurrentStatBlock.StatName, CLAY_TEXT_CONFIG(StatPageTextConfig));
        };
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
                    if (SDL_strlen(BuildListMembers[i].name) != 0) {
                        DisplayEncounterChain(i);
                    }
                }
    }
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */
static void DisplayEncounterChain(int position) {
    CLAY_AUTO_ID({
        BuildWindowRow,
        .backgroundColor = COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }){
        CLAY(CLAY_IDI("DislplayListInitiative", position), {
            BuildInitiativeQuantityLayoutConfig,
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("DislplayListInitiative", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
            .border = {
                    .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                    .color = COLOR_WHITE
                }
        }) {
            char InitiativeBuffer[8] = {0};
            SDL_itoa(BuildListMembers[position].initiative, InitiativeBuffer, 8);
            Clay_String InitiativeAdd = {
                .chars = InitiativeBuffer,
                .length = (int32_t)SDL_strlen(InitiativeBuffer),
                .isStaticallyAllocated = false
            };
            CLAY_TEXT(InitiativeAdd, CLAY_TEXT_CONFIG(StatPageTextConfig));
        }
        CLAY(CLAY_IDI("DisplayListName", position), {BuildWindowDescriptionHeader}){
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
        CLAY(CLAY_IDI("DisplayListQuantity", position), {
            BuildInitiativeQuantityLayoutConfig,
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("DisplayListQuantity", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
            .border = {
                    .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                    .color = COLOR_WHITE
                }
        }) {
            char QuantityBuffer[8] = {0};
            SDL_itoa(BuildListMembers[position].Quantity, QuantityBuffer, 8);
            Clay_String QuantityAdd = {
                .chars = QuantityBuffer,
                .length = (int32_t)SDL_strlen(QuantityBuffer),
                .isStaticallyAllocated = false
            };
            CLAY_TEXT(QuantityAdd, CLAY_TEXT_CONFIG(StatPageTextConfig));
        }
    }
}

/* Callbacks */
static void CallEncounterDetailsCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = elementId;
        gAppState->IsTextInputFocused = false;
        Clay_String *EncounterName = (Clay_String *)userData;

        /* copy into the fixed buffer and set length safely */
        SDL_strlcpy((char *)gAppState->EncounterToDisplay.chars, EncounterName->chars, sizeof(gAppState->EncounterToDisplayBuffer));
        gAppState->EncounterToDisplay.length = (int)SDL_strlen((char *)gAppState->EncounterToDisplay.chars);
        gAppState->EncounterToDisplay.isStaticallyAllocated = true;
        
        LookUpEncounterDetails(EncounterName);
        WindowState = SHOW_ENCOUNTER_DETAILS;
    }
}

static void AddEncounterCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->IsTextInputFocused = false;
        ClearTextBoxes();
        WindowState = BUILD_ENCOUNTER_SCREEN;
        gAppState->ActiveScreen = BUILD_ENCOUNTER_SCREEN;
    }
}

static void CancelNewEncounterCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->ActiveScreen = CREATURE_DB_SCREEN;
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->IsTextInputFocused = false;
        ClearTextBoxes();
        WindowState = CREATURE_DB_SCREEN;
    }
}