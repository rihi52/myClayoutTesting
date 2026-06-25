#include "db_query.h"
#include "global.h"

/*========================================================================*
 *  SECTION - Local Function Prototypes                                   *
 *========================================================================*
 */
sqlite3_int64 InsertEncounterNameToDB();
int InsertEncounterCreaturesToDB(sqlite3_int64 EncounterID);
void InsertEncounterPlayersToDB(sqlite3_int64 EncounterID);
int LoadEncounters(int EncounterId);
void LoadEncounterCreaturesForDisplay(int EncounterId);
void LoadEncounterPlayersForDisplay(int EncounterId);

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;

CreatureHeader DBPageHeaders[MAX_DB_COUNT] = {0};
PlayerHeader DBPlayerPageHeaders[MAX_DB_COUNT] = {0};
EncounterHeader DBEncounterPageHeaders[MAX_DB_COUNT] = {0};

int HeadersToShow[MAX_DB_COUNT];
int PlayersToShow[MAX_DB_COUNT];
int EncountersToShow[MAX_DB_COUNT];

static const char * BoundedStrStr(const char * haystack, const char * needle, size_t haystackLen, size_t NeedleLen);

TextBox * IntegerFields[NUMBER_INTEGER_FIELDS];
TextBox * TextFields[NUMBER_TEXT_FIELDS];

TextBox StatCrTextBox;
TextBox StatArmorClassTextBox;
TextBox StatHitpointsAvgTextBox;
TextBox StatHitDiceTextBox;
TextBox StatHitpointsRollTextBox;
TextBox StatStrTextBox;
TextBox StatDexTextBox;
TextBox StatConTextBox;
TextBox StatIntTextBox;
TextBox StatWisTextBox;
TextBox StatChaTextBox;
TextBox StatProfBonusTextBox;
TextBox StatThrowStrTextBox;
TextBox StatThrowDexTextBox;
TextBox StatThrowConTextBox;
TextBox StatThrowIntTextBox;
TextBox StatThrowWisTextBox;
TextBox StatThrowChaTextBox;

// Short text fields
TextBox StatNameTextBox;
TextBox StatTypeTextBox;
TextBox StatSizeTextBox;
TextBox StatSpeedTypeTextBox;
TextBox StatSpeedWalkTextBox;
TextBox StatSpeedFlyTextBox;
TextBox StatSpeedSwimTextBox;
TextBox StatSpeedClimbTextBox;
TextBox StatSpeedBurrowTextBox;
TextBox StatAlignmentTextBox;
TextBox StatLegendaryTextBox;
TextBox StatSkillsTextBox;
TextBox StatLanguagesTextBox;
TextBox StatSensesTextBox;
TextBox StatRangeDarkvisionTextBox;
TextBox StatRangeTremorsenseTextBox;
TextBox StatRangeBlindsightTextBox;
TextBox StatRangeTruesightTextBox;
TextBox StatSpecialAbilityOneTextBox;
TextBox StatSpecialAbilityOneDescTextBox;
TextBox StatSpecialAbilityTwoTextBox;
TextBox StatSpecialAbilityTwoDescTextBox;
TextBox StatSpecialAbilityThreeTextBox;
TextBox StatSpecialAbilityThreeDescTextBox;
TextBox StatSpecialAbilityFourTextBox;
TextBox StatSpecialAbilityFourDescTextBox;
TextBox StatAttack1TextBox;
TextBox StatAttack1DescTextBox;
TextBox StatAttack2TextBox;
TextBox StatAttack2DescTextBox;
TextBox StatAttack3TextBox;
TextBox StatAttack3DescTextBox;
TextBox StatAttack4TextBox;
TextBox StatAttack4DescTextBox;
TextBox StatAttack5TextBox;
TextBox StatAttack5DescTextBox;
TextBox StatAttack6TextBox;
TextBox StatAttack6DescTextBox;
TextBox StatActionLegTextBox;
TextBox StatActionLeg1TextBox;
TextBox StatActionLeg1DescTextBox;
TextBox StatActionLeg2TextBox;
TextBox StatActionLeg2DescTextBox;
TextBox StatActionLeg3TextBox;
TextBox StatActionLeg3DescTextBox;
TextBox StatActionLairTextBox;
TextBox StatActionLair1TextBox;
TextBox StatActionLair1DescTextBox;
TextBox StatActionLair2TextBox;
TextBox StatActionLair2DescTextBox;
TextBox StatActionLair3TextBox;
TextBox StatActionLair3DescTextBox;
TextBox StatRegionalEffectTextBox;
TextBox StatRegionalEffect1TextBox;
TextBox StatRegionalEffect2TextBox;
TextBox StatRegionalEffect3TextBox;
TextBox StatEndRegionalEffectTextBox;
TextBox StatEnvironmentTextBox;
TextBox StatBa1TextBox;
TextBox StatBa1DescTextBox;
TextBox StatBa2TextBox;
TextBox StatBa2DescTextBox;
TextBox StatBa3TextBox;
TextBox StatBa3DescTextBox;
TextBox StatBa4TextBox;
TextBox StatBa4DescTextBox;
TextBox StatReaction1TextBox;
TextBox StatReaction1DescTextBox;
TextBox StatReaction2TextBox;
TextBox StatReaction2DescTextBox;
TextBox StatReaction3TextBox;
TextBox StatReaction3DescTextBox;
TextBox StatVillActionTextBox;
TextBox StatVillAction1TextBox;
TextBox StatVillAction1DescTextBox;
TextBox StatVillAction2TextBox;
TextBox StatVillAction2DescTextBox;
TextBox StatVillAction3TextBox;
TextBox StatVillAction3DescTextBox;
TextBox StatUtilitySpellsTextBox;
TextBox StatUtilitySpellsListTextBox;
TextBox StatFeature1TextBox;
TextBox StatFeature1DescTextBox;
TextBox StatFeature2TextBox;
TextBox StatFeature2DescTextBox;
TextBox StatFeature3TextBox;
TextBox StatFeature3DescTextBox;
TextBox StatFeature4TextBox;
TextBox StatFeature4DescTextBox;
TextBox StatFeature5TextBox;
TextBox StatFeature5DescTextBox;

