#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "db_query.h"
#include "build_encounter.h"
#include "creature_db.h"
#include "slider.h"
#include "modal.h"
#include "player_db.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
static void AddStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void SaveNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void CancelNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void NewStatblockPage(void);

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void PlayerDatabaseWindow(AppState * state) {
    /* Player database window*/
    
    CLAY(CLAY_ID("PlayerDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("PlayerDBSidebar"), {SidebarLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("PlayerDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, &WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                if (WindowState == NEW_STATBLOCK_SCREEN) {
                    CLAY(CLAY_ID("PlayerDBSaveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Save"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                        // Clay_OnHover(SaveNewStatBlockCallback, &WindowState);
                    };
                    CLAY(CLAY_ID("PlayerDBCancelButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Cancel"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                        Clay_OnHover(CancelNewStatBlockCallback, &WindowState);
                    };
                }
                else {
                    CLAY(CLAY_ID("PlayerDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        // Clay_OnHover(AddStatBlockCallback, &WindowState);
                        CLAY_TEXT(CLAY_STRING("Add New"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                    CLAY(CLAY_ID("PlayerDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Remove Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                    CLAY(CLAY_ID("PlayerDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Edit Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                }
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

                CLAY(CLAY_ID("PlayerDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, &WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Name Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, &WindowState);
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        
        CLAY(CLAY_ID("PlayerDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            if (WindowState == NEW_STATBLOCK_SCREEN) {
                    NewStatblockPage();
            }
            else {
                CLAY(CLAY_ID("PlayerHeaderContainer"), {
                    CreatureButtonContainerLayoutConfig,
                    .backgroundColor = COLOR_TRANSPARENT,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                    .clip = {true, true, Clay_GetScrollOffset()}
                }) {
                    for (int i = 0; i < TotalPlayers; i++) {
                        if (HeadersToShow[i] != -1) {
                            MakePlayerHeader(i, CREATURE_DB_SCREEN);
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


void MakePlayerHeader(int i, int CallingWindow) {
    CLAY(CLAY_IDI("PlayerHeader", i), {
        PlayerButtonLayoutConfig,
        .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("PlayerHeader", i).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
    }) {

        CLAY_AUTO_ID({NameContainerLayoutConfig}){
            CLAY_TEXT(DBPlayerPageHeaders[i].PlayerName, CLAY_TEXT_CONFIG(ButtonTextConfig));
        };

        CLAY_AUTO_ID({PlayerACContainerLayoutConfig}) {
            /* change to be just size and type */
            CLAY_TEXT(CLAY_STRING("Armor Class"), CLAY_TEXT_CONFIG(ButtonTextConfig));
            CLAY_TEXT(DBPlayerPageHeaders[i].PlayerAC, CLAY_TEXT_CONFIG(ButtonTextConfig));
        };
        if (BUILD_ENCOUNTER_SCREEN == CallingWindow) {
            Clay_OnHover(PlayerBuildListCallback, &DBPlayerPageHeaders[i].PlayerName);
        }
    };
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */


static void AddStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = NEW_STATBLOCK_SCREEN;
    }
}

static void SaveNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        // SaveNewStatBlockToDB();
        // /* Saving encounter name before reset to display in modal */
        // SDL_strlcpy(gAppState->PlayerSaved, StatNameTextBox.StringToDisplay.chars, sizeof(gAppState->PlayerSaved));

        // RefreshDatabasePlayers();

        // WindowState = CREATURE_DB_SCREEN;
        // gAppState->ActiveScreen = CREATURE_DB_SCREEN;
        // ClearFocus();
        // ClearTextBoxes();
        // ResetVisiblePlayerHeaders();

        // /* Building modal message */
        // gAppState->IsModalOpen = true;
        // gAppState->ModalParentId = CLAY_ID("PlayerDBOuterContainer");
        // char temp[256];
        // SDL_snprintf(gAppState->ModalMessage, sizeof(gAppState->ModalMessage), "%s saved successfully.", gAppState->PlayerSaved);
    }
}

static void CancelNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->ActiveScreen = CREATURE_DB_SCREEN;
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->IsTextInputFocused = false;
        ClearTextBoxes();
        WindowState = CREATURE_DB_SCREEN;
    }
}