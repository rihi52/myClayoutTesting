#include "clay.h"
#include "global.h"
#include "db_query.h"
#include "styles.h"

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
static void ClearTextBoxes();

int WindowState = 0;

char TextBuffer[MAX_TEXT] = {0};

const int FONT_ID_BODY_16 = 0;
const int FONT_ID_BODY_32 = 0;

int WindowWidth = 0;
int WindowHeight = 0;
uint16_t TotalCreatures = 0;

bool MouseDown;

TextBox BuildCreatureSearch;
TextBox BuildPlayerSearch;
TextBox DBCreatureSearch;
TextBox DBPlayerSearch;
TextBox BuildListMemberQuantity[BUILD_LIST_MAX] = {0};
TextBox BuildListMemberInitiative[BUILD_LIST_MAX] = {0};

Clay_String StatId = {0};
Clay_String StatName = {0};
Clay_String StatCr = {0};
Clay_String StatType = {0};
Clay_String StatSize = {0};

Clay_String StatArmorClass = {0};
Clay_String StatHitpointsAvg = {0};
Clay_String StatHitDice = {0};
Clay_String StatHitpointsRoll = {0};

Clay_String StatSpeedType = {0};
Clay_String StatSpeedWalk = {0};
Clay_String StatSpeedFly = {0};
Clay_String StatSpeedSwim = {0};
Clay_String StatSpeedClimb = {0};
Clay_String StatSpeedBurrow = {0};

Clay_String StatAlignment = {0};
Clay_String StatLegendary = {0};

Clay_String StatStr = {0};
Clay_String StatDex = {0};
Clay_String StatCon = {0};
Clay_String StatInt = {0};
Clay_String StatWis = {0};
Clay_String StatCha = {0};

Clay_String StatProfBonus = {0};

Clay_String StatThrowStr = {0};
Clay_String StatThrowDex = {0};
Clay_String StatThrowCon = {0};
Clay_String StatThrowInt = {0};
Clay_String StatThrowWis = {0};
Clay_String StatThrowCha = {0};

Clay_String StatSavingThrows = {0};

Clay_String StatSkills = {0};
Clay_String StatLanguages = {0};
Clay_String StatSenses = {0};

Clay_String StatRangeDarkvision = {0};
Clay_String StatRangeTremorsense = {0};
Clay_String StatRangeBlindsight = {0};
Clay_String StatRangeTruesight = {0};

Clay_String StatSpecialAbilityOne = {0};
Clay_String StatSpecialAbilityOneDesc = {0};
Clay_String StatSpecialAbilityTwo = {0};
Clay_String StatSpecialAbilityTwoDesc = {0};
Clay_String StatSpecialAbilityThree = {0};
Clay_String StatSpecialAbilityThreeDesc = {0};
Clay_String StatSpecialAbilityFour = {0};
Clay_String StatSpecialAbilityFourDesc = {0};

Clay_String StatAttack1 = {0};
Clay_String StatAttack1Desc = {0};
Clay_String StatAttack2 = {0};
Clay_String StatAttack2Desc = {0};
Clay_String StatAttack3 = {0};
Clay_String StatAttack3Desc = {0};
Clay_String StatAttack4 = {0};
Clay_String StatAttack4Desc = {0};

Clay_String StatAttack5 = {0};
Clay_String StatAttack5Desc = {0};

Clay_String StatAttack6 = {0};
Clay_String StatAttack6Desc = {0};

Clay_String StatActionLeg = {0};
Clay_String StatActionLeg1 = {0};
Clay_String StatActionLeg1Desc = {0};
Clay_String StatActionLeg2 = {0};
Clay_String StatActionLeg2Desc = {0};
Clay_String StatActionLeg3 = {0};
Clay_String StatActionLeg3Desc = {0};