TextBox StatSavingThrowsTextBox;

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void DatabaseOpen(void) {

    int rc = sqlite3_open("../guidnbatter.db", &pGuidnbatterDB);
    if (rc)
    {
        /* Exit if the database can't be opened */
        SDL_Log("Can't open database: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }
    else
    {
        SDL_Log("Opened database successfully");
    }
    return;
}

void DatabaseClose(void) {
    sqlite3_close(pGuidnbatterDB);
}

void LoadDatabaseMonsters() {
    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (0 == LoadCreatureHeaderAlphabetical(i)) {
            TotalCreatures++;
            HeadersToShow[i] = i;
        }
        else {
            HeadersToShow[i] = -1;
        }
    }
}

void LoadDatabasePlayers() {
    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (0 == LoadPlayers(i)) {
            TotalPlayers++;
            PlayersToShow[i] = i;
        }
        else {
            PlayersToShow[i] = -1;
        }
    }
}

void LoadDatabaseEncounters() {
    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (0 == LoadEncounters(i)) {
            TotalEncounters++;
            EncountersToShow[i] = i;
        }
        else {
            EncountersToShow[i] = -1;
        }
    }
}

void RefreshDatabaseMonsters() {
    TotalCreatures = 0;

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        HeadersToShow[i] = -1;
    }

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (LoadCreatureHeaderAlphabetical(i) == 0) {
            HeadersToShow[TotalCreatures] = i;
            TotalCreatures++;
        } else {
            break;
        }
    }
}

void RefreshDatabasePlayers() {
    TotalPlayers = 0;

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        PlayersToShow[i] = -1;
    }

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (LoadPlayers(i) == 0) {
            PlayersToShow[TotalPlayers] = i;
            TotalPlayers++;
        } else {
            break;
        }
    }
}

void RefreshDatabaseEncounters() {
    TotalEncounters = 0;

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        EncountersToShow[i] = -1;
    }

    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (LoadEncounters(i) == 0) {
            EncountersToShow[TotalEncounters] = i;
            TotalEncounters++;
        } else {
            break;
        }
    }
}

Clay_String MakeClayString(const char * string) {
    if (string == NULL) {
        string = "";
    }

    size_t length = SDL_strlen(string);
    char * copy = SDL_malloc(length + 1);
    if (!copy) {
        Clay_String str = {
            .isStaticallyAllocated = false,
            .length = 0,
            .chars = NULL
        };
        return str;
    }

    SDL_strlcpy(copy, string, length + 1);

    Clay_String str = {
        .isStaticallyAllocated = false,
        .length = length,
        .chars = copy
    };

    return str;
}

void ClearClayString(Clay_String *s) {
    if (!s->isStaticallyAllocated && s->chars != NULL) {
        SDL_free((void*)s->chars);
    }
    s->chars = NULL;
    s->length = 0;
    s->isStaticallyAllocated = false;
}

Clay_String MakeClayIntString(int value) {
    char buffer[32];
    SDL_snprintf(buffer, sizeof(buffer), "%d", value);
    return MakeClayString(buffer);
}

int LoadCreatureHeaderAlphabetical(int MonsterId) {

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT name, cr, size, type FROM monsters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, MonsterId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const char *Name    = sqlite3_column_text(stmt, 0);
        const char *Cr      = sqlite3_column_text(stmt, 1);
        const char *Size    = sqlite3_column_text(stmt, 2);
        const char *Type    = sqlite3_column_text(stmt, 3);
        //const char *Source  = sqlite3_column_text(stmt, 4);

        DBPageHeaders[MonsterId].CreatureName = MakeClayString(Name);
        DBPageHeaders[MonsterId].CreatureCR = MakeClayString(Cr);
        DBPageHeaders[MonsterId].CreatureSize = MakeClayString(Size);
        DBPageHeaders[MonsterId].CreatureType = MakeClayString(Type);
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    if (NULL == DBPageHeaders[MonsterId].CreatureName.chars) return 1;
    return 0;
}

int LoadPlayers(int PlayerId) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT name, armor_class FROM players WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, PlayerId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const char *Name    = sqlite3_column_text(stmt, 0);
        const char *Ac      = sqlite3_column_text(stmt, 1);
        //const char *Source  = sqlite3_column_text(stmt, 4);

        DBPlayerPageHeaders[PlayerId].PlayerName = MakeClayString(Name);
        DBPlayerPageHeaders[PlayerId].PlayerAC = MakeClayString(Ac);
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    if (NULL == DBPlayerPageHeaders[PlayerId].PlayerName.chars) return 1;
    return 0;
}

int LoadEncounters(int EncounterId) {
    int ReturnedValue = 0;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT encounter_name FROM encounters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        ReturnedValue = -1;
        return ReturnedValue;
    }

    sqlite3_bind_int(stmt, 1, EncounterId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const char *Name = sqlite3_column_text(stmt, 0);

        DBEncounterPageHeaders[EncounterId].EncounterName = MakeClayString(Name);
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    if (NULL == DBEncounterPageHeaders[EncounterId].EncounterName.chars){
        ReturnedValue = 1;
        return ReturnedValue;
    }
    return ReturnedValue;
}

void LookUpEncounterDetails(Clay_String *EncounterName) {
    char *EncounterToLookUp = NULL;
    SDL_strlcpy(EncounterToLookUp, EncounterName->chars, EncounterName->length + 1);

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id FROM encounters WHERE encounter_name = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    sqlite3_bind_text(stmt, 1, EncounterToLookUp, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        int EncounterId = sqlite3_column_int(stmt, 0);
        LoadEncounterCreaturesForDisplay(EncounterId);
        LoadEncounterPlayersForDisplay(EncounterId);
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s, no encounter named %s in database", sqlite3_errmsg(pGuidnbatterDB), EncounterToLookUp);
    }

    sqlite3_finalize(stmt);
}

