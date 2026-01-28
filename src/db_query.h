#ifndef DB_QUERY
#define DB_QUERY
#include "sql/sqlite3.h"
#include <SDL3/SDL.h>
#include "clay.h"
#include "string.h"
#include "global.h"

/*========================================================================* 
 *  SECTION - Defines
 *========================================================================* 
 */
#define MAX_DB_COUNT 2000

typedef enum
{
    INT_CR = 0,
    INT_ARMOR_CLASS,
    INT_HITPOINTS_AVG,
    INT_HIT_DICE,
    INT_HITPOINTS_ROLL,
    INT_STR,
    INT_DEX,
    INT_CON,
    INT_INT,
    INT_WIS,
    INT_CHA,
    INT_PROF_BONUS,
    // INT_THROW_STR,
    // INT_THROW_DEX,
    // INT_THROW_CON,
    // INT_THROW_INT,
    // INT_THROW_WIS,
    // INT_THROW_CHA,

    NUMBER_INTEGER_FIELDS
} IntegerFieldIndex;

typedef enum
{
    TXT_NAME = 0,
    TXT_TYPE,
    TXT_SIZE,
    TXT_SPEED_TYPE,
    TXT_SPEED_WALK,
    TXT_SPEED_FLY,
    TXT_SPEED_SWIM,
    TXT_SPEED_CLIMB,
    TXT_SPEED_BURROW,
    TXT_ALIGNMENT,
    TXT_LEGENDARY,
    TXT_SKILLS,
    TXT_LANGUAGES,
    TXT_SENSES,
    TXT_RANGE_DARKVISION,
    TXT_RANGE_TREMORSENSE,
    TXT_RANGE_BLINDSIGHT,
    TXT_RANGE_TRUESIGHT,
    TXT_SPECIAL_ABILITY_ONE,
    TXT_SPECIAL_ABILITY_ONE_DESC,
    TXT_SPECIAL_ABILITY_TWO,
    TXT_SPECIAL_ABILITY_TWO_DESC,
    TXT_SPECIAL_ABILITY_THREE,
    TXT_SPECIAL_ABILITY_THREE_DESC,
    TXT_SPECIAL_ABILITY_FOUR,
    TXT_SPECIAL_ABILITY_FOUR_DESC,
    TXT_ATTACK_1,
    TXT_ATTACK_1_DESC,
    TXT_ATTACK_2,
    TXT_ATTACK_2_DESC,
    TXT_ATTACK_3,
    TXT_ATTACK_3_DESC,
    TXT_ATTACK_4,
    TXT_ATTACK_4_DESC,
    TXT_ATTACK_5,
    TXT_ATTACK_5_DESC,
    TXT_ATTACK_6,
    TXT_ATTACK_6_DESC,
    TXT_ACTION_LEG,
    TXT_ACTION_LEG1,
    TXT_ACTION_LEG1_DESC,
    TXT_ACTION_LEG2,
    TXT_ACTION_LEG2_DESC,
    TXT_ACTION_LEG3,
    TXT_ACTION_LEG3_DESC,
    TXT_ACTION_LAIR,
    TXT_ACTION_LAIR1,
    TXT_ACTION_LAIR1_DESC,
    TXT_ACTION_LAIR2,
    TXT_ACTION_LAIR2_DESC,
    TXT_ACTION_LAIR3,
    TXT_ACTION_LAIR3_DESC,
    TXT_REGIONAL_EFFECT,
    TXT_REGIONAL_EFFECT1,
    TXT_REGIONAL_EFFECT2,
    TXT_REGIONAL_EFFECT3,
    TXT_END_REGIONAL_EFFECT,
    TXT_ENVIRONMENT,
    TXT_BA1,
    TXT_BA1_DESC,
    TXT_BA2,
    TXT_BA2_DESC,
    TXT_BA3,
    TXT_BA3_DESC,
    TXT_BA4,
    TXT_BA4_DESC,
    TXT_REACTION1,
    TXT_REACTION1_DESC,
    TXT_REACTION2,
    TXT_REACTION2_DESC,
    TXT_REACTION3,
    TXT_REACTION3_DESC,
    TXT_VILL_ACTION,
    TXT_VILL_ACTION1,
    TXT_VILL_ACTION1_DESC,
    TXT_VILL_ACTION2,
    TXT_VILL_ACTION2_DESC,
    TXT_VILL_ACTION3,
    TXT_VILL_ACTION3_DESC,
    TXT_UTILITY_SPELLS,
    TXT_UTILITY_SPELLS_LIST,
    TXT_FEATURE1,
    TXT_FEATURE1_DESC,
    TXT_FEATURE2,
    TXT_FEATURE2_DESC,
    TXT_FEATURE3,
    TXT_FEATURE3_DESC,
    TXT_FEATURE4,
    TXT_FEATURE4_DESC,
    TXT_FEATURE5,
    TXT_FEATURE5_DESC,

    NUMBER_TEXT_FIELDS
} TextFieldIndex;