Clay_String StatActionLair = {0};
Clay_String StatActionLair1 = {0};
Clay_String StatActionLair1Desc = {0};
Clay_String StatActionLair2 = {0};
Clay_String StatActionLair2Desc = {0};
Clay_String StatActionLair3 = {0};
Clay_String StatActionLair3Desc = {0};

Clay_String StatRegionalEffect = {0};
Clay_String StatRegionalEffect1 = {0};
Clay_String StatRegionalEffect2 = {0};
Clay_String StatRegionalEffect3 = {0};
Clay_String StatEndRegionalEffect = {0};

Clay_String StatEnvironment = {0};

Clay_String StatBa1 = {0};
Clay_String StatBa1Desc = {0};
Clay_String StatBa2 = {0};
Clay_String StatBa2Desc = {0};
Clay_String StatBa3 = {0};
Clay_String StatBa3Desc = {0};
Clay_String StatBa4 = {0};
Clay_String StatBa4Desc = {0};

Clay_String StatReaction1 = {0};
Clay_String StatReaction1Desc = {0};
Clay_String StatReaction2 = {0};
Clay_String StatReaction2Desc = {0};
Clay_String StatReaction3 = {0};
Clay_String StatReaction3Desc = {0};

Clay_String StatVillAction = {0};
Clay_String StatVillAction1 = {0};
Clay_String StatVillAction1Desc = {0};
Clay_String StatVillAction2 = {0};
Clay_String StatVillAction2Desc = {0};
Clay_String StatVillAction3 = {0};
Clay_String StatVillAction3Desc = {0};

Clay_String StatUtilitySpells = {0};
Clay_String StatUtilitySpellsList = {0};

Clay_String StatFeature1 = {0};
Clay_String StatFeature1Desc = {0};
Clay_String StatFeature2 = {0};
Clay_String StatFeature2Desc = {0};
Clay_String StatFeature3 = {0};
Clay_String StatFeature3Desc = {0};
Clay_String StatFeature4 = {0};
Clay_String StatFeature4Desc = {0};
Clay_String StatFeature5 = {0};
Clay_String StatFeature5Desc = {0};

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
/* Helpers */
void ModifyTextBoxText(TextBox * TextToModify, uint32_t CopyOrWrite) {
    
    
    if (COPY_TEXT == CopyOrWrite) {
        memset(TextBuffer, 0, sizeof(TextBuffer));
        SDL_strlcpy(TextBuffer, TextToModify->TextBoxBuffer, MAX_TEXT);
    }
    else {
        SDL_strlcpy(TextToModify->TextBoxBuffer, TextBuffer, sizeof(TextBuffer));
        
    }
    TextToModify->StringToDisplay.length = SDL_strlen(TextToModify->TextBoxBuffer);
}

void FocusAndWriteTextBox(Clay_ElementId IdToFocus, uint32_t CurrentFocus, TextBox * TextToModify) {
    if (gAppState->focusedId.id == IdToFocus.id) {
        if(PreviousFocusId != CurrentFocus) {
            ModifyTextBoxText(TextToModify, COPY_TEXT);
        }
        else {
            ModifyTextBoxText(TextToModify, WRITE_TEXT);
        }
    }
    CLAY_TEXT(TextToModify->StringToDisplay, CLAY_TEXT_CONFIG(InputTextConfig));
}

void InitializeOneTextBox(TextBox * TextBoxToInit) {
    SDL_memset(TextBoxToInit->TextBoxBuffer, 0, sizeof(TextBoxToInit->TextBoxBuffer));
    TextBoxToInit->StringToDisplay.chars = TextBoxToInit->TextBoxBuffer;
    TextBoxToInit->StringToDisplay.length = 0;
    TextBoxToInit->StringToDisplay.isStaticallyAllocated = false;
    TextBoxToInit->IsInitialized = true;
}