void LoadEncounterCreaturesForDisplay(int EncounterId) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT creature_name, initiative, quantity FROM encounters WHERE encounter_id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    sqlite3_bind_int(stmt, 1, EncounterId);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
            if (BuildListMembers[i].IsAdded) {
                continue;
            }
            const char *CreatureName = sqlite3_column_text(stmt, 0);

            SDL_strlcpy(BuildListMembers[i].name, CreatureName, SDL_strlen(CreatureName) + 1);
            BuildListMembers[i].initiative = sqlite3_column_int(stmt, 1);
            BuildListMembers[i].Quantity = sqlite3_column_int(stmt, 2);
            BuildListMembers[i].IsAdded = true;
            BuildListMembers[i].IsCreature = true;
        }
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    return;
}

void LoadEncounterPlayersForDisplay(int EncounterId) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT player_name, initiative FROM encounters WHERE encounter_id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    sqlite3_bind_int(stmt, 1, EncounterId);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        for (int i = 0; i < BUILD_LIST_MAX; i++) {
            if (BuildListMembers[i].IsAdded) {
                continue;
            }
            const char *PlayerName = sqlite3_column_text(stmt, 0);

            SDL_strlcpy(BuildListMembers[i].name, PlayerName, SDL_strlen(PlayerName) + 1);
            BuildListMembers[i].initiative = sqlite3_column_int(stmt, 1);
            BuildListMembers[i].Quantity = 1;
            BuildListMembers[i].IsAdded = true;
            BuildListMembers[i].IsCreature = false;
        }
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    return;
}

