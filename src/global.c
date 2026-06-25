#include "clay.h"
#include "global.h"
#include "db_query.h"
#include "styles.h"
#include "start_encounter.h"

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */

int WindowState = 0;

char TextBuffer[MAX_TEXT] = {0};

const int FONT_ID_BODY_16 = 0;
const int FONT_ID_BODY_32 = 0;

int WindowWidth = 0;
int WindowHeight = 0;
uint16_t TotalCreatures = 0;
uint16_t TotalPlayers = 0;
uint16_t TotalEncounters = 0;

bool MouseDown;

TextBox EncounterName;
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
    
    if (!TextToModify || !TextToModify->IsInitialized) {
        return;
    }
    
    if (COPY_TEXT == CopyOrWrite) {
        memset(TextBuffer, 0, MAX_TEXT);
        SDL_strlcpy(TextBuffer, TextToModify->TextBoxBuffer, MAX_TEXT);
    }
    else {
        SDL_strlcpy(TextToModify->TextBoxBuffer, TextBuffer, MAX_TEXT);
        
    }
    TextToModify->StringToDisplay.length = SDL_strlen(TextToModify->TextBoxBuffer);
}

void FocusAndWriteTextBox(Clay_ElementId IdToFocus, uint32_t CurrentFocus, TextBox * TextToModify) {

    if (!TextToModify || !TextToModify->IsInitialized) {
        return;
    }
    
    if (gAppState->focusedId.id == IdToFocus.id) {
        gAppState->IsTextInputFocused = true;
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

    SDL_memset(EncounterName.TextBoxBuffer, 0, sizeof(EncounterName.TextBoxBuffer));
    EncounterName.StringToDisplay.chars = EncounterName.TextBoxBuffer;
    EncounterName.StringToDisplay.length = 0;
    EncounterName.StringToDisplay.isStaticallyAllocated = false;
    EncounterName.IsInitialized = true;

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

    // --- Statblock TextBoxes ---

    // Core Info
    SDL_memset(StatNameTextBox.TextBoxBuffer, 0, sizeof(StatNameTextBox.TextBoxBuffer));
    StatNameTextBox.StringToDisplay.chars = StatNameTextBox.TextBoxBuffer;
    StatNameTextBox.StringToDisplay.length = 0;
    StatNameTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatNameTextBox.IsInitialized = true;

    SDL_memset(StatCrTextBox.TextBoxBuffer, 0, sizeof(StatCrTextBox.TextBoxBuffer));
    StatCrTextBox.StringToDisplay.chars = StatCrTextBox.TextBoxBuffer;
    StatCrTextBox.StringToDisplay.length = 0;
    StatCrTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatCrTextBox.IsInitialized = true;

    SDL_memset(StatTypeTextBox.TextBoxBuffer, 0, sizeof(StatTypeTextBox.TextBoxBuffer));
    StatTypeTextBox.StringToDisplay.chars = StatTypeTextBox.TextBoxBuffer;
    StatTypeTextBox.StringToDisplay.length = 0;
    StatTypeTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatTypeTextBox.IsInitialized = true;

    SDL_memset(StatSizeTextBox.TextBoxBuffer, 0, sizeof(StatSizeTextBox.TextBoxBuffer));
    StatSizeTextBox.StringToDisplay.chars = StatSizeTextBox.TextBoxBuffer;
    StatSizeTextBox.StringToDisplay.length = 0;
    StatSizeTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSizeTextBox.IsInitialized = true;

    SDL_memset(StatArmorClassTextBox.TextBoxBuffer, 0, sizeof(StatArmorClassTextBox.TextBoxBuffer));
    StatArmorClassTextBox.StringToDisplay.chars = StatArmorClassTextBox.TextBoxBuffer;
    StatArmorClassTextBox.StringToDisplay.length = 0;
    StatArmorClassTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatArmorClassTextBox.IsInitialized = true;

    SDL_memset(StatHitpointsAvgTextBox.TextBoxBuffer, 0, sizeof(StatHitpointsAvgTextBox.TextBoxBuffer));
    StatHitpointsAvgTextBox.StringToDisplay.chars = StatHitpointsAvgTextBox.TextBoxBuffer;
    StatHitpointsAvgTextBox.StringToDisplay.length = 0;
    StatHitpointsAvgTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatHitpointsAvgTextBox.IsInitialized = true;

    SDL_memset(StatHitDiceTextBox.TextBoxBuffer, 0, sizeof(StatHitDiceTextBox.TextBoxBuffer));
    StatHitDiceTextBox.StringToDisplay.chars = StatHitDiceTextBox.TextBoxBuffer;
    StatHitDiceTextBox.StringToDisplay.length = 0;
    StatHitDiceTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatHitDiceTextBox.IsInitialized = true;

    SDL_memset(StatHitpointsRollTextBox.TextBoxBuffer, 0, sizeof(StatHitpointsRollTextBox.TextBoxBuffer));
    StatHitpointsRollTextBox.StringToDisplay.chars = StatHitpointsRollTextBox.TextBoxBuffer;
    StatHitpointsRollTextBox.StringToDisplay.length = 0;
    StatHitpointsRollTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatHitpointsRollTextBox.IsInitialized = true;

    // Ability Scores
    SDL_memset(StatStrTextBox.TextBoxBuffer, 0, sizeof(StatStrTextBox.TextBoxBuffer));
    StatStrTextBox.StringToDisplay.chars = StatStrTextBox.TextBoxBuffer;
    StatStrTextBox.StringToDisplay.length = 0;
    StatStrTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatStrTextBox.IsInitialized = true;

    SDL_memset(StatDexTextBox.TextBoxBuffer, 0, sizeof(StatDexTextBox.TextBoxBuffer));
    StatDexTextBox.StringToDisplay.chars = StatDexTextBox.TextBoxBuffer;
    StatDexTextBox.StringToDisplay.length = 0;
    StatDexTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatDexTextBox.IsInitialized = true;

    SDL_memset(StatConTextBox.TextBoxBuffer, 0, sizeof(StatConTextBox.TextBoxBuffer));
    StatConTextBox.StringToDisplay.chars = StatConTextBox.TextBoxBuffer;
    StatConTextBox.StringToDisplay.length = 0;
    StatConTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatConTextBox.IsInitialized = true;

    SDL_memset(StatIntTextBox.TextBoxBuffer, 0, sizeof(StatIntTextBox.TextBoxBuffer));
    StatIntTextBox.StringToDisplay.chars = StatIntTextBox.TextBoxBuffer;
    StatIntTextBox.StringToDisplay.length = 0;
    StatIntTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatIntTextBox.IsInitialized = true;

    SDL_memset(StatWisTextBox.TextBoxBuffer, 0, sizeof(StatWisTextBox.TextBoxBuffer));
    StatWisTextBox.StringToDisplay.chars = StatWisTextBox.TextBoxBuffer;
    StatWisTextBox.StringToDisplay.length = 0;
    StatWisTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatWisTextBox.IsInitialized = true;

    SDL_memset(StatChaTextBox.TextBoxBuffer, 0, sizeof(StatChaTextBox.TextBoxBuffer));
    StatChaTextBox.StringToDisplay.chars = StatChaTextBox.TextBoxBuffer;
    StatChaTextBox.StringToDisplay.length = 0;
    StatChaTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatChaTextBox.IsInitialized = true;

    SDL_memset(StatProfBonusTextBox.TextBoxBuffer, 0, sizeof(StatProfBonusTextBox.TextBoxBuffer));
    StatProfBonusTextBox.StringToDisplay.chars = StatProfBonusTextBox.TextBoxBuffer;
    StatProfBonusTextBox.StringToDisplay.length = 0;
    StatProfBonusTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatProfBonusTextBox.IsInitialized = true;

    // Saving Throws
    SDL_memset(StatThrowStrTextBox.TextBoxBuffer, 0, sizeof(StatThrowStrTextBox.TextBoxBuffer));
    StatThrowStrTextBox.StringToDisplay.chars = StatThrowStrTextBox.TextBoxBuffer;
    StatThrowStrTextBox.StringToDisplay.length = 0;
    StatThrowStrTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowStrTextBox.IsInitialized = true;

    SDL_memset(StatThrowDexTextBox.TextBoxBuffer, 0, sizeof(StatThrowDexTextBox.TextBoxBuffer));
    StatThrowDexTextBox.StringToDisplay.chars = StatThrowDexTextBox.TextBoxBuffer;
    StatThrowDexTextBox.StringToDisplay.length = 0;
    StatThrowDexTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowDexTextBox.IsInitialized = true;

    SDL_memset(StatThrowConTextBox.TextBoxBuffer, 0, sizeof(StatThrowConTextBox.TextBoxBuffer));
    StatThrowConTextBox.StringToDisplay.chars = StatThrowConTextBox.TextBoxBuffer;
    StatThrowConTextBox.StringToDisplay.length = 0;
    StatThrowConTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowConTextBox.IsInitialized = true;

    SDL_memset(StatThrowIntTextBox.TextBoxBuffer, 0, sizeof(StatThrowIntTextBox.TextBoxBuffer));
    StatThrowIntTextBox.StringToDisplay.chars = StatThrowIntTextBox.TextBoxBuffer;
    StatThrowIntTextBox.StringToDisplay.length = 0;
    StatThrowIntTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowIntTextBox.IsInitialized = true;

    SDL_memset(StatThrowWisTextBox.TextBoxBuffer, 0, sizeof(StatThrowWisTextBox.TextBoxBuffer));
    StatThrowWisTextBox.StringToDisplay.chars = StatThrowWisTextBox.TextBoxBuffer;
    StatThrowWisTextBox.StringToDisplay.length = 0;
    StatThrowWisTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowWisTextBox.IsInitialized = true;

    SDL_memset(StatThrowChaTextBox.TextBoxBuffer, 0, sizeof(StatThrowChaTextBox.TextBoxBuffer));
    StatThrowChaTextBox.StringToDisplay.chars = StatThrowChaTextBox.TextBoxBuffer;
    StatThrowChaTextBox.StringToDisplay.length = 0;
    StatThrowChaTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatThrowChaTextBox.IsInitialized = true;

    // Short Text Fields
    SDL_memset(StatSpeedTypeTextBox.TextBoxBuffer, 0, sizeof(StatSpeedTypeTextBox.TextBoxBuffer));
    StatSpeedTypeTextBox.StringToDisplay.chars = StatSpeedTypeTextBox.TextBoxBuffer;
    StatSpeedTypeTextBox.StringToDisplay.length = 0;
    StatSpeedTypeTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedTypeTextBox.IsInitialized = true;

    SDL_memset(StatSpeedWalkTextBox.TextBoxBuffer, 0, sizeof(StatSpeedWalkTextBox.TextBoxBuffer));
    StatSpeedWalkTextBox.StringToDisplay.chars = StatSpeedWalkTextBox.TextBoxBuffer;
    StatSpeedWalkTextBox.StringToDisplay.length = 0;
    StatSpeedWalkTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedWalkTextBox.IsInitialized = true;

    SDL_memset(StatSpeedFlyTextBox.TextBoxBuffer, 0, sizeof(StatSpeedFlyTextBox.TextBoxBuffer));
    StatSpeedFlyTextBox.StringToDisplay.chars = StatSpeedFlyTextBox.TextBoxBuffer;
    StatSpeedFlyTextBox.StringToDisplay.length = 0;
    StatSpeedFlyTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedFlyTextBox.IsInitialized = true;

    SDL_memset(StatSpeedSwimTextBox.TextBoxBuffer, 0, sizeof(StatSpeedSwimTextBox.TextBoxBuffer));
    StatSpeedSwimTextBox.StringToDisplay.chars = StatSpeedSwimTextBox.TextBoxBuffer;
    StatSpeedSwimTextBox.StringToDisplay.length = 0;
    StatSpeedSwimTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedSwimTextBox.IsInitialized = true;

    SDL_memset(StatSpeedClimbTextBox.TextBoxBuffer, 0, sizeof(StatSpeedClimbTextBox.TextBoxBuffer));
    StatSpeedClimbTextBox.StringToDisplay.chars = StatSpeedClimbTextBox.TextBoxBuffer;
    StatSpeedClimbTextBox.StringToDisplay.length = 0;
    StatSpeedClimbTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedClimbTextBox.IsInitialized = true;

    SDL_memset(StatSpeedBurrowTextBox.TextBoxBuffer, 0, sizeof(StatSpeedBurrowTextBox.TextBoxBuffer));
    StatSpeedBurrowTextBox.StringToDisplay.chars = StatSpeedBurrowTextBox.TextBoxBuffer;
    StatSpeedBurrowTextBox.StringToDisplay.length = 0;
    StatSpeedBurrowTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpeedBurrowTextBox.IsInitialized = true;

    SDL_memset(StatAlignmentTextBox.TextBoxBuffer, 0, sizeof(StatAlignmentTextBox.TextBoxBuffer));
    StatAlignmentTextBox.StringToDisplay.chars = StatAlignmentTextBox.TextBoxBuffer;
    StatAlignmentTextBox.StringToDisplay.length = 0;
    StatAlignmentTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAlignmentTextBox.IsInitialized = true;

    SDL_memset(StatLegendaryTextBox.TextBoxBuffer, 0, sizeof(StatLegendaryTextBox.TextBoxBuffer));
    StatLegendaryTextBox.StringToDisplay.chars = StatLegendaryTextBox.TextBoxBuffer;
    StatLegendaryTextBox.StringToDisplay.length = 0;
    StatLegendaryTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatLegendaryTextBox.IsInitialized = true;

    SDL_memset(StatSkillsTextBox.TextBoxBuffer, 0, sizeof(StatSkillsTextBox.TextBoxBuffer));
    StatSkillsTextBox.StringToDisplay.chars = StatSkillsTextBox.TextBoxBuffer;
    StatSkillsTextBox.StringToDisplay.length = 0;
    StatSkillsTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSkillsTextBox.IsInitialized = true;

    SDL_memset(StatLanguagesTextBox.TextBoxBuffer, 0, sizeof(StatLanguagesTextBox.TextBoxBuffer));
    StatLanguagesTextBox.StringToDisplay.chars = StatLanguagesTextBox.TextBoxBuffer;
    StatLanguagesTextBox.StringToDisplay.length = 0;
    StatLanguagesTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatLanguagesTextBox.IsInitialized = true;

    SDL_memset(StatSensesTextBox.TextBoxBuffer, 0, sizeof(StatSensesTextBox.TextBoxBuffer));
    StatSensesTextBox.StringToDisplay.chars = StatSensesTextBox.TextBoxBuffer;
    StatSensesTextBox.StringToDisplay.length = 0;
    StatSensesTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSensesTextBox.IsInitialized = true;

    SDL_memset(StatRangeDarkvisionTextBox.TextBoxBuffer, 0, sizeof(StatRangeDarkvisionTextBox.TextBoxBuffer));
    StatRangeDarkvisionTextBox.StringToDisplay.chars = StatRangeDarkvisionTextBox.TextBoxBuffer;
    StatRangeDarkvisionTextBox.StringToDisplay.length = 0;
    StatRangeDarkvisionTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRangeDarkvisionTextBox.IsInitialized = true;

    SDL_memset(StatRangeTremorsenseTextBox.TextBoxBuffer, 0, sizeof(StatRangeTremorsenseTextBox.TextBoxBuffer));
    StatRangeTremorsenseTextBox.StringToDisplay.chars = StatRangeTremorsenseTextBox.TextBoxBuffer;
    StatRangeTremorsenseTextBox.StringToDisplay.length = 0;
    StatRangeTremorsenseTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRangeTremorsenseTextBox.IsInitialized = true;

    SDL_memset(StatRangeBlindsightTextBox.TextBoxBuffer, 0, sizeof(StatRangeBlindsightTextBox.TextBoxBuffer));
    StatRangeBlindsightTextBox.StringToDisplay.chars = StatRangeBlindsightTextBox.TextBoxBuffer;
    StatRangeBlindsightTextBox.StringToDisplay.length = 0;
    StatRangeBlindsightTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRangeBlindsightTextBox.IsInitialized = true;

    SDL_memset(StatRangeTruesightTextBox.TextBoxBuffer, 0, sizeof(StatRangeTruesightTextBox.TextBoxBuffer));
    StatRangeTruesightTextBox.StringToDisplay.chars = StatRangeTruesightTextBox.TextBoxBuffer;
    StatRangeTruesightTextBox.StringToDisplay.length = 0;
    StatRangeTruesightTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRangeTruesightTextBox.IsInitialized = true;

    SDL_memset(StatSavingThrowsTextBox.TextBoxBuffer, 0, sizeof(StatSavingThrowsTextBox.TextBoxBuffer));
    StatSavingThrowsTextBox.StringToDisplay.chars = StatSavingThrowsTextBox.TextBoxBuffer;
    StatSavingThrowsTextBox.StringToDisplay.length = 0;
    StatSavingThrowsTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSavingThrowsTextBox.IsInitialized = true;

    // --- Special Abilities ---
    SDL_memset(StatSpecialAbilityOneTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityOneTextBox.TextBoxBuffer));
    StatSpecialAbilityOneTextBox.StringToDisplay.chars = StatSpecialAbilityOneTextBox.TextBoxBuffer;
    StatSpecialAbilityOneTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityOneTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityOneTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityOneDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityOneDescTextBox.TextBoxBuffer));
    StatSpecialAbilityOneDescTextBox.StringToDisplay.chars = StatSpecialAbilityOneDescTextBox.TextBoxBuffer;
    StatSpecialAbilityOneDescTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityOneDescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityOneDescTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityTwoTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityTwoTextBox.TextBoxBuffer));
    StatSpecialAbilityTwoTextBox.StringToDisplay.chars = StatSpecialAbilityTwoTextBox.TextBoxBuffer;
    StatSpecialAbilityTwoTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityTwoTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityTwoTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityTwoDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityTwoDescTextBox.TextBoxBuffer));
    StatSpecialAbilityTwoDescTextBox.StringToDisplay.chars = StatSpecialAbilityTwoDescTextBox.TextBoxBuffer;
    StatSpecialAbilityTwoDescTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityTwoDescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityTwoDescTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityThreeTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityThreeTextBox.TextBoxBuffer));
    StatSpecialAbilityThreeTextBox.StringToDisplay.chars = StatSpecialAbilityThreeTextBox.TextBoxBuffer;
    StatSpecialAbilityThreeTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityThreeTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityThreeTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityThreeDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityThreeDescTextBox.TextBoxBuffer));
    StatSpecialAbilityThreeDescTextBox.StringToDisplay.chars = StatSpecialAbilityThreeDescTextBox.TextBoxBuffer;
    StatSpecialAbilityThreeDescTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityThreeDescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityThreeDescTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityFourTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityFourTextBox.TextBoxBuffer));
    StatSpecialAbilityFourTextBox.StringToDisplay.chars = StatSpecialAbilityFourTextBox.TextBoxBuffer;
    StatSpecialAbilityFourTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityFourTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityFourTextBox.IsInitialized = true;

    SDL_memset(StatSpecialAbilityFourDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityFourDescTextBox.TextBoxBuffer));
    StatSpecialAbilityFourDescTextBox.StringToDisplay.chars = StatSpecialAbilityFourDescTextBox.TextBoxBuffer;
    StatSpecialAbilityFourDescTextBox.StringToDisplay.length = 0;
    StatSpecialAbilityFourDescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatSpecialAbilityFourDescTextBox.IsInitialized = true;

    // --- Attacks ---
    SDL_memset(StatAttack1TextBox.TextBoxBuffer, 0, sizeof(StatAttack1TextBox.TextBoxBuffer));
    StatAttack1TextBox.StringToDisplay.chars = StatAttack1TextBox.TextBoxBuffer;
    StatAttack1TextBox.StringToDisplay.length = 0;
    StatAttack1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack1TextBox.IsInitialized = true;

    SDL_memset(StatAttack1DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack1DescTextBox.TextBoxBuffer));
    StatAttack1DescTextBox.StringToDisplay.chars = StatAttack1DescTextBox.TextBoxBuffer;
    StatAttack1DescTextBox.StringToDisplay.length = 0;
    StatAttack1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack1DescTextBox.IsInitialized = true;

    SDL_memset(StatAttack2TextBox.TextBoxBuffer, 0, sizeof(StatAttack2TextBox.TextBoxBuffer));
    StatAttack2TextBox.StringToDisplay.chars = StatAttack2TextBox.TextBoxBuffer;
    StatAttack2TextBox.StringToDisplay.length = 0;
    StatAttack2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack2TextBox.IsInitialized = true;

    SDL_memset(StatAttack2DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack2DescTextBox.TextBoxBuffer));
    StatAttack2DescTextBox.StringToDisplay.chars = StatAttack2DescTextBox.TextBoxBuffer;
    StatAttack2DescTextBox.StringToDisplay.length = 0;
    StatAttack2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack2DescTextBox.IsInitialized = true;

    SDL_memset(StatAttack3TextBox.TextBoxBuffer, 0, sizeof(StatAttack3TextBox.TextBoxBuffer));
    StatAttack3TextBox.StringToDisplay.chars = StatAttack3TextBox.TextBoxBuffer;
    StatAttack3TextBox.StringToDisplay.length = 0;
    StatAttack3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack3TextBox.IsInitialized = true;

    SDL_memset(StatAttack3DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack3DescTextBox.TextBoxBuffer));
    StatAttack3DescTextBox.StringToDisplay.chars = StatAttack3DescTextBox.TextBoxBuffer;
    StatAttack3DescTextBox.StringToDisplay.length = 0;
    StatAttack3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack3DescTextBox.IsInitialized = true;

    SDL_memset(StatAttack4TextBox.TextBoxBuffer, 0, sizeof(StatAttack4TextBox.TextBoxBuffer));
    StatAttack4TextBox.StringToDisplay.chars = StatAttack4TextBox.TextBoxBuffer;
    StatAttack4TextBox.StringToDisplay.length = 0;
    StatAttack4TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack4TextBox.IsInitialized = true;

    SDL_memset(StatAttack4DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack4DescTextBox.TextBoxBuffer));
    StatAttack4DescTextBox.StringToDisplay.chars = StatAttack4DescTextBox.TextBoxBuffer;
    StatAttack4DescTextBox.StringToDisplay.length = 0;
    StatAttack4DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack4DescTextBox.IsInitialized = true;

    SDL_memset(StatAttack5TextBox.TextBoxBuffer, 0, sizeof(StatAttack5TextBox.TextBoxBuffer));
    StatAttack5TextBox.StringToDisplay.chars = StatAttack5TextBox.TextBoxBuffer;
    StatAttack5TextBox.StringToDisplay.length = 0;
    StatAttack5TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack5TextBox.IsInitialized = true;

    SDL_memset(StatAttack5DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack5DescTextBox.TextBoxBuffer));
    StatAttack5DescTextBox.StringToDisplay.chars = StatAttack5DescTextBox.TextBoxBuffer;
    StatAttack5DescTextBox.StringToDisplay.length = 0;
    StatAttack5DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack5DescTextBox.IsInitialized = true;

    SDL_memset(StatAttack6TextBox.TextBoxBuffer, 0, sizeof(StatAttack6TextBox.TextBoxBuffer));
    StatAttack6TextBox.StringToDisplay.chars = StatAttack6TextBox.TextBoxBuffer;
    StatAttack6TextBox.StringToDisplay.length = 0;
    StatAttack6TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack6TextBox.IsInitialized = true;

    SDL_memset(StatAttack6DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack6DescTextBox.TextBoxBuffer));
    StatAttack6DescTextBox.StringToDisplay.chars = StatAttack6DescTextBox.TextBoxBuffer;
    StatAttack6DescTextBox.StringToDisplay.length = 0;
    StatAttack6DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatAttack6DescTextBox.IsInitialized = true;

    // --- Legendary Actions ---
    SDL_memset(StatActionLegTextBox.TextBoxBuffer, 0, sizeof(StatActionLegTextBox.TextBoxBuffer));
    StatActionLegTextBox.StringToDisplay.chars = StatActionLegTextBox.TextBoxBuffer;
    StatActionLegTextBox.StringToDisplay.length = 0;
    StatActionLegTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLegTextBox.IsInitialized = true;

    SDL_memset(StatActionLeg1TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg1TextBox.TextBoxBuffer));
    StatActionLeg1TextBox.StringToDisplay.chars = StatActionLeg1TextBox.TextBoxBuffer;
    StatActionLeg1TextBox.StringToDisplay.length = 0;
    StatActionLeg1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg1TextBox.IsInitialized = true;

    SDL_memset(StatActionLeg1DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg1DescTextBox.TextBoxBuffer));
    StatActionLeg1DescTextBox.StringToDisplay.chars = StatActionLeg1DescTextBox.TextBoxBuffer;
    StatActionLeg1DescTextBox.StringToDisplay.length = 0;
    StatActionLeg1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg1DescTextBox.IsInitialized = true;

    SDL_memset(StatActionLeg2TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg2TextBox.TextBoxBuffer));
    StatActionLeg2TextBox.StringToDisplay.chars = StatActionLeg2TextBox.TextBoxBuffer;
    StatActionLeg2TextBox.StringToDisplay.length = 0;
    StatActionLeg2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg2TextBox.IsInitialized = true;

    SDL_memset(StatActionLeg2DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg2DescTextBox.TextBoxBuffer));
    StatActionLeg2DescTextBox.StringToDisplay.chars = StatActionLeg2DescTextBox.TextBoxBuffer;
    StatActionLeg2DescTextBox.StringToDisplay.length = 0;
    StatActionLeg2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg2DescTextBox.IsInitialized = true;

    SDL_memset(StatActionLeg3TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg3TextBox.TextBoxBuffer));
    StatActionLeg3TextBox.StringToDisplay.chars = StatActionLeg3TextBox.TextBoxBuffer;
    StatActionLeg3TextBox.StringToDisplay.length = 0;
    StatActionLeg3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg3TextBox.IsInitialized = true;

    SDL_memset(StatActionLeg3DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg3DescTextBox.TextBoxBuffer));
    StatActionLeg3DescTextBox.StringToDisplay.chars = StatActionLeg3DescTextBox.TextBoxBuffer;
    StatActionLeg3DescTextBox.StringToDisplay.length = 0;
    StatActionLeg3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLeg3DescTextBox.IsInitialized = true;

    // --- Lair Actions ---
    SDL_memset(StatActionLairTextBox.TextBoxBuffer, 0, sizeof(StatActionLairTextBox.TextBoxBuffer));
    StatActionLairTextBox.StringToDisplay.chars = StatActionLairTextBox.TextBoxBuffer;
    StatActionLairTextBox.StringToDisplay.length = 0;
    StatActionLairTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLairTextBox.IsInitialized = true;

    SDL_memset(StatActionLair1TextBox.TextBoxBuffer, 0, sizeof(StatActionLair1TextBox.TextBoxBuffer));
    StatActionLair1TextBox.StringToDisplay.chars = StatActionLair1TextBox.TextBoxBuffer;
    StatActionLair1TextBox.StringToDisplay.length = 0;
    StatActionLair1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair1TextBox.IsInitialized = true;

    SDL_memset(StatActionLair1DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair1DescTextBox.TextBoxBuffer));
    StatActionLair1DescTextBox.StringToDisplay.chars = StatActionLair1DescTextBox.TextBoxBuffer;
    StatActionLair1DescTextBox.StringToDisplay.length = 0;
    StatActionLair1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair1DescTextBox.IsInitialized = true;

    SDL_memset(StatActionLair2TextBox.TextBoxBuffer, 0, sizeof(StatActionLair2TextBox.TextBoxBuffer));
    StatActionLair2TextBox.StringToDisplay.chars = StatActionLair2TextBox.TextBoxBuffer;
    StatActionLair2TextBox.StringToDisplay.length = 0;
    StatActionLair2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair2TextBox.IsInitialized = true;

    SDL_memset(StatActionLair2DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair2DescTextBox.TextBoxBuffer));
    StatActionLair2DescTextBox.StringToDisplay.chars = StatActionLair2DescTextBox.TextBoxBuffer;
    StatActionLair2DescTextBox.StringToDisplay.length = 0;
    StatActionLair2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair2DescTextBox.IsInitialized = true;

    SDL_memset(StatActionLair3TextBox.TextBoxBuffer, 0, sizeof(StatActionLair3TextBox.TextBoxBuffer));
    StatActionLair3TextBox.StringToDisplay.chars = StatActionLair3TextBox.TextBoxBuffer;
    StatActionLair3TextBox.StringToDisplay.length = 0;
    StatActionLair3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair3TextBox.IsInitialized = true;

    SDL_memset(StatActionLair3DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair3DescTextBox.TextBoxBuffer));
    StatActionLair3DescTextBox.StringToDisplay.chars = StatActionLair3DescTextBox.TextBoxBuffer;
    StatActionLair3DescTextBox.StringToDisplay.length = 0;
    StatActionLair3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatActionLair3DescTextBox.IsInitialized = true;

    // --- Regional Effects ---
    SDL_memset(StatRegionalEffectTextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffectTextBox.TextBoxBuffer));
    StatRegionalEffectTextBox.StringToDisplay.chars = StatRegionalEffectTextBox.TextBoxBuffer;
    StatRegionalEffectTextBox.StringToDisplay.length = 0;
    StatRegionalEffectTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRegionalEffectTextBox.IsInitialized = true;

    SDL_memset(StatRegionalEffect1TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect1TextBox.TextBoxBuffer));
    StatRegionalEffect1TextBox.StringToDisplay.chars = StatRegionalEffect1TextBox.TextBoxBuffer;
    StatRegionalEffect1TextBox.StringToDisplay.length = 0;
    StatRegionalEffect1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRegionalEffect1TextBox.IsInitialized = true;

    SDL_memset(StatRegionalEffect2TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect2TextBox.TextBoxBuffer));
    StatRegionalEffect2TextBox.StringToDisplay.chars = StatRegionalEffect2TextBox.TextBoxBuffer;
    StatRegionalEffect2TextBox.StringToDisplay.length = 0;
    StatRegionalEffect2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRegionalEffect2TextBox.IsInitialized = true;

    SDL_memset(StatRegionalEffect3TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect3TextBox.TextBoxBuffer));
    StatRegionalEffect3TextBox.StringToDisplay.chars = StatRegionalEffect3TextBox.TextBoxBuffer;
    StatRegionalEffect3TextBox.StringToDisplay.length = 0;
    StatRegionalEffect3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatRegionalEffect3TextBox.IsInitialized = true;

    SDL_memset(StatEndRegionalEffectTextBox.TextBoxBuffer, 0, sizeof(StatEndRegionalEffectTextBox.TextBoxBuffer));
    StatEndRegionalEffectTextBox.StringToDisplay.chars = StatEndRegionalEffectTextBox.TextBoxBuffer;
    StatEndRegionalEffectTextBox.StringToDisplay.length = 0;
    StatEndRegionalEffectTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatEndRegionalEffectTextBox.IsInitialized = true;

    // --- Environment / Breath Attacks ---
    SDL_memset(StatEnvironmentTextBox.TextBoxBuffer, 0, sizeof(StatEnvironmentTextBox.TextBoxBuffer));
    StatEnvironmentTextBox.StringToDisplay.chars = StatEnvironmentTextBox.TextBoxBuffer;
    StatEnvironmentTextBox.StringToDisplay.length = 0;
    StatEnvironmentTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatEnvironmentTextBox.IsInitialized = true;

    SDL_memset(StatBa1TextBox.TextBoxBuffer, 0, sizeof(StatBa1TextBox.TextBoxBuffer));
    StatBa1TextBox.StringToDisplay.chars = StatBa1TextBox.TextBoxBuffer;
    StatBa1TextBox.StringToDisplay.length = 0;
    StatBa1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa1TextBox.IsInitialized = true;

    SDL_memset(StatBa1DescTextBox.TextBoxBuffer, 0, sizeof(StatBa1DescTextBox.TextBoxBuffer));
    StatBa1DescTextBox.StringToDisplay.chars = StatBa1DescTextBox.TextBoxBuffer;
    StatBa1DescTextBox.StringToDisplay.length = 0;
    StatBa1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa1DescTextBox.IsInitialized = true;

    SDL_memset(StatBa2TextBox.TextBoxBuffer, 0, sizeof(StatBa2TextBox.TextBoxBuffer));
    StatBa2TextBox.StringToDisplay.chars = StatBa2TextBox.TextBoxBuffer;
    StatBa2TextBox.StringToDisplay.length = 0;
    StatBa2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa2TextBox.IsInitialized = true;

    SDL_memset(StatBa2DescTextBox.TextBoxBuffer, 0, sizeof(StatBa2DescTextBox.TextBoxBuffer));
    StatBa2DescTextBox.StringToDisplay.chars = StatBa2DescTextBox.TextBoxBuffer;
    StatBa2DescTextBox.StringToDisplay.length = 0;
    StatBa2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa2DescTextBox.IsInitialized = true;

    SDL_memset(StatBa3TextBox.TextBoxBuffer, 0, sizeof(StatBa3TextBox.TextBoxBuffer));
    StatBa3TextBox.StringToDisplay.chars = StatBa3TextBox.TextBoxBuffer;
    StatBa3TextBox.StringToDisplay.length = 0;
    StatBa3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa3TextBox.IsInitialized = true;

    SDL_memset(StatBa3DescTextBox.TextBoxBuffer, 0, sizeof(StatBa3DescTextBox.TextBoxBuffer));
    StatBa3DescTextBox.StringToDisplay.chars = StatBa3DescTextBox.TextBoxBuffer;
    StatBa3DescTextBox.StringToDisplay.length = 0;
    StatBa3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa3DescTextBox.IsInitialized = true;

    SDL_memset(StatBa4TextBox.TextBoxBuffer, 0, sizeof(StatBa4TextBox.TextBoxBuffer));
    StatBa4TextBox.StringToDisplay.chars = StatBa4TextBox.TextBoxBuffer;
    StatBa4TextBox.StringToDisplay.length = 0;
    StatBa4TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa4TextBox.IsInitialized = true;

    SDL_memset(StatBa4DescTextBox.TextBoxBuffer, 0, sizeof(StatBa4DescTextBox.TextBoxBuffer));
    StatBa4DescTextBox.StringToDisplay.chars = StatBa4DescTextBox.TextBoxBuffer;
    StatBa4DescTextBox.StringToDisplay.length = 0;
    StatBa4DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatBa4DescTextBox.IsInitialized = true;

    // --- Reactions ---
    SDL_memset(StatReaction1TextBox.TextBoxBuffer, 0, sizeof(StatReaction1TextBox.TextBoxBuffer));
    StatReaction1TextBox.StringToDisplay.chars = StatReaction1TextBox.TextBoxBuffer;
    StatReaction1TextBox.StringToDisplay.length = 0;
    StatReaction1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction1TextBox.IsInitialized = true;

    SDL_memset(StatReaction1DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction1DescTextBox.TextBoxBuffer));
    StatReaction1DescTextBox.StringToDisplay.chars = StatReaction1DescTextBox.TextBoxBuffer;
    StatReaction1DescTextBox.StringToDisplay.length = 0;
    StatReaction1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction1DescTextBox.IsInitialized = true;

    SDL_memset(StatReaction2TextBox.TextBoxBuffer, 0, sizeof(StatReaction2TextBox.TextBoxBuffer));
    StatReaction2TextBox.StringToDisplay.chars = StatReaction2TextBox.TextBoxBuffer;
    StatReaction2TextBox.StringToDisplay.length = 0;
    StatReaction2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction2TextBox.IsInitialized = true;

    SDL_memset(StatReaction2DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction2DescTextBox.TextBoxBuffer));
    StatReaction2DescTextBox.StringToDisplay.chars = StatReaction2DescTextBox.TextBoxBuffer;
    StatReaction2DescTextBox.StringToDisplay.length = 0;
    StatReaction2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction2DescTextBox.IsInitialized = true;

    SDL_memset(StatReaction3TextBox.TextBoxBuffer, 0, sizeof(StatReaction3TextBox.TextBoxBuffer));
    StatReaction3TextBox.StringToDisplay.chars = StatReaction3TextBox.TextBoxBuffer;
    StatReaction3TextBox.StringToDisplay.length = 0;
    StatReaction3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction3TextBox.IsInitialized = true;

    SDL_memset(StatReaction3DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction3DescTextBox.TextBoxBuffer));
    StatReaction3DescTextBox.StringToDisplay.chars = StatReaction3DescTextBox.TextBoxBuffer;
    StatReaction3DescTextBox.StringToDisplay.length = 0;
    StatReaction3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatReaction3DescTextBox.IsInitialized = true;

    // --- Villain Actions ---
    SDL_memset(StatVillActionTextBox.TextBoxBuffer, 0, sizeof(StatVillActionTextBox.TextBoxBuffer));
    StatVillActionTextBox.StringToDisplay.chars = StatVillActionTextBox.TextBoxBuffer;
    StatVillActionTextBox.StringToDisplay.length = 0;
    StatVillActionTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillActionTextBox.IsInitialized = true;

    SDL_memset(StatVillAction1TextBox.TextBoxBuffer, 0, sizeof(StatVillAction1TextBox.TextBoxBuffer));
    StatVillAction1TextBox.StringToDisplay.chars = StatVillAction1TextBox.TextBoxBuffer;
    StatVillAction1TextBox.StringToDisplay.length = 0;
    StatVillAction1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction1TextBox.IsInitialized = true;

    SDL_memset(StatVillAction1DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction1DescTextBox.TextBoxBuffer));
    StatVillAction1DescTextBox.StringToDisplay.chars = StatVillAction1DescTextBox.TextBoxBuffer;
    StatVillAction1DescTextBox.StringToDisplay.length = 0;
    StatVillAction1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction1DescTextBox.IsInitialized = true;

    SDL_memset(StatVillAction2TextBox.TextBoxBuffer, 0, sizeof(StatVillAction2TextBox.TextBoxBuffer));
    StatVillAction2TextBox.StringToDisplay.chars = StatVillAction2TextBox.TextBoxBuffer;
    StatVillAction2TextBox.StringToDisplay.length = 0;
    StatVillAction2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction2TextBox.IsInitialized = true;

    SDL_memset(StatVillAction2DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction2DescTextBox.TextBoxBuffer));
    StatVillAction2DescTextBox.StringToDisplay.chars = StatVillAction2DescTextBox.TextBoxBuffer;
    StatVillAction2DescTextBox.StringToDisplay.length = 0;
    StatVillAction2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction2DescTextBox.IsInitialized = true;

    SDL_memset(StatVillAction3TextBox.TextBoxBuffer, 0, sizeof(StatVillAction3TextBox.TextBoxBuffer));
    StatVillAction3TextBox.StringToDisplay.chars = StatVillAction3TextBox.TextBoxBuffer;
    StatVillAction3TextBox.StringToDisplay.length = 0;
    StatVillAction3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction3TextBox.IsInitialized = true;

    SDL_memset(StatVillAction3DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction3DescTextBox.TextBoxBuffer));
    StatVillAction3DescTextBox.StringToDisplay.chars = StatVillAction3DescTextBox.TextBoxBuffer;
    StatVillAction3DescTextBox.StringToDisplay.length = 0;
    StatVillAction3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatVillAction3DescTextBox.IsInitialized = true;

    // --- Utility Spells ---
    SDL_memset(StatUtilitySpellsTextBox.TextBoxBuffer, 0, sizeof(StatUtilitySpellsTextBox.TextBoxBuffer));
    StatUtilitySpellsTextBox.StringToDisplay.chars = StatUtilitySpellsTextBox.TextBoxBuffer;
    StatUtilitySpellsTextBox.StringToDisplay.length = 0;
    StatUtilitySpellsTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatUtilitySpellsTextBox.IsInitialized = true;

    SDL_memset(StatUtilitySpellsListTextBox.TextBoxBuffer, 0, sizeof(StatUtilitySpellsListTextBox.TextBoxBuffer));
    StatUtilitySpellsListTextBox.StringToDisplay.chars = StatUtilitySpellsListTextBox.TextBoxBuffer;
    StatUtilitySpellsListTextBox.StringToDisplay.length = 0;
    StatUtilitySpellsListTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatUtilitySpellsListTextBox.IsInitialized = true;

    // --- Features ---
    SDL_memset(StatFeature1TextBox.TextBoxBuffer, 0, sizeof(StatFeature1TextBox.TextBoxBuffer));
    StatFeature1TextBox.StringToDisplay.chars = StatFeature1TextBox.TextBoxBuffer;
    StatFeature1TextBox.StringToDisplay.length = 0;
    StatFeature1TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature1TextBox.IsInitialized = true;

    SDL_memset(StatFeature1DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature1DescTextBox.TextBoxBuffer));
    StatFeature1DescTextBox.StringToDisplay.chars = StatFeature1DescTextBox.TextBoxBuffer;
    StatFeature1DescTextBox.StringToDisplay.length = 0;
    StatFeature1DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature1DescTextBox.IsInitialized = true;

    SDL_memset(StatFeature2TextBox.TextBoxBuffer, 0, sizeof(StatFeature2TextBox.TextBoxBuffer));
    StatFeature2TextBox.StringToDisplay.chars = StatFeature2TextBox.TextBoxBuffer;
    StatFeature2TextBox.StringToDisplay.length = 0;
    StatFeature2TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature2TextBox.IsInitialized = true;

    SDL_memset(StatFeature2DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature2DescTextBox.TextBoxBuffer));
    StatFeature2DescTextBox.StringToDisplay.chars = StatFeature2DescTextBox.TextBoxBuffer;
    StatFeature2DescTextBox.StringToDisplay.length = 0;
    StatFeature2DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature2DescTextBox.IsInitialized = true;

    SDL_memset(StatFeature3TextBox.TextBoxBuffer, 0, sizeof(StatFeature3TextBox.TextBoxBuffer));
    StatFeature3TextBox.StringToDisplay.chars = StatFeature3TextBox.TextBoxBuffer;
    StatFeature3TextBox.StringToDisplay.length = 0;
    StatFeature3TextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature3TextBox.IsInitialized = true;

    SDL_memset(StatFeature3DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature3DescTextBox.TextBoxBuffer));
    StatFeature3DescTextBox.StringToDisplay.chars = StatFeature3DescTextBox.TextBoxBuffer;
    StatFeature3DescTextBox.StringToDisplay.length = 0;
    StatFeature3DescTextBox.StringToDisplay.isStaticallyAllocated = false;
    StatFeature3DescTextBox.IsInitialized = true;

    for (int i = 0; i < BUILD_LIST_MAX; i++) {
        BuildListMembers[i].initiative = 0;
        BuildListMembers[i].Quantity = 0;
        SDL_memset(BuildListMembers[i].name, '\0', sizeof(BuildListMembers[i].name));
        BuildListMembers[i].IsAdded = false;
        BuildListMembers[i].IsCreature = false;
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

    for(int i = 0; i < SPEED_TYPES; i++) {
        sb->SpeedValues[i] = MakeClayString("");
    };

    for(int i = 0; i < NUM_ABILITIES; i++) {
        sb->StatValues[i] = MakeClayString("");
    };

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

    for(int i = 0; i < SPEED_TYPES; i++) {
        ClearClayString(&sb->SpeedValues[i]);
    };

    for(int i = 0; i < NUM_ABILITIES; i++) {
        ClearClayString(&sb->StatValues[i]);
    };

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

void ClearFocus() {
    gAppState->IsTextInputFocused = false;
        /* Clear focused ID */
    gAppState->focusedId = CLAY_ID("NULL");
}

void ResetVisibleCreatureHeaders() {
    for (int i = 0; i <TotalCreatures; i++) {
        HeadersToShow[i] = i;
    }
}

void FreeLinkedLists() {
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
        StartEncounterState = ENCOUNTER_MAIN_SCREEN;
    }
}