void InitializeTextBoxes() {
    SDL_memset(BuildCreatureSearch.TextBoxBuffer, 0, sizeof(BuildCreatureSearch.TextBoxBuffer));
    BuildCreatureSearch.StringToDisplay.chars = BuildCreatureSearch.TextBoxBuffer;
    BuildCreatureSearch.StringToDisplay.length = 0;
    BuildCreatureSearch.StringToDisplay.isStaticallyAllocated = false;
    BuildCreatureSearch.IsInitialized = true;

    SDL_memset(BuildPlayerSearch.TextBoxBuffer, 0, sizeof(BuildPlayerSearch.TextBoxBuffer));
    BuildPlayerSearch.StringToDisplay.chars = BuildPlayerSearch.TextBoxBuffer;
    BuildPlayerSearch.StringToDisplay.length = 0;
    BuildPlayerSearch.StringToDisplay.isStaticallyAllocated = false;
    BuildPlayerSearch.IsInitialized = true;

    SDL_memset(DBCreatureSearch.TextBoxBuffer, 0, sizeof(DBCreatureSearch.TextBoxBuffer));
    DBCreatureSearch.StringToDisplay.chars = DBCreatureSearch.TextBoxBuffer;
    DBCreatureSearch.StringToDisplay.length = 0;
    DBCreatureSearch.StringToDisplay.isStaticallyAllocated = false;
    DBCreatureSearch.IsInitialized = true;

    SDL_memset(DBPlayerSearch.TextBoxBuffer, 0, sizeof(DBPlayerSearch.TextBoxBuffer));
    DBPlayerSearch.StringToDisplay.chars = DBPlayerSearch.TextBoxBuffer;
    DBPlayerSearch.StringToDisplay.length = 0;
    DBPlayerSearch.StringToDisplay.isStaticallyAllocated = false;
    DBPlayerSearch.IsInitialized = true;

    for (int i = 0; i < BUILD_LIST_MAX; i++){
        SDL_memset(BuildListMemberQuantity[i].TextBoxBuffer, 0, sizeof(BuildListMemberQuantity[i].TextBoxBuffer));
        BuildListMemberQuantity[i].StringToDisplay.chars = BuildListMemberQuantity[i].TextBoxBuffer;
        BuildListMemberQuantity[i].StringToDisplay.length = 0;
        BuildListMemberQuantity[i].StringToDisplay.isStaticallyAllocated = false;
        BuildListMemberQuantity[i].IsInitialized = true;

        SDL_memset(BuildListMemberInitiative[i].TextBoxBuffer, 0, sizeof(BuildListMemberInitiative[i].TextBoxBuffer));
        BuildListMemberInitiative[i].StringToDisplay.chars = BuildListMemberInitiative[i].TextBoxBuffer;
        BuildListMemberInitiative[i].StringToDisplay.length = 0;
        BuildListMemberInitiative[i].StringToDisplay.isStaticallyAllocated = false;
        BuildListMemberInitiative[i].IsInitialized = true;
    }
    
}

#include "global.h"