void LookUpCreatureStats(int MonsterId) {
    // Clear all Clay_Strings before writing new data
    ClearClayString(&gAppState->CurrentStatBlock.StatId);
    ClearClayString(&gAppState->CurrentStatBlock.StatName);
    ClearClayString(&gAppState->CurrentStatBlock.StatCr);
    ClearClayString(&gAppState->CurrentStatBlock.StatType);
    ClearClayString(&gAppState->CurrentStatBlock.StatSize);
    ClearClayString(&gAppState->CurrentStatBlock.StatArmorClass);
    ClearClayString(&gAppState->CurrentStatBlock.StatHitpointsAvg);
    ClearClayString(&gAppState->CurrentStatBlock.StatHitDice);
    ClearClayString(&gAppState->CurrentStatBlock.StatHitpointsRoll);

    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedType);
    for (int i = 0; i < SPEED_TYPES; i++) {
        ClearClayString(&gAppState->CurrentStatBlock.SpeedValues[i]);
    }
    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedWalk);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedFly);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedSwim);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedClimb);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpeedBurrow);

    ClearClayString(&gAppState->CurrentStatBlock.StatAlignment);
    ClearClayString(&gAppState->CurrentStatBlock.StatLegendary);

    ClearClayString(&gAppState->CurrentStatBlock.StatStr);
    ClearClayString(&gAppState->CurrentStatBlock.StatDex);
    ClearClayString(&gAppState->CurrentStatBlock.StatCon);
    ClearClayString(&gAppState->CurrentStatBlock.StatInt);
    ClearClayString(&gAppState->CurrentStatBlock.StatWis);
    ClearClayString(&gAppState->CurrentStatBlock.StatCha);

    ClearClayString(&gAppState->CurrentStatBlock.StatProfBonus);

    ClearClayString(&gAppState->CurrentStatBlock.StatThrowStr);
    ClearClayString(&gAppState->CurrentStatBlock.StatThrowDex);
    ClearClayString(&gAppState->CurrentStatBlock.StatThrowCon);
    ClearClayString(&gAppState->CurrentStatBlock.StatThrowInt);
    ClearClayString(&gAppState->CurrentStatBlock.StatThrowWis);
    ClearClayString(&gAppState->CurrentStatBlock.StatThrowCha);
    ClearClayString(&gAppState->CurrentStatBlock.StatSavingThrows);

    ClearClayString(&gAppState->CurrentStatBlock.StatSkills);
    ClearClayString(&gAppState->CurrentStatBlock.StatLanguages);
    ClearClayString(&gAppState->CurrentStatBlock.StatSenses);

    ClearClayString(&gAppState->CurrentStatBlock.StatRangeDarkvision);
    ClearClayString(&gAppState->CurrentStatBlock.StatRangeTremorsense);
    ClearClayString(&gAppState->CurrentStatBlock.StatRangeBlindsight);
    ClearClayString(&gAppState->CurrentStatBlock.StatRangeTruesight);

    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityOne);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityOneDesc);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityTwo);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityTwoDesc);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityThree);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityThreeDesc);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityFour);
    ClearClayString(&gAppState->CurrentStatBlock.StatSpecialAbilityFourDesc);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack1);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack2);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack3);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack3Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack4);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack4Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatAttack5);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack5Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatAttack6);
    ClearClayString(&gAppState->CurrentStatBlock.StatAttack6Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg1);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg2);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg3);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLeg3Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair1);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair2);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair3);
    ClearClayString(&gAppState->CurrentStatBlock.StatActionLair3Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatRegionalEffect);
    ClearClayString(&gAppState->CurrentStatBlock.StatRegionalEffect1);
    ClearClayString(&gAppState->CurrentStatBlock.StatRegionalEffect2);
    ClearClayString(&gAppState->CurrentStatBlock.StatRegionalEffect3);
    ClearClayString(&gAppState->CurrentStatBlock.StatEndRegionalEffect);

    ClearClayString(&gAppState->CurrentStatBlock.StatEnvironment);

    ClearClayString(&gAppState->CurrentStatBlock.StatBa1);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa2);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa3);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa3Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa4);
    ClearClayString(&gAppState->CurrentStatBlock.StatBa4Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatReaction1);
    ClearClayString(&gAppState->CurrentStatBlock.StatReaction1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatReaction2);
    ClearClayString(&gAppState->CurrentStatBlock.StatReaction2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatReaction3);
    ClearClayString(&gAppState->CurrentStatBlock.StatReaction3Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction1);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction2);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction3);
    ClearClayString(&gAppState->CurrentStatBlock.StatVillAction3Desc);

    ClearClayString(&gAppState->CurrentStatBlock.StatUtilitySpells);
    ClearClayString(&gAppState->CurrentStatBlock.StatUtilitySpellsList);

    ClearClayString(&gAppState->CurrentStatBlock.StatFeature1);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature1Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature2);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature2Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature3);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature3Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature4);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature4Desc);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature5);
    ClearClayString(&gAppState->CurrentStatBlock.StatFeature5Desc);
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM monsters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    sqlite3_bind_int(stmt, 1, MonsterId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        int col = 0;

        gAppState->CurrentStatBlock.StatId                = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatName              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatCr                = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatType              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSize              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatArmorClass        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatHitpointsAvg      = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatHitDice           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatHitpointsRoll     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatSpeedType         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.SpeedValues[WALK]     = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.SpeedValues[FLY]      = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.SpeedValues[SWIM]     = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.SpeedValues[CLIMB]    = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.SpeedValues[BURROW]   = MakeClayIntString(sqlite3_column_int(stmt, col++));

        // gAppState->CurrentStatBlock.StatSpeedWalk         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatSpeedFly          = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatSpeedSwim         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatSpeedClimb        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatSpeedBurrow       = MakeClayIntString(sqlite3_column_int(stmt, col++));

        gAppState->CurrentStatBlock.StatAlignment         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatLegendary         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatValues[STR]       = MakeClayIntString(sqlite3_column_int(stmt, col++));       
        gAppState->CurrentStatBlock.StatValues[DEX]       = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatValues[CON]       = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatValues[INT]       = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatValues[WIS]       = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatValues[CHA]       = MakeClayIntString(sqlite3_column_int(stmt, col++));

        // gAppState->CurrentStatBlock.StatStr               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatDex               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatCon               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatInt               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatWis               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        // gAppState->CurrentStatBlock.StatCha               = MakeClayIntString(sqlite3_column_int(stmt, col++));

        gAppState->CurrentStatBlock.StatProfBonus         = MakeClayIntString(sqlite3_column_int(stmt, col++));

        int strScore = atoi(gAppState->CurrentStatBlock.StatValues[STR].chars);
        int dexScore = atoi(gAppState->CurrentStatBlock.StatValues[DEX].chars);
        int conScore = atoi(gAppState->CurrentStatBlock.StatValues[CON].chars);
        int intScore = atoi(gAppState->CurrentStatBlock.StatValues[INT].chars);
        int wisScore = atoi(gAppState->CurrentStatBlock.StatValues[WIS].chars);
        int chaScore = atoi(gAppState->CurrentStatBlock.StatValues[CHA].chars);


        #define DND_MOD(score) \
        (((score) - 10) / 2 - (((score) - 10) < 0 && ((score) - 10) % 2 != 0 ? 1 : 0))


        int strMod = DND_MOD(strScore);
        int dexMod = DND_MOD(dexScore);
        int conMod = DND_MOD(conScore);
        int intMod = DND_MOD(intScore);
        int wisMod = DND_MOD(wisScore);
        int chaMod = DND_MOD(chaScore);

        int throwStr = sqlite3_column_int(stmt, col++);
        int throwDex = sqlite3_column_int(stmt, col++);
        int throwCon = sqlite3_column_int(stmt, col++);
        int throwInt = sqlite3_column_int(stmt, col++);
        int throwWis = sqlite3_column_int(stmt, col++);
        int throwCha = sqlite3_column_int(stmt, col++);

        // Temporary buffer to assemble the text
        char buffer[256] = {0};
        bool first = true;

        #define APPEND_THROW(label, value, mod) \
        if ((value) > (mod) && (value) > 0) { \
            if (!first) SDL_strlcat(buffer, ", ", sizeof(buffer)); \
            char temp[32]; \
            SDL_snprintf(temp, sizeof(temp), "%s +%d", label, value); \
            SDL_strlcat(buffer, temp, sizeof(buffer)); \
            first = false; \
        }

        if (SDL_strcmp(gAppState->CurrentStatBlock.StatName.chars, "Orc") == 0) {
            SDL_Log("throwStr: %d, Str mod: %d", throwStr, strMod);
            SDL_Log("throwDex: %d, Dex mod: %d", throwDex, dexMod);
            SDL_Log("throwCon: %d, Con mod: %d", throwCon, conMod);
            SDL_Log("throwInt: %d, Int mod: %d", throwInt, intMod);
            SDL_Log("throwWis: %d, Wis mod: %d", throwWis, wisMod);
            SDL_Log("throwCha: %d, Cha mod: %d", throwCha, chaMod);
        }

        // Build final formatted string
        APPEND_THROW("Str", throwStr, strMod);
        APPEND_THROW("Dex", throwDex, dexMod);
        APPEND_THROW("Con", throwCon, conMod);
        APPEND_THROW("Int", throwInt, intMod);
        APPEND_THROW("Wis", throwWis, wisMod);
        APPEND_THROW("Cha", throwCha, chaMod);

        // Prefix the full thing if any throws exist
        char finalBuffer[300] = {0};
        if (!first) {
            SDL_snprintf(finalBuffer, sizeof(finalBuffer), "Saving Throws %s", buffer);
        }

        // Convert to Clay_String
        gAppState->CurrentStatBlock.StatSavingThrows = MakeClayString(finalBuffer);


        gAppState->CurrentStatBlock.StatSkills            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatLanguages         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSenses            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatRangeDarkvision   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatRangeTremorsense  = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatRangeBlindsight   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatRangeTruesight    = MakeClayIntString(sqlite3_column_int(stmt, col++));

        gAppState->CurrentStatBlock.StatSpecialAbilityOne       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityOneDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityTwo       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityTwoDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityThree     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityThreeDesc= MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityFour      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatSpecialAbilityFourDesc = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack1          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack1Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack2          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack2Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack3          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack3Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack4          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack4Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatAttack5          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack5Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack6          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatAttack6Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatActionLeg        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLeg3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair1      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair1Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair2      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair2Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair3      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatActionLair3Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatRegionalEffect    = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatRegionalEffect1   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatRegionalEffect2   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatRegionalEffect3   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatEndRegionalEffect = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatEnvironment       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatBa1               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa1Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa2               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa2Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa3               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa3Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa4               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatBa4Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatReaction1         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatReaction1Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatReaction2         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatReaction2Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatReaction3         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatReaction3Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatVillAction3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatUtilitySpells      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatUtilitySpellsList  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature1           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature1Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature2           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature2Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature3           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature3Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature4           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature4Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature5           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatFeature5Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
}