/*========================================================================* 
 *  SECTION - Global structs
 *========================================================================* 
 */
typedef struct CreatureHeader {
Clay_String CreatureName;
Clay_String CreatureCR;
Clay_String CreatureSize;
Clay_String CreatureType;
Clay_String CreatureSource;
}CreatureHeader;

typedef struct PlayerHeader {
Clay_String PlayerName;
Clay_String PlayerAC;
}PlayerHeader;

typedef struct EncounterHeader {
Clay_String EncounterName;
}EncounterHeader;

/*========================================================================* 
 *  SECTION - External variables that cannot be defined in header files
 *========================================================================* 
 */
// Integer / numeric fields
extern TextBox StatCrTextBox;
extern TextBox StatArmorClassTextBox;
extern TextBox StatHitpointsAvgTextBox;
extern TextBox StatHitDiceTextBox;
extern TextBox StatHitpointsRollTextBox;
extern TextBox StatStrTextBox;
extern TextBox StatDexTextBox;
extern TextBox StatConTextBox;
extern TextBox StatIntTextBox;
extern TextBox StatWisTextBox;
extern TextBox StatChaTextBox;
extern TextBox StatProfBonusTextBox;
extern TextBox StatThrowStrTextBox;
extern TextBox StatThrowDexTextBox;
extern TextBox StatThrowConTextBox;
extern TextBox StatThrowIntTextBox;
extern TextBox StatThrowWisTextBox;
extern TextBox StatThrowChaTextBox;

// Short text fields
extern TextBox StatNameTextBox;
extern TextBox StatTypeTextBox;
extern TextBox StatSizeTextBox;
extern TextBox StatSpeedTypeTextBox;
extern TextBox StatSpeedWalkTextBox;
extern TextBox StatSpeedFlyTextBox;
extern TextBox StatSpeedSwimTextBox;
extern TextBox StatSpeedClimbTextBox;
extern TextBox StatSpeedBurrowTextBox;
extern TextBox StatAlignmentTextBox;
extern TextBox StatLegendaryTextBox;
extern TextBox StatSkillsTextBox;
extern TextBox StatLanguagesTextBox;
extern TextBox StatSensesTextBox;
extern TextBox StatRangeDarkvisionTextBox;
extern TextBox StatRangeTremorsenseTextBox;
extern TextBox StatRangeBlindsightTextBox;
extern TextBox StatRangeTruesightTextBox;
extern TextBox StatSavingThrowsTextBox;

// Long text fields
extern TextBox StatSpecialAbilityOneTextBox;
extern TextBox StatSpecialAbilityOneDescTextBox;
extern TextBox StatSpecialAbilityTwoTextBox;
extern TextBox StatSpecialAbilityTwoDescTextBox;
extern TextBox StatSpecialAbilityThreeTextBox;
extern TextBox StatSpecialAbilityThreeDescTextBox;
extern TextBox StatSpecialAbilityFourTextBox;
extern TextBox StatSpecialAbilityFourDescTextBox;

extern TextBox StatAttack1TextBox;
extern TextBox StatAttack1DescTextBox;
extern TextBox StatAttack2TextBox;
extern TextBox StatAttack2DescTextBox;
extern TextBox StatAttack3TextBox;
extern TextBox StatAttack3DescTextBox;
extern TextBox StatAttack4TextBox;
extern TextBox StatAttack4DescTextBox;
extern TextBox StatAttack5TextBox;
extern TextBox StatAttack5DescTextBox;
extern TextBox StatAttack6TextBox;
extern TextBox StatAttack6DescTextBox;