void ResetBuildListData() {
    for (int i = 0; i < BUILD_LIST_MAX; i++) {
        BuildListMembers[i].initiative = 0;
        BuildListMembers[i].Quantity = 0;
        SDL_memset(BuildListMembers[i].name, 0, sizeof(BuildListMembers[i].name));
        BuildListMembers[i].IsAdded = false;
        BuildListMembers[i].IsCreature = false;
    }

    /* Reset list to an unstarted state */
    ListStarted = 0;

    /* Reset start encounter window to the initial two options */
    StartEncounterState = ENCOUNTER_MAIN_SCREEN;
}

void ClearTextBoxes() {
    SDL_memset(TextBuffer, 0, sizeof(TextBuffer)); 

    SDL_memset(EncounterName.TextBoxBuffer, 0, sizeof(EncounterName.TextBoxBuffer));
    EncounterName.StringToDisplay.length = 0;

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

        SDL_memset(BuildListMemberInitiative[i].TextBoxBuffer, 0, sizeof(BuildListMemberInitiative[i].TextBoxBuffer));
        BuildListMemberInitiative[i].StringToDisplay.length = 0;
    }

    // Core Info
    SDL_memset(StatNameTextBox.TextBoxBuffer, 0, sizeof(StatNameTextBox.TextBoxBuffer));
    StatNameTextBox.StringToDisplay.length = 0;

    SDL_memset(StatCrTextBox.TextBoxBuffer, 0, sizeof(StatCrTextBox.TextBoxBuffer));
    StatCrTextBox.StringToDisplay.length = 0;

    SDL_memset(StatTypeTextBox.TextBoxBuffer, 0, sizeof(StatTypeTextBox.TextBoxBuffer));
    StatTypeTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSizeTextBox.TextBoxBuffer, 0, sizeof(StatSizeTextBox.TextBoxBuffer));
    StatSizeTextBox.StringToDisplay.length = 0;

    SDL_memset(StatArmorClassTextBox.TextBoxBuffer, 0, sizeof(StatArmorClassTextBox.TextBoxBuffer));
    StatArmorClassTextBox.StringToDisplay.length = 0;

    SDL_memset(StatHitpointsAvgTextBox.TextBoxBuffer, 0, sizeof(StatHitpointsAvgTextBox.TextBoxBuffer));
    StatHitpointsAvgTextBox.StringToDisplay.length = 0;

    SDL_memset(StatHitDiceTextBox.TextBoxBuffer, 0, sizeof(StatHitDiceTextBox.TextBoxBuffer));
    StatHitDiceTextBox.StringToDisplay.length = 0;

    SDL_memset(StatHitpointsRollTextBox.TextBoxBuffer, 0, sizeof(StatHitpointsRollTextBox.TextBoxBuffer));
    StatHitpointsRollTextBox.StringToDisplay.length = 0;

    // Ability Scores
    SDL_memset(StatStrTextBox.TextBoxBuffer, 0, sizeof(StatStrTextBox.TextBoxBuffer));
    StatStrTextBox.StringToDisplay.length = 0;

    SDL_memset(StatDexTextBox.TextBoxBuffer, 0, sizeof(StatDexTextBox.TextBoxBuffer));
    StatDexTextBox.StringToDisplay.length = 0;

    SDL_memset(StatConTextBox.TextBoxBuffer, 0, sizeof(StatConTextBox.TextBoxBuffer));
    StatConTextBox.StringToDisplay.length = 0;

    SDL_memset(StatIntTextBox.TextBoxBuffer, 0, sizeof(StatIntTextBox.TextBoxBuffer));
    StatIntTextBox.StringToDisplay.length = 0;

    SDL_memset(StatWisTextBox.TextBoxBuffer, 0, sizeof(StatWisTextBox.TextBoxBuffer));
    StatWisTextBox.StringToDisplay.length = 0;

    SDL_memset(StatChaTextBox.TextBoxBuffer, 0, sizeof(StatChaTextBox.TextBoxBuffer));
    StatChaTextBox.StringToDisplay.length = 0;

    SDL_memset(StatProfBonusTextBox.TextBoxBuffer, 0, sizeof(StatProfBonusTextBox.TextBoxBuffer));
    StatProfBonusTextBox.StringToDisplay.length = 0;

    // Saving Throws
    SDL_memset(StatThrowStrTextBox.TextBoxBuffer, 0, sizeof(StatThrowStrTextBox.TextBoxBuffer));
    StatThrowStrTextBox.StringToDisplay.length = 0;

    SDL_memset(StatThrowDexTextBox.TextBoxBuffer, 0, sizeof(StatThrowDexTextBox.TextBoxBuffer));
    StatThrowDexTextBox.StringToDisplay.length = 0;

    SDL_memset(StatThrowConTextBox.TextBoxBuffer, 0, sizeof(StatThrowConTextBox.TextBoxBuffer));
    StatThrowConTextBox.StringToDisplay.length = 0;

    SDL_memset(StatThrowIntTextBox.TextBoxBuffer, 0, sizeof(StatThrowIntTextBox.TextBoxBuffer));
    StatThrowIntTextBox.StringToDisplay.length = 0;

    SDL_memset(StatThrowWisTextBox.TextBoxBuffer, 0, sizeof(StatThrowWisTextBox.TextBoxBuffer));
    StatThrowWisTextBox.StringToDisplay.length = 0;

    SDL_memset(StatThrowChaTextBox.TextBoxBuffer, 0, sizeof(StatThrowChaTextBox.TextBoxBuffer));
    StatThrowChaTextBox.StringToDisplay.length = 0;

    // Short Text Fields
    SDL_memset(StatSpeedTypeTextBox.TextBoxBuffer, 0, sizeof(StatSpeedTypeTextBox.TextBoxBuffer));
    StatSpeedTypeTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpeedWalkTextBox.TextBoxBuffer, 0, sizeof(StatSpeedWalkTextBox.TextBoxBuffer));
    StatSpeedWalkTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpeedFlyTextBox.TextBoxBuffer, 0, sizeof(StatSpeedFlyTextBox.TextBoxBuffer));
    StatSpeedFlyTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpeedSwimTextBox.TextBoxBuffer, 0, sizeof(StatSpeedSwimTextBox.TextBoxBuffer));
    StatSpeedSwimTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpeedClimbTextBox.TextBoxBuffer, 0, sizeof(StatSpeedClimbTextBox.TextBoxBuffer));
    StatSpeedClimbTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpeedBurrowTextBox.TextBoxBuffer, 0, sizeof(StatSpeedBurrowTextBox.TextBoxBuffer));
    StatSpeedBurrowTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAlignmentTextBox.TextBoxBuffer, 0, sizeof(StatAlignmentTextBox.TextBoxBuffer));
    StatAlignmentTextBox.StringToDisplay.length = 0;

    SDL_memset(StatLegendaryTextBox.TextBoxBuffer, 0, sizeof(StatLegendaryTextBox.TextBoxBuffer));
    StatLegendaryTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSkillsTextBox.TextBoxBuffer, 0, sizeof(StatSkillsTextBox.TextBoxBuffer));
    StatSkillsTextBox.StringToDisplay.length = 0;

    SDL_memset(StatLanguagesTextBox.TextBoxBuffer, 0, sizeof(StatLanguagesTextBox.TextBoxBuffer));
    StatLanguagesTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSensesTextBox.TextBoxBuffer, 0, sizeof(StatSensesTextBox.TextBoxBuffer));
    StatSensesTextBox.StringToDisplay.length = 0;

    SDL_memset(StatRangeDarkvisionTextBox.TextBoxBuffer, 0, sizeof(StatRangeDarkvisionTextBox.TextBoxBuffer));
    StatRangeDarkvisionTextBox.StringToDisplay.length = 0;

    SDL_memset(StatRangeTremorsenseTextBox.TextBoxBuffer, 0, sizeof(StatRangeTremorsenseTextBox.TextBoxBuffer));
    StatRangeTremorsenseTextBox.StringToDisplay.length = 0;

    SDL_memset(StatRangeBlindsightTextBox.TextBoxBuffer, 0, sizeof(StatRangeBlindsightTextBox.TextBoxBuffer));
    StatRangeBlindsightTextBox.StringToDisplay.length = 0;

    SDL_memset(StatRangeTruesightTextBox.TextBoxBuffer, 0, sizeof(StatRangeTruesightTextBox.TextBoxBuffer));
    StatRangeTruesightTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSavingThrowsTextBox.TextBoxBuffer, 0, sizeof(StatSavingThrowsTextBox.TextBoxBuffer));
    StatSavingThrowsTextBox.StringToDisplay.length = 0;

    // Special Abilities
    SDL_memset(StatSpecialAbilityOneTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityOneTextBox.TextBoxBuffer));
    StatSpecialAbilityOneTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityOneDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityOneDescTextBox.TextBoxBuffer));
    StatSpecialAbilityOneDescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityTwoTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityTwoTextBox.TextBoxBuffer));
    StatSpecialAbilityTwoTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityTwoDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityTwoDescTextBox.TextBoxBuffer));
    StatSpecialAbilityTwoDescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityThreeTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityThreeTextBox.TextBoxBuffer));
    StatSpecialAbilityThreeTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityThreeDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityThreeDescTextBox.TextBoxBuffer));
    StatSpecialAbilityThreeDescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityFourTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityFourTextBox.TextBoxBuffer));
    StatSpecialAbilityFourTextBox.StringToDisplay.length = 0;

    SDL_memset(StatSpecialAbilityFourDescTextBox.TextBoxBuffer, 0, sizeof(StatSpecialAbilityFourDescTextBox.TextBoxBuffer));
    StatSpecialAbilityFourDescTextBox.StringToDisplay.length = 0;

    // Attacks
    SDL_memset(StatAttack1TextBox.TextBoxBuffer, 0, sizeof(StatAttack1TextBox.TextBoxBuffer));
    StatAttack1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack1DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack1DescTextBox.TextBoxBuffer));
    StatAttack1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack2TextBox.TextBoxBuffer, 0, sizeof(StatAttack2TextBox.TextBoxBuffer));
    StatAttack2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack2DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack2DescTextBox.TextBoxBuffer));
    StatAttack2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack3TextBox.TextBoxBuffer, 0, sizeof(StatAttack3TextBox.TextBoxBuffer));
    StatAttack3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack3DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack3DescTextBox.TextBoxBuffer));
    StatAttack3DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack4TextBox.TextBoxBuffer, 0, sizeof(StatAttack4TextBox.TextBoxBuffer));
    StatAttack4TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack4DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack4DescTextBox.TextBoxBuffer));
    StatAttack4DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack5TextBox.TextBoxBuffer, 0, sizeof(StatAttack5TextBox.TextBoxBuffer));
    StatAttack5TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack5DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack5DescTextBox.TextBoxBuffer));
    StatAttack5DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack6TextBox.TextBoxBuffer, 0, sizeof(StatAttack6TextBox.TextBoxBuffer));
    StatAttack6TextBox.StringToDisplay.length = 0;

    SDL_memset(StatAttack6DescTextBox.TextBoxBuffer, 0, sizeof(StatAttack6DescTextBox.TextBoxBuffer));
    StatAttack6DescTextBox.StringToDisplay.length = 0;

    // Legendary Actions
    SDL_memset(StatActionLegTextBox.TextBoxBuffer, 0, sizeof(StatActionLegTextBox.TextBoxBuffer));
    StatActionLegTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg1TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg1TextBox.TextBoxBuffer));
    StatActionLeg1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg1DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg1DescTextBox.TextBoxBuffer));
    StatActionLeg1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg2TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg2TextBox.TextBoxBuffer));
    StatActionLeg2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg2DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg2DescTextBox.TextBoxBuffer));
    StatActionLeg2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg3TextBox.TextBoxBuffer, 0, sizeof(StatActionLeg3TextBox.TextBoxBuffer));
    StatActionLeg3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLeg3DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLeg3DescTextBox.TextBoxBuffer));
    StatActionLeg3DescTextBox.StringToDisplay.length = 0;

    // Lair Actions
    SDL_memset(StatActionLairTextBox.TextBoxBuffer, 0, sizeof(StatActionLairTextBox.TextBoxBuffer));
    StatActionLairTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair1TextBox.TextBoxBuffer, 0, sizeof(StatActionLair1TextBox.TextBoxBuffer));
    StatActionLair1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair1DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair1DescTextBox.TextBoxBuffer));
    StatActionLair1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair2TextBox.TextBoxBuffer, 0, sizeof(StatActionLair2TextBox.TextBoxBuffer));
    StatActionLair2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair2DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair2DescTextBox.TextBoxBuffer));
    StatActionLair2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair3TextBox.TextBoxBuffer, 0, sizeof(StatActionLair3TextBox.TextBoxBuffer));
    StatActionLair3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatActionLair3DescTextBox.TextBoxBuffer, 0, sizeof(StatActionLair3DescTextBox.TextBoxBuffer));
    StatActionLair3DescTextBox.StringToDisplay.length = 0;

    // Regional Effects
    SDL_memset(StatRegionalEffectTextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffectTextBox.TextBoxBuffer));
    StatRegionalEffectTextBox.StringToDisplay.length = 0;

    SDL_memset(StatRegionalEffect1TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect1TextBox.TextBoxBuffer));
    StatRegionalEffect1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatRegionalEffect2TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect2TextBox.TextBoxBuffer));
    StatRegionalEffect2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatRegionalEffect3TextBox.TextBoxBuffer, 0, sizeof(StatRegionalEffect3TextBox.TextBoxBuffer));
    StatRegionalEffect3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatEndRegionalEffectTextBox.TextBoxBuffer, 0, sizeof(StatEndRegionalEffectTextBox.TextBoxBuffer));
    StatEndRegionalEffectTextBox.StringToDisplay.length = 0;

    // Environment / Breath Attacks
    SDL_memset(StatEnvironmentTextBox.TextBoxBuffer, 0, sizeof(StatEnvironmentTextBox.TextBoxBuffer));
    StatEnvironmentTextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa1TextBox.TextBoxBuffer, 0, sizeof(StatBa1TextBox.TextBoxBuffer));
    StatBa1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa1DescTextBox.TextBoxBuffer, 0, sizeof(StatBa1DescTextBox.TextBoxBuffer));
    StatBa1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa2TextBox.TextBoxBuffer, 0, sizeof(StatBa2TextBox.TextBoxBuffer));
    StatBa2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa2DescTextBox.TextBoxBuffer, 0, sizeof(StatBa2DescTextBox.TextBoxBuffer));
    StatBa2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa3TextBox.TextBoxBuffer, 0, sizeof(StatBa3TextBox.TextBoxBuffer));
    StatBa3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa3DescTextBox.TextBoxBuffer, 0, sizeof(StatBa3DescTextBox.TextBoxBuffer));
    StatBa3DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa4TextBox.TextBoxBuffer, 0, sizeof(StatBa4TextBox.TextBoxBuffer));
    StatBa4TextBox.StringToDisplay.length = 0;

    SDL_memset(StatBa4DescTextBox.TextBoxBuffer, 0, sizeof(StatBa4DescTextBox.TextBoxBuffer));
    StatBa4DescTextBox.StringToDisplay.length = 0;

    // Reactions
    SDL_memset(StatReaction1TextBox.TextBoxBuffer, 0, sizeof(StatReaction1TextBox.TextBoxBuffer));
    StatReaction1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatReaction1DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction1DescTextBox.TextBoxBuffer));
    StatReaction1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatReaction2TextBox.TextBoxBuffer, 0, sizeof(StatReaction2TextBox.TextBoxBuffer));
    StatReaction2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatReaction2DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction2DescTextBox.TextBoxBuffer));
    StatReaction2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatReaction3TextBox.TextBoxBuffer, 0, sizeof(StatReaction3TextBox.TextBoxBuffer));
    StatReaction3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatReaction3DescTextBox.TextBoxBuffer, 0, sizeof(StatReaction3DescTextBox.TextBoxBuffer));
    StatReaction3DescTextBox.StringToDisplay.length = 0;

    // Villain Actions
    SDL_memset(StatVillActionTextBox.TextBoxBuffer, 0, sizeof(StatVillActionTextBox.TextBoxBuffer));
    StatVillActionTextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction1TextBox.TextBoxBuffer, 0, sizeof(StatVillAction1TextBox.TextBoxBuffer));
    StatVillAction1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction1DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction1DescTextBox.TextBoxBuffer));
    StatVillAction1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction2TextBox.TextBoxBuffer, 0, sizeof(StatVillAction2TextBox.TextBoxBuffer));
    StatVillAction2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction2DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction2DescTextBox.TextBoxBuffer));
    StatVillAction2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction3TextBox.TextBoxBuffer, 0, sizeof(StatVillAction3TextBox.TextBoxBuffer));
    StatVillAction3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatVillAction3DescTextBox.TextBoxBuffer, 0, sizeof(StatVillAction3DescTextBox.TextBoxBuffer));
    StatVillAction3DescTextBox.StringToDisplay.length = 0;

    // Utility Spells
    SDL_memset(StatUtilitySpellsTextBox.TextBoxBuffer, 0, sizeof(StatUtilitySpellsTextBox.TextBoxBuffer));
    StatUtilitySpellsTextBox.StringToDisplay.length = 0;

    SDL_memset(StatUtilitySpellsListTextBox.TextBoxBuffer, 0, sizeof(StatUtilitySpellsListTextBox.TextBoxBuffer));
    StatUtilitySpellsListTextBox.StringToDisplay.length = 0;

    // Features
    SDL_memset(StatFeature1TextBox.TextBoxBuffer, 0, sizeof(StatFeature1TextBox.TextBoxBuffer));
    StatFeature1TextBox.StringToDisplay.length = 0;

    SDL_memset(StatFeature1DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature1DescTextBox.TextBoxBuffer));
    StatFeature1DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatFeature2TextBox.TextBoxBuffer, 0, sizeof(StatFeature2TextBox.TextBoxBuffer));
    StatFeature2TextBox.StringToDisplay.length = 0;

    SDL_memset(StatFeature2DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature2DescTextBox.TextBoxBuffer));
    StatFeature2DescTextBox.StringToDisplay.length = 0;

    SDL_memset(StatFeature3TextBox.TextBoxBuffer, 0, sizeof(StatFeature3TextBox.TextBoxBuffer));
    StatFeature3TextBox.StringToDisplay.length = 0;

    SDL_memset(StatFeature3DescTextBox.TextBoxBuffer, 0, sizeof(StatFeature3DescTextBox.TextBoxBuffer));
    StatFeature3DescTextBox.StringToDisplay.length = 0;
}

/* Callbacks*/

void FocusWindowCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    AppState *state = userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->focusedId = elementId;
        gAppState->IsTextInputFocused = false;
    }
}

void ClearFocusCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    AppState *state = userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        ClearFocus();
    }
}

void FocusWindowAndCallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->focusedId = elementId;
        gAppState->IsTextInputFocused = false;
        int * GivenId = (int *) userData;
        int LookUpId = *GivenId - 1;
        LookUpCreatureStats(LookUpId);
    }
}

void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int * check = (int *) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        Turn = 0;
        RefreshDatabaseMonsters();
        RefreshDatabasePlayers();
        RefreshDatabaseEncounters();
        ClearFocus();

        ClearTextBoxes();

        WindowState = MAIN_SCREEN;

        ResetVisibleCreatureHeaders();

        FreeLinkedLists();

        ResetBuildListData();        
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