DisplayListMember * LookupCreatureForCombat(const char * CreatureName, int Initiative, bool IsCreature) {
    DisplayListMember * Member = SDL_malloc(sizeof(DisplayListMember));
    SDL_memset(Member, 0, sizeof(DisplayListMember));
    Member->Name = MakeClayString(CreatureName);
    Member->Initiative = Initiative;
    Member->Next = NULL;
    Member->IsCreature = true;
    Member->HitPointsTextBox.IsInitialized = false;
    SDL_memset(Member->HitPointsTextBox.TextBoxBuffer, 0, MAX_TEXT);

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id, armor_class, hitpoints_avg FROM monsters WHERE name = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        SDL_free(Member);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, CreatureName, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        int col = 0;

        Member->SqliteDbId  = sqlite3_column_int(stmt, col++);
        Member->ArmorClass  = sqlite3_column_int(stmt, col++);
        Member->HitPoints   = sqlite3_column_int(stmt, col++);
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    return Member;

}

DisplayListMember * LookupPlayerForCombat(const char * PlayerName, int Initiative, bool IsCreature) {
    DisplayListMember * Member = SDL_malloc(sizeof(DisplayListMember));
    Member->Name = MakeClayString(PlayerName);
    Member->Initiative = Initiative;
    Member->Next = NULL;
    Member->IsCreature = false;

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id, armor_class FROM players WHERE name = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, PlayerName, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        int col = 0;

        Member->SqliteDbId  = sqlite3_column_int(stmt, col++);
        Member->ArmorClass  = sqlite3_column_int(stmt, col++);
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    return Member;
}

