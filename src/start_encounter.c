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
int ListStarted = 0;
DisplayListMember * NewMember = NULL;
DisplayListMember * Head = NULL;
DisplayListMember * Tail = NULL;

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
void SelectExistingScreen();
void FillCombatScreen(int position);
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

int CountNodes(DisplayListMember *head) {
    int count = 0;
    while (NULL != head) {
        count++;
        head = head->Next;
    }
    SDL_Log("check 1");
    return count;
}


void ListToArray(DisplayListMember *head, DisplayListMember **array) {
    int i = 0;
    while (head) {
        array[i++] = head;
        head = head->Next;
    }
}


int CompareInitiative(const void *a, const void *b) {
    DisplayListMember *A = *(DisplayListMember **)a;
    DisplayListMember *B = *(DisplayListMember **)b;

    return B->Initiative - A->Initiative;
}


void NewEncounterScreen() {
    /* Build linked list */
    if (0 == ListStarted) {
        Head = NULL;
        Tail = NULL;
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
        if ('\0' != BuildListMembers->name[i][0]) {
            NewMember = LookupCreatureForCombat(BuildListMembers->name[i], BuildListMembers[i].initiative);
            
            if (NewMember == NULL)
            {
                printf("Error: NewMember returned NULL.\n");
                return;
            }

            if (NULL == Head)
            {
                Head = NewMember;
            }
            else
            {
                Tail->Next = NewMember;
            }

            Tail = NewMember;

            //NewMember = NewMember->Next;
            Tail->Next = NULL;
        }

        }
        int count = CountNodes(Head);
        if (count == 0) {
            gAppState->SortedListArray = NULL;
            gAppState->SortedListCount = 0;
            return;
        }
        DisplayListMember **arr = malloc(sizeof(DisplayListMember*) * count);

        ListToArray(Head, arr);
        qsort(arr, count, sizeof(DisplayListMember*), CompareInitiative);
        // Save in global state
        gAppState->SortedListArray = arr;
        gAppState->SortedListCount = count;
        ListStarted = 1;
    }

    CLAY(CLAY_ID("NewEncounterScreen"), {
        LTRParentWindowLayoutConfig,
        .backgroundColor = COLOR_BLACK,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }) {
        CLAY(CLAY_ID("CombatWindow"), {
            TTBCombatWindow,
            .backgroundColor = COLOR_GRAY_BG,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
        }) {
            for (int i = 0; i < gAppState->SortedListCount; i++) {
                if (gAppState->SortedListArray[i] != NULL) {
                    FillCombatScreen(i);
                }
            }
        };
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

void FillCombatScreen(int position) {
    CLAY(CLAY_IDI("CombatantRow", position), {
        BuildWindowRow,
        .backgroundColor = COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
    }) {
        CLAY(CLAY_IDI("CombatantInitiative", position), {
            CombatWindowInitiativeContainer,
            .backgroundColor = COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
        }){
            SDL_itoa(gAppState->SortedListArray[position]->Initiative, gAppState->SortedListArray[position]->InitiativeBuffer, 10);
            Clay_String Initiative = {
                .chars = gAppState->SortedListArray[position]->InitiativeBuffer,
                .length = (int32_t)SDL_strlen(gAppState->SortedListArray[position]->InitiativeBuffer),
                .isStaticallyAllocated = true
            };
            CLAY_TEXT(Initiative, CLAY_TEXT_CONFIG(StatPageTextConfig));
        }
        CLAY(CLAY_IDI("CombatantName", position), {
            CombatWindowNameContainer,
            .backgroundColor = COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
        }){
            CLAY_TEXT(gAppState->SortedListArray[position]->Name, CLAY_TEXT_CONFIG(StatPageTextConfig));
        }
            if (gAppState->SortedListArray[position]->IsCreature) {
            CLAY(CLAY_IDI("CombatantArmorClass", position), {
                CombatWindowArmorClassContainer,
                .backgroundColor = COLOR_BUTTON_GRAY,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
            }){
                SDL_itoa(gAppState->SortedListArray[position]->ArmorClass, gAppState->SortedListArray[position]->ArmorClassBuffer, 10);
                Clay_String Initiative = {
                    .chars = gAppState->SortedListArray[position]->ArmorClassBuffer,
                    .length = (int32_t)SDL_strlen(gAppState->SortedListArray[position]->ArmorClassBuffer),
                    .isStaticallyAllocated = true
                };
                CLAY_TEXT(Initiative, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(CLAY_STRING("AC"), CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY(CLAY_IDI("CombatantHitPoints", position), {
                CombatWindowArmorClassContainer,
                .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantHitPoints", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
            }){
                SDL_itoa(gAppState->SortedListArray[position]->HitPoints, gAppState->SortedListArray[position]->HitPointsBuffer, 10);
                Clay_String Initiative = {
                    .chars = gAppState->SortedListArray[position]->HitPointsBuffer,
                    .length = (int32_t)SDL_strlen(gAppState->SortedListArray[position]->HitPointsBuffer),
                    .isStaticallyAllocated = true
                };
                if (ListStarted == 1 ) {
                    InitializeOneTextBox(&gAppState->SortedListArray[position]->HitPointsTextBox);
                    SDL_strlcpy(gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer, gAppState->SortedListArray[position]->HitPointsBuffer, sizeof(gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer));
                    ListStarted = 2;
                }
                
                
                Clay_OnHover(FocusWindowCallback, gAppState);
                uint32_t CurrentFocus = gAppState->focusedId.id;
                FocusAndWriteTextBox(CLAY_IDI("CombatantHitPoints", position), CurrentFocus, &gAppState->SortedListArray[position]->HitPointsTextBox);
                
                // CLAY_TEXT(Initiative, CLAY_TEXT_CONFIG(StatPageTextConfig));
                // CLAY_TEXT(CLAY_STRING("HP"), CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }
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