/* Initialize every Clay_String in a StatBlock to an empty string. */
void InitStatBlock(StatBlock *sb)
{
    if (!sb) return;

    sb->StatId = MakeClayString("");
    sb->StatName = MakeClayString("");
    sb->StatCr = MakeClayString("");
    sb->StatType = MakeClayString("");
    sb->StatSize = MakeClayString("");

    sb->StatArmorClass = MakeClayString("");
    sb->StatHitpointsAvg = MakeClayString("");
    sb->StatHitDice = MakeClayString("");
    sb->StatHitpointsRoll = MakeClayString("");

    sb->StatSpeedType = MakeClayString("");
    sb->StatSpeedWalk = MakeClayString("");
    sb->StatSpeedFly = MakeClayString("");
    sb->StatSpeedSwim = MakeClayString("");
    sb->StatSpeedClimb = MakeClayString("");
    sb->StatSpeedBurrow = MakeClayString("");

    sb->StatAlignment = MakeClayString("");
    sb->StatLegendary = MakeClayString("");

    sb->StatStr = MakeClayString("");
    sb->StatDex = MakeClayString("");
    sb->StatCon = MakeClayString("");
    sb->StatInt = MakeClayString("");
    sb->StatWis = MakeClayString("");
    sb->StatCha = MakeClayString("");

    sb->StatProfBonus = MakeClayString("");

    sb->StatThrowStr = MakeClayString("");
    sb->StatThrowDex = MakeClayString("");
    sb->StatThrowCon = MakeClayString("");
    sb->StatThrowInt = MakeClayString("");
    sb->StatThrowWis = MakeClayString("");
    sb->StatThrowCha = MakeClayString("");

    sb->StatSavingThrows = MakeClayString("");

    sb->StatSkills = MakeClayString("");
    sb->StatLanguages = MakeClayString("");
    sb->StatSenses = MakeClayString("");

    sb->StatRangeDarkvision = MakeClayString("");
    sb->StatRangeTremorsense = MakeClayString("");
    sb->StatRangeBlindsight = MakeClayString("");
    sb->StatRangeTruesight = MakeClayString("");

    sb->StatSpecialAbilityOne = MakeClayString("");
    sb->StatSpecialAbilityOneDesc = MakeClayString("");
    sb->StatSpecialAbilityTwo = MakeClayString("");
    sb->StatSpecialAbilityTwoDesc = MakeClayString("");
    sb->StatSpecialAbilityThree = MakeClayString("");
    sb->StatSpecialAbilityThreeDesc = MakeClayString("");
    sb->StatSpecialAbilityFour = MakeClayString("");
    sb->StatSpecialAbilityFourDesc = MakeClayString("");

    sb->StatAttack1 = MakeClayString("");
    sb->StatAttack1Desc = MakeClayString("");
    sb->StatAttack2 = MakeClayString("");
    sb->StatAttack2Desc = MakeClayString("");
    sb->StatAttack3 = MakeClayString("");
    sb->StatAttack3Desc = MakeClayString("");
    sb->StatAttack4 = MakeClayString("");
    sb->StatAttack4Desc = MakeClayString("");

    sb->StatAttack5 = MakeClayString("");
    sb->StatAttack5Desc = MakeClayString("");

    sb->StatAttack6 = MakeClayString("");
    sb->StatAttack6Desc = MakeClayString("");

    sb->StatActionLeg = MakeClayString("");
    sb->StatActionLeg1 = MakeClayString("");
    sb->StatActionLeg1Desc = MakeClayString("");
    sb->StatActionLeg2 = MakeClayString("");
    sb->StatActionLeg2Desc = MakeClayString("");
    sb->StatActionLeg3 = MakeClayString("");
    sb->StatActionLeg3Desc = MakeClayString("");

    sb->StatActionLair = MakeClayString("");
    sb->StatActionLair1 = MakeClayString("");
    sb->StatActionLair1Desc = MakeClayString("");
    sb->StatActionLair2 = MakeClayString("");
    sb->StatActionLair2Desc = MakeClayString("");
    sb->StatActionLair3 = MakeClayString("");
    sb->StatActionLair3Desc = MakeClayString("");

    sb->StatRegionalEffect = MakeClayString("");
    sb->StatRegionalEffect1 = MakeClayString("");
    sb->StatRegionalEffect2 = MakeClayString("");
    sb->StatRegionalEffect3 = MakeClayString("");
    sb->StatEndRegionalEffect = MakeClayString("");

    sb->StatEnvironment = MakeClayString("");

    sb->StatBa1 = MakeClayString("");
    sb->StatBa1Desc = MakeClayString("");
    sb->StatBa2 = MakeClayString("");
    sb->StatBa2Desc = MakeClayString("");
    sb->StatBa3 = MakeClayString("");
    sb->StatBa3Desc = MakeClayString("");
    sb->StatBa4 = MakeClayString("");
    sb->StatBa4Desc = MakeClayString("");

    sb->StatReaction1 = MakeClayString("");
    sb->StatReaction1Desc = MakeClayString("");
    sb->StatReaction2 = MakeClayString("");
    sb->StatReaction2Desc = MakeClayString("");
    sb->StatReaction3 = MakeClayString("");
    sb->StatReaction3Desc = MakeClayString("");

    sb->StatVillAction = MakeClayString("");
    sb->StatVillAction1 = MakeClayString("");
    sb->StatVillAction1Desc = MakeClayString("");
    sb->StatVillAction2 = MakeClayString("");
    sb->StatVillAction2Desc = MakeClayString("");
    sb->StatVillAction3 = MakeClayString("");
    sb->StatVillAction3Desc = MakeClayString("");

    sb->StatUtilitySpells = MakeClayString("");
    sb->StatUtilitySpellsList = MakeClayString("");

    sb->StatFeature1 = MakeClayString("");
    sb->StatFeature1Desc = MakeClayString("");
    sb->StatFeature2 = MakeClayString("");
    sb->StatFeature2Desc = MakeClayString("");
    sb->StatFeature3 = MakeClayString("");
    sb->StatFeature3Desc = MakeClayString("");
    sb->StatFeature4 = MakeClayString("");
    sb->StatFeature4Desc = MakeClayString("");
    sb->StatFeature5 = MakeClayString("");
    sb->StatFeature5Desc = MakeClayString("");
}