void SaveNewStatBlockToDB() {
    const char *saveList = StatSavingThrowsTextBox.StringToDisplay.chars;
    char buffer[256];

    strncpy(buffer, saveList, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';

    char *token;
    char *saveptr;

    int strScore = atoi(StatStrTextBox.StringToDisplay.chars);
    int dexScore = atoi(StatDexTextBox.StringToDisplay.chars);
    int conScore = atoi(StatConTextBox.StringToDisplay.chars);
    int intScore = atoi(StatIntTextBox.StringToDisplay.chars);
    int wisScore = atoi(StatWisTextBox.StringToDisplay.chars);
    int chaScore = atoi(StatChaTextBox.StringToDisplay.chars);

    int strMod = DND_MOD(strScore);
    int dexMod = DND_MOD(dexScore);
    int conMod = DND_MOD(conScore);
    int intMod = DND_MOD(intScore);
    int wisMod = DND_MOD(wisScore);
    int chaMod = DND_MOD(chaScore);

    int throwStr = 0;
    int throwDex = 0;
    int throwCon = 0;
    int throwInt = 0;
    int throwWis = 0;
    int throwCha = 0;

    token = SDL_strtok_r(buffer, ",", &saveptr);

    while (token != NULL) {

        // trim whitespace
        while (*token == ' ') token++;

        if (SDL_strcasecmp(token, "str") == 0) {
            throwStr = strMod;
        } else if (SDL_strcasecmp(token, "dex") == 0) {
            throwDex = dexMod;
        } else if (SDL_strcasecmp(token, "con") == 0) {
            throwCon = conMod;
        } else if (SDL_strcasecmp(token, "int") == 0) {
            throwInt = intMod;
        } else if (SDL_strcasecmp(token, "wis") == 0) {
            throwWis = wisMod;
        } else if (SDL_strcasecmp(token, "cha") == 0) {
            throwCha = chaMod;
        }

        token = SDL_strtok_r(NULL, ",", &saveptr);
    }

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql =
                        "INSERT INTO monsters ("
                        "cr, armor_class, hitpoints_avg, hit_dice, hitpoints_roll,"
                        "str, dex, con, int, wis, cha, prof_bonus,"
                        "throw_str, throw_dex, throw_con, throw_int, throw_wis, throw_cha,"
                        "name, type, size, speed_type, speed_walk, speed_fly, speed_swim, speed_climb, speed_burrow,"
                        "alignment, legendary, skills, languages, senses,"
                        "range_darkvision, range_tremorsense, range_blindsight, range_truesight,"
                        "special_ability_one, special_ability_one_desc,"
                        "special_ability_two, special_ability_two_desc,"
                        "special_ability_three, special_ability_three_desc,"
                        "special_ability_four, special_ability_four_desc,"
                        "attack_1, attack_1_desc, attack_2, attack_2_desc, attack_3, attack_3_desc,"
                        "attack_4, attack_4_desc, attack_5, attack_5_desc, attack_6, attack_6_desc,"
                        "action_leg, action_leg1, action_leg1_desc, action_leg2, action_leg2_desc, action_leg3, action_leg3_desc,"
                        "action_lair, action_lair1, action_lair1_desc, action_lair2, action_lair2_desc, action_lair3, action_lair3_desc,"
                        "regionaleffect, regionaleffect1, regionaleffect2, regionaleffect3, endregionaleffect,"
                        "environment,"
                        "ba1, ba1_desc, ba2, ba2_desc, ba3, ba3_desc, ba4, ba4_desc,"
                        "reaction1, reaction1_desc, reaction2, reaction2_desc, reaction3, reaction3_desc,"
                        "vill_action, vill_action1, vill_action1_desc, vill_action2, vill_action2_desc, vill_action3, vill_action3_desc,"
                        "utility_spells, utility_spells_list,"
                        "feature1, feature1_desc, feature2, feature2_desc, feature3, feature3_desc, feature4, feature4_desc, feature5, feature5_desc"
                        ") VALUES ("
                        "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?"
                        ");";

    rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    for (int i = 0; i < NUMBER_INTEGER_FIELDS; i++) {
        if (NULL != IntegerFields[i]->StringToDisplay.chars) {
            rc = sqlite3_bind_int(stmt, i + 1, atoi(IntegerFields[i]->StringToDisplay.chars));
            SDL_Log("Binding integer field %d with value %d", i + 1, atoi(IntegerFields[i]->StringToDisplay.chars));
        }
        else {
            rc = sqlite3_bind_int(stmt, i + 1, 0);
            SDL_Log("Null entity: Binding integer field %d with value 0", i + 1);
        }
    }

    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 1, throwStr);
    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 2, throwDex);
    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 3, throwCon);
    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 4, throwInt);
    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 5, throwWis);
    rc = sqlite3_bind_int(stmt, NUMBER_INTEGER_FIELDS + 6, throwCha);

    for (int i = 0; i < NUMBER_TEXT_FIELDS; i++) {
        if(NULL == TextFields[i]->StringToDisplay.chars || SDL_strcmp(TextFields[i]->StringToDisplay.chars, "") == 0) {
            rc = sqlite3_bind_text(stmt, i + 1 + NUMBER_INTEGER_FIELDS + 6, "", -1, SQLITE_TRANSIENT);
            SDL_Log("Null entity: Binding text field %d with empty string", i + 1 + NUMBER_INTEGER_FIELDS + 6);
        }
        else {
            rc = sqlite3_bind_text(stmt, i + 1 + NUMBER_INTEGER_FIELDS + 6, TextFields[i]->StringToDisplay.chars, -1, SQLITE_TRANSIENT);
            SDL_Log("Binding text field %d with value %s", i + 1 + NUMBER_INTEGER_FIELDS + 6, TextFields[i]->StringToDisplay.chars);
            
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }
    else
    {
        SDL_Log("New creature added successfully.");
    }

    sqlite3_finalize(stmt);

}

