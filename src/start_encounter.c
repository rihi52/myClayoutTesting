#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"
#include "db_query.h"
#include "main_window.h"
#include "start_encounter.h"
#include "build_encounter.h"

/*========================================================================* 
 *  SECTION - Local defines
 *========================================================================* 
 */
#define BUILD_NEW_ENCOUNTER_SCREEN  1
#define SELECT_EXISTING_SCREEN      2

/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */
int StartEncounterState = 0;
int BuildingEncounter;

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
void SelectExistingScreen();
static void NewEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void SelectExistingEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

 /*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void StartEncounterWindow() {
    BuildingEncounter = 0;
    switch (StartEncounterState) {
        case MAIN_SCREEN:
            CLAY(CLAY_ID("StartEncounterOuterContainer"), {
                MainScreenLayoutConfig,
                .backgroundColor = COLOR_BLACK,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
            }) {
                CLAY(CLAY_ID("NewEncounter"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("New Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(NewEncounterButtonCallback, &StartEncounterState);
                };

                /* Build button */
                CLAY(CLAY_ID("SelectExisting"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Select Existing"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(ReturnToMainScreenCallback, &StartEncounterState);
                };
            };
            break;
        case BUILD_NEW_ENCOUNTER_SCREEN:
            BuildEncounterWindow(gAppState, START_ENCOUNTER_SCREEN);
            break;

        case START_NEW_ENCOUNTER_SCREEN:
            NewEncounterScreen();
            break;

        case SELECT_EXISTING_SCREEN:
            SelectExistingScreen();
            break;
        default:
            break;
    }
}

void NewEncounterScreen() {
    CLAY(CLAY_ID("NewEncounterScreen"), {
        MainScreenLayoutConfig,
        .backgroundColor = COLOR_BLACK,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }) {
        CLAY_TEXT(CLAY_STRING("New Encounter Screen - Work in Progress"), CLAY_TEXT_CONFIG(ButtonTextConfig));
    };
}

 /*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */
void SelectExistingScreen() {
    CLAY(CLAY_ID("SelectExistingScreen"), {
        MainScreenLayoutConfig,
        .backgroundColor = COLOR_BLACK,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }) {
        CLAY_TEXT(CLAY_STRING("Select Existing Screen - Work in Progress"), CLAY_TEXT_CONFIG(ButtonTextConfig));
    };
}

static void NewEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        StartEncounterState = BUILD_NEW_ENCOUNTER_SCREEN;
    }
}

static void SelectExistingEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        StartEncounterState = SELECT_EXISTING_SCREEN;
    }
}