#include "global.h"

/* Free every Clay_String in a StatBlock using ClearClayString. */
void FreeStatBlock(StatBlock *sb)
{
    if (!sb) return;

    ClearClayString(&sb->StatId);
    ClearClayString(&sb->StatName);
    ClearClayString(&sb->StatCr);
    ClearClayString(&sb->StatType);
    ClearClayString(&sb->StatSize);

    ClearClayString(&sb->StatArmorClass);
    ClearClayString(&sb->StatHitpointsAvg);
    ClearClayString(&sb->StatHitDice);
    ClearClayString(&sb->StatHitpointsRoll);

    ClearClayString(&sb->StatSpeedType);
    ClearClayString(&sb->StatSpeedWalk);
    ClearClayString(&sb->StatSpeedFly);
    ClearClayString(&sb->StatSpeedSwim);
    ClearClayString(&sb->StatSpeedClimb);
    ClearClayString(&sb->StatSpeedBurrow);

    ClearClayString(&sb->StatAlignment);
    ClearClayString(&sb->StatLegendary);

    ClearClayString(&sb->StatStr);
    ClearClayString(&sb->StatDex);
    ClearClayString(&sb->StatCon);
    ClearClayString(&sb->StatInt);
    ClearClayString(&sb->StatWis);
    ClearClayString(&sb->StatCha);

    ClearClayString(&sb->StatProfBonus);

    ClearClayString(&sb->StatThrowStr);
    ClearClayString(&sb->StatThrowDex);
    ClearClayString(&sb->StatThrowCon);
    ClearClayString(&sb->StatThrowInt);
    ClearClayString(&sb->StatThrowWis);
    ClearClayString(&sb->StatThrowCha);

    ClearClayString(&sb->StatSavingThrows);

    ClearClayString(&sb->StatSkills);
    ClearClayString(&sb->StatLanguages);
    ClearClayString(&sb->StatSenses);

    ClearClayString(&sb->StatRangeDarkvision);
    ClearClayString(&sb->StatRangeTremorsense);
    ClearClayString(&sb->StatRangeBlindsight);
    ClearClayString(&sb->StatRangeTruesight);

    ClearClayString(&sb->StatSpecialAbilityOne);
    ClearClayString(&sb->StatSpecialAbilityOneDesc);
    ClearClayString(&sb->StatSpecialAbilityTwo);
    ClearClayString(&sb->StatSpecialAbilityTwoDesc);
    ClearClayString(&sb->StatSpecialAbilityThree);
    ClearClayString(&sb->StatSpecialAbilityThreeDesc);
    ClearClayString(&sb->StatSpecialAbilityFour);
    ClearClayString(&sb->StatSpecialAbilityFourDesc);

    ClearClayString(&sb->StatAttack1);
    ClearClayString(&sb->StatAttack1Desc);
    ClearClayString(&sb->StatAttack2);
    ClearClayString(&sb->StatAttack2Desc);
    ClearClayString(&sb->StatAttack3);
    ClearClayString(&sb->StatAttack3Desc);
    ClearClayString(&sb->StatAttack4);
    ClearClayString(&sb->StatAttack4Desc);

    ClearClayString(&sb->StatAttack5);
    ClearClayString(&sb->StatAttack5Desc);

    ClearClayString(&sb->StatAttack6);
    ClearClayString(&sb->StatAttack6Desc);

    ClearClayString(&sb->StatActionLeg);
    ClearClayString(&sb->StatActionLeg1);
    ClearClayString(&sb->StatActionLeg1Desc);
    ClearClayString(&sb->StatActionLeg2);
    ClearClayString(&sb->StatActionLeg2Desc);
    ClearClayString(&sb->StatActionLeg3);
    ClearClayString(&sb->StatActionLeg3Desc);

    ClearClayString(&sb->StatActionLair);
    ClearClayString(&sb->StatActionLair1);
    ClearClayString(&sb->StatActionLair1Desc);
    ClearClayString(&sb->StatActionLair2);
    ClearClayString(&sb->StatActionLair2Desc);
    ClearClayString(&sb->StatActionLair3);
    ClearClayString(&sb->StatActionLair3Desc);

    ClearClayString(&sb->StatRegionalEffect);
    ClearClayString(&sb->StatRegionalEffect1);
    ClearClayString(&sb->StatRegionalEffect2);
    ClearClayString(&sb->StatRegionalEffect3);
    ClearClayString(&sb->StatEndRegionalEffect);

    ClearClayString(&sb->StatEnvironment);

    ClearClayString(&sb->StatBa1);
    ClearClayString(&sb->StatBa1Desc);
    ClearClayString(&sb->StatBa2);
    ClearClayString(&sb->StatBa2Desc);
    ClearClayString(&sb->StatBa3);
    ClearClayString(&sb->StatBa3Desc);
    ClearClayString(&sb->StatBa4);
    ClearClayString(&sb->StatBa4Desc);

    ClearClayString(&sb->StatReaction1);
    ClearClayString(&sb->StatReaction1Desc);
    ClearClayString(&sb->StatReaction2);
    ClearClayString(&sb->StatReaction2Desc);
    ClearClayString(&sb->StatReaction3);
    ClearClayString(&sb->StatReaction3Desc);

    ClearClayString(&sb->StatVillAction);
    ClearClayString(&sb->StatVillAction1);
    ClearClayString(&sb->StatVillAction1Desc);
    ClearClayString(&sb->StatVillAction2);
    ClearClayString(&sb->StatVillAction2Desc);
    ClearClayString(&sb->StatVillAction3);
    ClearClayString(&sb->StatVillAction3Desc);

    ClearClayString(&sb->StatUtilitySpells);
    ClearClayString(&sb->StatUtilitySpellsList);

    ClearClayString(&sb->StatFeature1);
    ClearClayString(&sb->StatFeature1Desc);
    ClearClayString(&sb->StatFeature2);
    ClearClayString(&sb->StatFeature2Desc);
    ClearClayString(&sb->StatFeature3);
    ClearClayString(&sb->StatFeature3Desc);
    ClearClayString(&sb->StatFeature4);
    ClearClayString(&sb->StatFeature4Desc);
    ClearClayString(&sb->StatFeature5);
    ClearClayString(&sb->StatFeature5Desc);
}

