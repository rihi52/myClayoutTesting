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
#define MAX_DB_COUNT 1000
#define SMALL_INTEGER_INPUT_BUFFER_SIZE     32
#define MEDIUM_INTEGER_INPUT_BUFFER_SIZE    64
#define LARGE_INTEGER_INPUT_BUFFER_SIZE     128
#define SMALL_INPUT_TEXT_BUFFER_SIZE        256
#define MEDIUM_INPUT_TEXT_BUFFER_SIZE       512
#define LARGE_INPUT_TEXT_BUFFER_SIZE        2048

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

extern CreatureHeader DBPageHeaders[MAX_DB_COUNT];

void DatabaseOpen(void);
void DatabaseClose(void);
int LoadCreatureHeaderAlphabetical(int MonsterId);
Clay_String MakeClayString(const char * string);
void LookUpCreatureStats(int MonsterId);
void SearchCreatureNames(const char * SearchTerm);
void ModifyTypedString(void);
Clay_String MakeClayIntString(int value);
DisplayListMember * LookupCreatureForCombat(const char * CreatureName, int Initiative, bool IsCreature);

#endif /* DB_QUERY */