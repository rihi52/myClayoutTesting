#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"

#include "db_query.h"
#include "main_window.h"
#include "start_encounter.h"
#include "build_encounter.h"
#include "creature_db.h"

/*========================================================================* 
 *  SECTION - Local defines
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */
int StartEncounterState = ENCOUNTER_MAIN_SCREEN;
int BuildingEncounter;
int Turn = 0;
int ListStarted = 0;
int AddToEncounter = 0;
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
static void BuildMemberChain(int i);
static void RemoveExcessNodes(const char *name, int keepCount);

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void StartEncounterWindow() {
    BuildingEncounter = 0;
    switch (StartEncounterState) {
        case ENCOUNTER_MAIN_SCREEN:
            gAppState->ActiveScreen = ENCOUNTER_MAIN_SCREEN;
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
            gAppState->ActiveScreen = BUILD_NEW_ENCOUNTER_SCREEN;
            BuildEncounterWindow(gAppState, FIRST_START_ENCOUNTER_SCREEN);
            break;

        case START_NEW_ENCOUNTER_SCREEN:
            gAppState->ActiveScreen = START_NEW_ENCOUNTER_SCREEN;
            NewEncounterScreen();
            break;

        case START_NEW_WITH_STATS_SCREEN:
            gAppState->ActiveScreen = START_NEW_WITH_STATS_SCREEN;
            NewEncounterScreen();
            break;

        case SELECT_EXISTING_SCREEN:
            gAppState->ActiveScreen = SELECT_EXISTING_SCREEN;
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

static void BuildMemberChain(int i) {
    
    // Add a single member
    if (BuildListMembers[i].IsCreature) {
        NewMember = LookupCreatureForCombat(BuildListMembers[i].name, BuildListMembers[i].initiative, BuildListMembers[i].IsCreature);
    }
    else {
        NewMember = LookupPlayerForCombat(BuildListMembers[i].name, BuildListMembers[i].initiative, BuildListMembers[i].IsCreature);
    }                

    if (NewMember == NULL) {
        printf("Error: NewMember returned NULL.\n");
        return;
    }
    BuildListMembers[i].IsAdded = true;

    if (NULL == Head) {
        Head = NewMember;
    }
    else {
        Tail->Next = NewMember;
    }

    Tail = NewMember;

    Tail->Next = NULL;
    
}

static void RemoveExcessNodes(const char *name, int keepCount) {
    DisplayListMember *current = Head;
    DisplayListMember *prev = NULL;
    int kept = 0;
    while (current) {
        if (SDL_strcmp(current->Name.chars, name) == 0) {
            if (kept >= keepCount) {
                // Remove this node
                if (prev) {
                    prev->Next = current->Next;
                } else {
                    Head = current->Next;
                }
                if (current == Tail) {
                    Tail = prev;
                }
                // Move to next, but don't free the node
                current = current->Next;
                continue;
            } else {
                kept++;
            }
        }
        prev = current;
        current = current->Next;
    }
}

void NewEncounterScreen() {
    /* Build linked list */
    if (0 == ListStarted) {
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
            
            if (gAppState->ModifyCurrentEncounter) {
                int currentCount = 0;
                DisplayListMember *temp = Head;
                while (temp) {
                    if (SDL_strcmp(temp->Name.chars, BuildListMembers[i].name) == 0) {
                        currentCount++;
                    }
                    temp = temp->Next;
                }
                if (currentCount < BuildListMembers[i].Quantity) {
                    int toAdd = BuildListMembers[i].Quantity - currentCount;
                    for (int k = 0; k < toAdd; k++) {
                        BuildMemberChain(i);
                    }
                } else if (currentCount > BuildListMembers[i].Quantity) {
                    RemoveExcessNodes(BuildListMembers[i].name, BuildListMembers[i].Quantity);
                }
                BuildListMembers[i].IsAdded = true;
            } else {
                if (SDL_strlen(BuildListMembers[i].name) != 0 && !BuildListMembers[i].IsAdded) {
                    for (int m = 0; m < BuildListMembers[i].Quantity; m++) {
                        BuildMemberChain(i);
                    }
                }
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
        for (int i = 0; i < gAppState->SortedListCount; i++) {
            gAppState->SortedListArray[i]->TurnOrder = Turn;
            Turn++;
        }
        Turn = 0;
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
            CLAY(CLAY_ID("CombatSection"), {
                CombatWindowCombatSection,
                .backgroundColor = COLOR_GRAY_BG,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                .clip = {false, true, Clay_GetScrollOffset()}
            }) {
                for (int i = 0; i < gAppState->SortedListCount; i++) {
                    if (gAppState->SortedListArray[i] != NULL) {
                        FillCombatScreen(i);
                    }
                }
                Clay_OnHover(ClearFocusCallback, gAppState);
            }
            CLAY(CLAY_ID("ButtonSection"), {
                CombatWindowButtonSection,
                .backgroundColor = COLOR_GRAY_BG,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)
            }) {
                CLAY(CLAY_ID("AddInitiativeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    AddToEncounter = ADD_TO_ENCOUNTER;
                    Clay_OnHover(NewEncounterButtonCallback, &AddToEncounter);
                    CLAY_TEXT(CLAY_STRING("Add to Initiative"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("ExitEncounterButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, gAppState);
                    CLAY_TEXT(CLAY_STRING("Exit Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            }            
        };
        CLAY(CLAY_ID("CreatureHeaderContainer"), {  
            LTRParentWindowLayoutConfig,
            .backgroundColor = COLOR_TRANSPARENT,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
        }) {
            FillStats();
        }
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
        .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantRow", position).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
        .border = {
            .width = {.left = INPUT_BORDER_WIDTH_PX, .right = INPUT_BORDER_WIDTH_PX, .top = INPUT_BORDER_WIDTH_PX, .bottom = INPUT_BORDER_WIDTH_PX},
            .color = (Turn == gAppState->SortedListArray[position]->TurnOrder) ? COLOR_RED : COLOR_TRANSPARENT,
        }
    }) {
        CLAY(CLAY_IDI("CombatantInitiative", position), {
            CombatWindowInitiativeContainer,
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantRow", position).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
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
            .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantRow", position).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
        }){
            CLAY_TEXT(gAppState->SortedListArray[position]->Name, CLAY_TEXT_CONFIG(StatPageTextConfig));
        }
        CLAY(CLAY_IDI("CombatantArmorClass", position), {
                CombatWindowArmorClassContainer,
                .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantRow", position).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
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
            if (gAppState->SortedListArray[position]->IsCreature) {
            CLAY(CLAY_IDI("CombatantHitPoints", position), {
                CombatWindowArmorClassContainer,
                .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CombatantHitPoints", position).id) ? COLOR_BLACK : COLOR_BUTTON_GRAY,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                .border = {
                    .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                    .color = COLOR_WHITE
                }
            }){
                SDL_itoa(gAppState->SortedListArray[position]->HitPoints, gAppState->SortedListArray[position]->HitPointsBuffer, 10);
                if (true != gAppState->SortedListArray[position]->HitPointsTextBox.IsInitialized) {
                    InitializeOneTextBox(&gAppState->SortedListArray[position]->HitPointsTextBox);
                    SDL_strlcpy(gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer, gAppState->SortedListArray[position]->HitPointsBuffer, sizeof(gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer));
                }

                Clay_OnHover(FocusWindowCallback, gAppState);
                uint32_t CurrentFocus = gAppState->focusedId.id;
                
                // Show static text when not focused, editable text when focused
                if (CurrentFocus != CLAY_IDI("CombatantHitPoints", position).id) {
                    Clay_String HPDisplay = {
                        .chars = gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer,
                        .length = (int32_t)SDL_strlen(gAppState->SortedListArray[position]->HitPointsTextBox.TextBoxBuffer),
                        .isStaticallyAllocated = true
                    };
                    CLAY_TEXT(HPDisplay, CLAY_TEXT_CONFIG(StatPageTextConfig));
                } else {
                    FocusAndWriteTextBox(CLAY_IDI("CombatantHitPoints", position), CurrentFocus, &gAppState->SortedListArray[position]->HitPointsTextBox);
                }
            }
        }
        if (gAppState->SortedListArray[position]->IsCreature) {
            Clay_OnHover(FocusWindowAndCallStatBlockCallback, &gAppState->SortedListArray[position]->SqliteDbId);
        }
        else {
            Clay_OnHover(FocusWindowCallback, gAppState);
        }
    };
}

static void NewEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        if (ADD_TO_ENCOUNTER == *check) {
            AddToEncounter = 0;
            ListStarted = 0;
            Turn = 0;

        /* Free appstate sorted array of combat linked list */
            // if (gAppState->SortedListArray) {
            //     free(gAppState->SortedListArray);
            //     gAppState->SortedListArray = NULL;
            //     gAppState->SortedListCount = 0;
            //     StartEncounterState = MAIN_SCREEN;
            // }
            gAppState->ModifyCurrentEncounter = true;
        }
        StartEncounterState = BUILD_NEW_ENCOUNTER_SCREEN;
    }
}

static void SelectExistingEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        StartEncounterState = SELECT_EXISTING_SCREEN;
    }
}