void FreeDisplayList(DisplayListMember *head) {
    DisplayListMember *curr = head;

    while (curr) {
        DisplayListMember *next = curr->Next;

        if (!curr->Name.isStaticallyAllocated && curr->Name.chars) {
            ClearClayString(&curr->Name);
        }

        SDL_free(curr);
        curr = next;
    }
}


/* Callbacks*/

void FocusWindowCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    AppState *state = userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->focusedId = elementId;
    }
}

void FocusWindowAndCallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->focusedId = elementId;
        int * GivenId = (int *) userData;
        int LookUpId = *GivenId - 1;
        LookUpCreatureStats(LookUpId);
    }
}

void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        /* Clear focused ID */
        gAppState->focusedId = CLAY_ID("NULL");

        /* Reset all global text boxes */
        ClearTextBoxes();

        /* Clear internal windowstate tracking */
        WindowState = MAIN_SCREEN;

        /* Reset creatures to be shown in creature database screen */
        for (int i = 0; i <TotalCreatures; i++) {
            HeadersToShow[i] = i;
        }

        /* Free combat linked list */
        if (Head != NULL) {
            FreeDisplayList(Head);
            NewMember = NULL;
            Head = NULL;
            Tail = NULL;
        }

        /* Free appstate sorted array of combat linked list */
        if (gAppState->SortedListArray) {
            free(gAppState->SortedListArray);
            gAppState->SortedListArray = NULL;
            gAppState->SortedListCount = 0;
            StartEncounterState = MAIN_SCREEN;
        }
        
        /* Reset all buildlist data */
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
            BuildListMemberQuantity[i].IsInitialized = false;
            BuildListMemberInitiative[i].IsInitialized = false;

            BuildListMembers[i].initiative = 0;
            BuildListMembers[i].Quantity = 0;
            SDL_memset(BuildListMembers[i].name, 0, sizeof(BuildListMembers[i].name));
            BuildListMembers[i].IsCreature = false;
        }
        memset(&BuildListMembers, 0, sizeof(BuildListMembers));

        /* Reset list to an unstarted state */
        ListStarted = 0;

        /* Reset start encounter window to the initial two options */
        StartEncounterState = MAIN_SCREEN;
    }
}

void SearchButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        char * SearchText = &TextBuffer[0];
        SearchCreatureNames(SearchText);
    }
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */
static void ClearTextBoxes() {
    SDL_memset(TextBuffer, 0, sizeof(TextBuffer));

    SDL_memset(BuildCreatureSearch.TextBoxBuffer, 0, sizeof(BuildCreatureSearch.TextBoxBuffer));
    BuildCreatureSearch.StringToDisplay.length = 0;

    SDL_memset(BuildPlayerSearch.TextBoxBuffer, 0, sizeof(BuildPlayerSearch.TextBoxBuffer));
    BuildPlayerSearch.StringToDisplay.length = 0;

    SDL_memset(DBCreatureSearch.TextBoxBuffer, 0, sizeof(DBCreatureSearch.TextBoxBuffer));
    DBCreatureSearch.StringToDisplay.length = 0;

    SDL_memset(DBPlayerSearch.TextBoxBuffer, 0, sizeof(DBPlayerSearch.TextBoxBuffer));
    DBPlayerSearch.StringToDisplay.length = 0;

    for (int i = 0; i < BUILD_LIST_MAX; i++){
        SDL_memset(BuildListMemberQuantity[i].TextBoxBuffer, 0, sizeof(BuildListMemberQuantity[i].TextBoxBuffer));
        BuildListMemberQuantity[i].StringToDisplay.length = 0;
        BuildListMemberQuantity[i].IsInitialized = false;

        SDL_memset(BuildListMemberInitiative[i].TextBoxBuffer, 0, sizeof(BuildListMemberInitiative[i].TextBoxBuffer));
        BuildListMemberInitiative[i].StringToDisplay.length = 0;
        BuildListMemberInitiative[i].IsInitialized = false;
    }
}