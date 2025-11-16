#include "main_window.h"
#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"
#include "db_query.h"
#include "build_encounter.h"
#include "creature_db.h"
#include "start_encounter.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */

static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

void PlayerDatabaseWindow(AppState * state);


/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */

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
            case MAIN_SCREEN:
            /* Main label */
            CLAY_TEXT(CLAY_STRING("GUIDNBATTER"), CLAY_TEXT_CONFIG(MainLabelTextConfig));
            /* Start button */
            CLAY(CLAY_ID("StartButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Start Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(StartEncounterButtonCallback, &WindowState);
            };

            /* Build button */
            CLAY(CLAY_ID("BuildButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Build Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig)); 
                Clay_OnHover(BuildEncounterButtonCallback, &WindowState);
            };

            /* Creature DB button */
            CLAY(CLAY_ID("CreatureDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Creature Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(CreatureDatabaseButtonCallback, &WindowState);
            };

            /* Player DB button */ 
            CLAY(CLAY_ID("PlayerDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Player Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(PlayerDatabaseButtonCallback, &WindowState);
                
            };
            break;

        case START_ENCOUNTER_SCREEN:
            StartEncounterWindow();
            break;
        
        case BUILD_ENCOUNTER_SCREEN:
            BuildEncounterWindow(state, MAIN_SCREEN);
            break;
        
        case CREATURE_DB_SCREEN:
            CreatureDatabaseWindow(state);
            break;

        case PLAYER_DB_SCREEN:
            PlayerDatabaseWindow(state);            
            break;

        case ADD_STAT_SCREEN:
            CreatureDatabaseWindow(state);
            break;

        default:
            break;
        }
    };

    return Clay_EndLayout();
}

void PlayerDatabaseWindow(AppState * state) {
    /* Player database window*/
    CLAY(CLAY_ID("PlayerDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)} ) {
        
        /* Sidebar for option buttons */
        CLAY(CLAY_ID("PlayerDBSidebar"), SidebarLayoutConfig) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("PlayerDBHomeButton"), {
                    MainScreenButtonLayoutConfig,
                    .backgroundColor = COLOR_BUTTON_GRAY,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
                }) {
                    Clay_OnHover(ReturnToMainScreenCallback, &WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBAddButton"), {
                    MainScreenButtonLayoutConfig,
                    .backgroundColor = COLOR_BUTTON_GRAY,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
                }) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBRemoveButton"), {
                    MainScreenButtonLayoutConfig,
                    .backgroundColor = COLOR_BUTTON_GRAY,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
                }) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBEditButton"), {
                    MainScreenButtonLayoutConfig,
                    .backgroundColor = COLOR_BUTTON_GRAY,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
                }) {
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
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("PlayerTextBox"), CurrentFocus, &DBPlayerSearch);      
                };

                CLAY(CLAY_ID("PlayerDBSearchButton"), {
                    MainScreenButtonLayoutConfig,
                    .backgroundColor = COLOR_BUTTON_GRAY,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
                }) {
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

/*========================================================================* 
 *  SECTION - Local Functions 
 *========================================================================*
 */
static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = START_ENCOUNTER_SCREEN;
    }
}

static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = BUILD_ENCOUNTER_SCREEN;
    }
}

static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        ScrollOffset = 0;
        WindowState = CREATURE_DB_SCREEN;        
    }
}

static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = PLAYER_DB_SCREEN;
    }
}