extern TextBox StatActionLegTextBox;
extern TextBox StatActionLeg1TextBox;
extern TextBox StatActionLeg1DescTextBox;
extern TextBox StatActionLeg2TextBox;
extern TextBox StatActionLeg2DescTextBox;
extern TextBox StatActionLeg3TextBox;
extern TextBox StatActionLeg3DescTextBox;

extern TextBox StatActionLairTextBox;
extern TextBox StatActionLair1TextBox;
extern TextBox StatActionLair1DescTextBox;
extern TextBox StatActionLair2TextBox;
extern TextBox StatActionLair2DescTextBox;
extern TextBox StatActionLair3TextBox;
extern TextBox StatActionLair3DescTextBox;

extern TextBox StatRegionalEffectTextBox;
extern TextBox StatRegionalEffect1TextBox;
extern TextBox StatRegionalEffect2TextBox;
extern TextBox StatRegionalEffect3TextBox;
extern TextBox StatEndRegionalEffectTextBox;

extern TextBox StatEnvironmentTextBox;

extern TextBox StatBa1TextBox;
extern TextBox StatBa1DescTextBox;
extern TextBox StatBa2TextBox;
extern TextBox StatBa2DescTextBox;
extern TextBox StatBa3TextBox;
extern TextBox StatBa3DescTextBox;
extern TextBox StatBa4TextBox;
extern TextBox StatBa4DescTextBox;

extern TextBox StatReaction1TextBox;
extern TextBox StatReaction1DescTextBox;
extern TextBox StatReaction2TextBox;
extern TextBox StatReaction2DescTextBox;
extern TextBox StatReaction3TextBox;
extern TextBox StatReaction3DescTextBox;

extern TextBox StatVillActionTextBox;
extern TextBox StatVillAction1TextBox;
extern TextBox StatVillAction1DescTextBox;
extern TextBox StatVillAction2TextBox;
extern TextBox StatVillAction2DescTextBox;
extern TextBox StatVillAction3TextBox;
extern TextBox StatVillAction3DescTextBox;

extern TextBox StatUtilitySpellsTextBox;
extern TextBox StatUtilitySpellsListTextBox;

extern TextBox StatFeature1TextBox;
extern TextBox StatFeature1DescTextBox;
extern TextBox StatFeature2TextBox;
extern TextBox StatFeature2DescTextBox;
extern TextBox StatFeature3TextBox;
extern TextBox StatFeature3DescTextBox;
extern TextBox StatFeature4TextBox;
extern TextBox StatFeature4DescTextBox;
extern TextBox StatFeature5TextBox;
extern TextBox StatFeature5DescTextBox;


extern sqlite3 * pGuidnbatterDB;

extern int HeadersToShow[MAX_DB_COUNT];
extern int PlayersToShow[MAX_DB_COUNT];
extern int EncountersToShow[MAX_DB_COUNT];

extern CreatureHeader DBPageHeaders[MAX_DB_COUNT];
extern PlayerHeader DBPlayerPageHeaders[MAX_DB_COUNT];
extern EncounterHeader DBEncounterPageHeaders[MAX_DB_COUNT];

void DatabaseOpen(void);
void DatabaseClose(void);
void LoadDatabaseMonsters();
void LoadDatabasePlayers();
void RefreshDatabaseMonsters();
void RefreshDatabasePlayers();
void RefreshDatabaseEncounters();
void LoadDatabaseEncounters();
int LoadCreatureHeaderAlphabetical(int MonsterId);
int LoadPlayers(int PlayerId);
Clay_String MakeClayString(const char * string);
void LookUpCreatureStats(int MonsterId);
void SearchCreatureNames(const char * SearchTerm);
void ModifyTypedString(void);
void SaveNewStatBlockToDB();
void SaveEncounterToDB();
Clay_String MakeClayIntString(int value);
DisplayListMember * LookupCreatureForCombat(const char * CreatureName, int Initiative, bool IsCreature);
DisplayListMember * LookupPlayerForCombat(const char * PlayerName, int Initiative, bool IsCreature);
void StructureFieldArrays();

#endif /* DB_QUERY */