void StructureFieldArrays() {
    IntegerFields[INT_CR]               = &StatCrTextBox;
    IntegerFields[INT_ARMOR_CLASS]      = &StatArmorClassTextBox;
    IntegerFields[INT_HITPOINTS_AVG]    = &StatHitpointsAvgTextBox;
    IntegerFields[INT_HIT_DICE]         = &StatHitDiceTextBox;
    IntegerFields[INT_HITPOINTS_ROLL]   = &StatHitpointsRollTextBox;
    IntegerFields[INT_STR]              = &StatStrTextBox;
    IntegerFields[INT_DEX]              = &StatDexTextBox;
    IntegerFields[INT_CON]              = &StatConTextBox;
    IntegerFields[INT_INT]              = &StatIntTextBox;
    IntegerFields[INT_WIS]              = &StatWisTextBox;
    IntegerFields[INT_CHA]              = &StatChaTextBox;
    IntegerFields[INT_PROF_BONUS]       = &StatProfBonusTextBox;

    TextFields[TXT_NAME]                        = &StatNameTextBox;
    TextFields[TXT_TYPE]                        = &StatTypeTextBox;
    TextFields[TXT_SIZE]                        = &StatSizeTextBox;
    TextFields[TXT_SPEED_TYPE]                  = &StatSpeedTypeTextBox;
    TextFields[TXT_SPEED_WALK]                  = &StatSpeedWalkTextBox;
    TextFields[TXT_SPEED_FLY]                   = &StatSpeedFlyTextBox;
    TextFields[TXT_SPEED_SWIM]                  = &StatSpeedSwimTextBox;
    TextFields[TXT_SPEED_CLIMB]                 = &StatSpeedClimbTextBox;
    TextFields[TXT_SPEED_BURROW]                = &StatSpeedBurrowTextBox;
    TextFields[TXT_ALIGNMENT]                   = &StatAlignmentTextBox;
    TextFields[TXT_LEGENDARY]                   = &StatLegendaryTextBox;
    TextFields[TXT_SKILLS]                      = &StatSkillsTextBox;
    TextFields[TXT_LANGUAGES]                   = &StatLanguagesTextBox;
    TextFields[TXT_SENSES]                      = &StatSensesTextBox;
    TextFields[TXT_RANGE_DARKVISION]            = &StatRangeDarkvisionTextBox;
    TextFields[TXT_RANGE_TREMORSENSE]           = &StatRangeTremorsenseTextBox;
    TextFields[TXT_RANGE_BLINDSIGHT]            = &StatRangeBlindsightTextBox;
    TextFields[TXT_RANGE_TRUESIGHT]             = &StatRangeTruesightTextBox;
    TextFields[TXT_SPECIAL_ABILITY_ONE]         = &StatSpecialAbilityOneTextBox;
    TextFields[TXT_SPECIAL_ABILITY_ONE_DESC]    = &StatSpecialAbilityOneDescTextBox;
    TextFields[TXT_SPECIAL_ABILITY_TWO]         = &StatSpecialAbilityTwoTextBox;
    TextFields[TXT_SPECIAL_ABILITY_TWO_DESC]    = &StatSpecialAbilityTwoDescTextBox;
    TextFields[TXT_SPECIAL_ABILITY_THREE]       = &StatSpecialAbilityThreeTextBox;
    TextFields[TXT_SPECIAL_ABILITY_THREE_DESC]  = &StatSpecialAbilityThreeDescTextBox;
    TextFields[TXT_SPECIAL_ABILITY_FOUR]        = &StatSpecialAbilityFourTextBox;
    TextFields[TXT_SPECIAL_ABILITY_FOUR_DESC]   = &StatSpecialAbilityFourDescTextBox;
    TextFields[TXT_ATTACK_1]                    = &StatAttack1TextBox;
    TextFields[TXT_ATTACK_1_DESC]               = &StatAttack1DescTextBox;
    TextFields[TXT_ATTACK_2]                    = &StatAttack2TextBox;
    TextFields[TXT_ATTACK_2_DESC]               = &StatAttack2DescTextBox;
    TextFields[TXT_ATTACK_3]                    = &StatAttack3TextBox;
    TextFields[TXT_ATTACK_3_DESC]               = &StatAttack3DescTextBox;
    TextFields[TXT_ATTACK_4]                    = &StatAttack4TextBox;
    TextFields[TXT_ATTACK_4_DESC]               = &StatAttack4DescTextBox;
    TextFields[TXT_ATTACK_5]                    = &StatAttack5TextBox;
    TextFields[TXT_ATTACK_5_DESC]               = &StatAttack5DescTextBox;
    TextFields[TXT_ATTACK_6]                    = &StatAttack6TextBox;
    TextFields[TXT_ATTACK_6_DESC]               = &StatAttack6DescTextBox;
    TextFields[TXT_ACTION_LEG]                  = &StatActionLegTextBox;
    TextFields[TXT_ACTION_LEG1]                 = &StatActionLeg1TextBox;
    TextFields[TXT_ACTION_LEG1_DESC]            = &StatActionLeg1DescTextBox;
    TextFields[TXT_ACTION_LEG2]                 = &StatActionLeg2TextBox;
    TextFields[TXT_ACTION_LEG2_DESC]            = &StatActionLeg2DescTextBox;
    TextFields[TXT_ACTION_LEG3]                 = &StatActionLeg3TextBox;
    TextFields[TXT_ACTION_LEG3_DESC]            = &StatActionLeg3DescTextBox;
    TextFields[TXT_ACTION_LAIR]                 = &StatActionLairTextBox;
    TextFields[TXT_ACTION_LAIR1]                = &StatActionLair1TextBox;
    TextFields[TXT_ACTION_LAIR1_DESC]           = &StatActionLair1DescTextBox;
    TextFields[TXT_ACTION_LAIR2]                = &StatActionLair2TextBox;
    TextFields[TXT_ACTION_LAIR2_DESC]           = &StatActionLair2DescTextBox;
    TextFields[TXT_ACTION_LAIR3]                = &StatActionLair3TextBox;
    TextFields[TXT_ACTION_LAIR3_DESC]           = &StatActionLair3DescTextBox;
    TextFields[TXT_REGIONAL_EFFECT]             = &StatRegionalEffectTextBox;
    TextFields[TXT_REGIONAL_EFFECT1]            = &StatRegionalEffect1TextBox;
    TextFields[TXT_REGIONAL_EFFECT2]            = &StatRegionalEffect2TextBox;
    TextFields[TXT_REGIONAL_EFFECT3]            = &StatRegionalEffect3TextBox;
    TextFields[TXT_END_REGIONAL_EFFECT]         = &StatEndRegionalEffectTextBox;
    TextFields[TXT_ENVIRONMENT]                 = &StatEnvironmentTextBox;
    TextFields[TXT_BA1]                         = &StatBa1TextBox;
    TextFields[TXT_BA1_DESC]                    = &StatBa1DescTextBox;
    TextFields[TXT_BA2]                         = &StatBa2TextBox;
    TextFields[TXT_BA2_DESC]                    = &StatBa2DescTextBox;
    TextFields[TXT_BA3]                         = &StatBa3TextBox;
    TextFields[TXT_BA3_DESC]                    = &StatBa3DescTextBox;
    TextFields[TXT_BA4]                         = &StatBa4TextBox;
    TextFields[TXT_BA4_DESC]                    = &StatBa4DescTextBox;
    TextFields[TXT_REACTION1]                   = &StatReaction1TextBox;
    TextFields[TXT_REACTION1_DESC]              = &StatReaction1DescTextBox;
    TextFields[TXT_REACTION2]                   = &StatReaction2TextBox;
    TextFields[TXT_REACTION2_DESC]              = &StatReaction2DescTextBox;
    TextFields[TXT_REACTION3]                   = &StatReaction3TextBox;
    TextFields[TXT_REACTION3_DESC]              = &StatReaction3DescTextBox;
    TextFields[TXT_VILL_ACTION]                 = &StatVillActionTextBox;
    TextFields[TXT_VILL_ACTION1]                = &StatVillAction1TextBox;
    TextFields[TXT_VILL_ACTION1_DESC]           = &StatVillAction1DescTextBox;
    TextFields[TXT_VILL_ACTION2]                = &StatVillAction2TextBox;
    TextFields[TXT_VILL_ACTION2_DESC]           = &StatVillAction2DescTextBox;
    TextFields[TXT_VILL_ACTION3]                = &StatVillAction3TextBox;
    TextFields[TXT_VILL_ACTION3_DESC]           = &StatVillAction3DescTextBox;
    TextFields[TXT_UTILITY_SPELLS]              = &StatUtilitySpellsTextBox;
    TextFields[TXT_UTILITY_SPELLS_LIST]         = &StatUtilitySpellsListTextBox;
    TextFields[TXT_FEATURE1]                    = &StatFeature1TextBox;
    TextFields[TXT_FEATURE1_DESC]               = &StatFeature1DescTextBox;
    TextFields[TXT_FEATURE2]                    = &StatFeature2TextBox;
    TextFields[TXT_FEATURE2_DESC]               = &StatFeature2DescTextBox;
    TextFields[TXT_FEATURE3]                    = &StatFeature3TextBox;
    TextFields[TXT_FEATURE3_DESC]               = &StatFeature3DescTextBox;
    TextFields[TXT_FEATURE4]                    = &StatFeature4TextBox;
    TextFields[TXT_FEATURE4_DESC]               = &StatFeature4DescTextBox;
    TextFields[TXT_FEATURE5]                    = &StatFeature5TextBox;
    TextFields[TXT_FEATURE5_DESC]               = &StatFeature5DescTextBox;
}

