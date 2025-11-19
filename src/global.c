#include "clay.h"
#include "global.h"

int WindowState = 0;

char TextBuffer[MAX_TEXT] = {0};

const int FONT_ID_BODY_16 = 0;
const int FONT_ID_BODY_32 = 0;

int WindowWidth = 0;
int WindowHeight = 0;
uint16_t TotalCreatures = 0;

float ScrollOffset = 0;

bool MouseDown;

Clay_String TypedText = {0};

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

void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        SDL_memset(TextBuffer, 0, sizeof(TextBuffer));
        ScrollOffset = 0;
        WindowState = MAIN_SCREEN;
        // WindowState = ADD_STAT_SCREEN;
    }
}