void SaveEncounterToDB() {

    sqlite3_int64 EncounterID = InsertEncounterNameToDB();

    if (EncounterID == -1) {
        return;
    }

    int Result = InsertEncounterCreaturesToDB(EncounterID);

    if (Result == -1) {
        return;
        
    }

    InsertEncounterPlayersToDB(EncounterID);
}

sqlite3_int64 InsertEncounterNameToDB() {

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "INSERT INTO encounters (encounter_name) VALUES (?);";

    rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 1, EncounterName.StringToDisplay.chars, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to bind name: %s", sqlite3_errmsg(pGuidnbatterDB));
        sqlite3_finalize(stmt);
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }
    else
    {
        SDL_Log("Encounter added successfully.");
    }

    sqlite3_int64 EncounterID = sqlite3_last_insert_rowid(pGuidnbatterDB);

    sqlite3_finalize(stmt);

    return EncounterID;
}

int InsertEncounterCreaturesToDB(sqlite3_int64 EncounterID) {

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "INSERT INTO encountercreatures (encounter_id, creature_name, initiative, quantity) VALUES (?,?,?,?);";

    rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }

    for (int i = 0; i < BUILD_LIST_MAX; i++) {
        if ('\0' != BuildListMembers[i].name[0] && !BuildListMembers[i].IsAdded) {
            if (!BuildListMembers[i].IsCreature) {
                continue;
            }
            sqlite3_bind_int64(stmt, 1, EncounterID);
            sqlite3_bind_text(stmt, 2, BuildListMembers[i].name, -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, BuildListMembers[i].initiative);
            sqlite3_bind_int(stmt, 4, BuildListMembers[i].Quantity);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                SDL_Log("Failed to insert encounter creature: %s", sqlite3_errmsg(pGuidnbatterDB));
                sqlite3_finalize(stmt);
                return -1;
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }

    SDL_Log("Encounter added successfully.");

    sqlite3_finalize(stmt);

    return 0;
}

void InsertEncounterPlayersToDB(sqlite3_int64 EncounterID) {

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "INSERT INTO encounterplayers (encounter_id, player_name, initiative) VALUES (?,?,?);";

    rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    for (int i = 0; i < BUILD_LIST_MAX; i++) {
        if ('\0' != BuildListMembers[i].name[0] && !BuildListMembers[i].IsAdded) {

            if (BuildListMembers[i].IsCreature) {
                continue;
            }

            sqlite3_bind_int64(stmt, 1, EncounterID);
            sqlite3_bind_text(stmt, 2, BuildListMembers[i].name, -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, BuildListMembers[i].initiative);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                SDL_Log("Failed to insert encounter creature: %s", sqlite3_errmsg(pGuidnbatterDB));
                sqlite3_finalize(stmt);
                return;
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }

    SDL_Log("Encounter added successfully.");

    sqlite3_finalize(stmt);
    return;
}

static const char * BoundedStrStr(const char * haystack, const char * needle, size_t haystackLen, size_t NeedleLen) {
    if (NeedleLen == 0) {
        return NULL;
    }
    if (NeedleLen > haystackLen) {
        return NULL;
    }
    for (int i = 0; i <= haystackLen - NeedleLen; i++) {
        if (SDL_strncasecmp(&haystack[i], needle, NeedleLen) == 0) {
            return &haystack[i];
        }
    }
    return NULL;

}

void SearchCreatureNames(const char * SearchTerm) {
    size_t SearchTermLen = SDL_strlen(SearchTerm);
    if (0 == SearchTermLen) {
        for (int i = 0; i <TotalCreatures; i++) {
            HeadersToShow[i] = i;
        }
        return;
    }
    for (int i = 0; i < MAX_DB_COUNT; i++) {
        if (DBPageHeaders[i].CreatureName.chars != NULL) {
            size_t SearchTermLen = SDL_strlen(SearchTerm);
            size_t CreatureNameLen = DBPageHeaders[i].CreatureName.length;
            if (NULL != BoundedStrStr(DBPageHeaders[i].CreatureName.chars, SearchTerm, CreatureNameLen, SearchTermLen)) {
                SDL_Log("Found match: %s", DBPageHeaders[i].CreatureName.chars);
                HeadersToShow[i] = i;
            }
            else {
                HeadersToShow[i] = -1;
            }
        }
    }
}

void ModifyTypedString(void) {

    // Error check to ensure there is something to delete
    if (0 == SDL_strlen(TextBuffer)) {
        return;
    }

    size_t i = SDL_strlen(TextBuffer) - 1;

    // Move backward to the start byte of the last UTF-8 character
    while (i > 0 && (TextBuffer[i] & 0xC0) == 0x80) {
        // Continuation byte (10xxxxxx), skip backwards
        i--;
    }

    // i now points to the start of the last UTF-8 character
    TextBuffer[i] = '\0';
}