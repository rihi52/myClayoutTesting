#include "db_query.h"
#include "global.h"

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;

CreatureHeader DBPageHeaders[MAX_DB_COUNT] = {0};

int HeadersToShow[MAX_DB_COUNT];

static const char * BoundedStrStr(const char * haystack, const char * needle, size_t haystackLen, size_t NeedleLen);

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

Clay_String MakeClayString(const char * string) {
    char * copy = SDL_malloc(SDL_strlen(string)+1);

    SDL_strlcpy(copy, string, SDL_strlen(string)+1);

    Clay_String str= {
        .isStaticallyAllocated = false,
        .length = SDL_strlen(copy),
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
        gAppState->CurrentStatBlock.StatSpeedWalk         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatSpeedFly          = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatSpeedSwim         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatSpeedClimb        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatSpeedBurrow       = MakeClayIntString(sqlite3_column_int(stmt, col++));

        gAppState->CurrentStatBlock.StatAlignment         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        gAppState->CurrentStatBlock.StatLegendary         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        gAppState->CurrentStatBlock.StatStr               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatDex               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatCon               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatInt               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatWis               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        gAppState->CurrentStatBlock.StatCha               = MakeClayIntString(sqlite3_column_int(stmt, col++));

        gAppState->CurrentStatBlock.StatProfBonus         = MakeClayIntString(sqlite3_column_int(stmt, col++));

        int throwStr = sqlite3_column_int(stmt, col++);
        int throwDex = sqlite3_column_int(stmt, col++);
        int throwCon = sqlite3_column_int(stmt, col++);
        int throwInt = sqlite3_column_int(stmt, col++);
        int throwWis = sqlite3_column_int(stmt, col++);
        int throwCha = sqlite3_column_int(stmt, col++);

        // Temporary buffer to assemble the text
        char buffer[256] = {0};
        bool first = true;

        #define APPEND_THROW(label, value) \
            if (value != 0) { \
                if (!first) SDL_strlcat(buffer, ", ", sizeof(buffer)); \
                char temp[32]; \
                SDL_snprintf(temp, sizeof(temp), "%s +%d", label, value); \
                SDL_strlcat(buffer, temp, sizeof(buffer)); \
                first = false; \
            }

        // Build final formatted string
        APPEND_THROW("Str", throwStr);
        APPEND_THROW("Dex", throwDex);
        APPEND_THROW("Con", throwCon);
        APPEND_THROW("Int", throwInt);
        APPEND_THROW("Wis", throwWis);
        APPEND_THROW("Cha", throwCha);

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
    Member->Name = MakeClayString(CreatureName);
    Member->Initiative = Initiative;
    Member->Next = NULL;

    if (!IsCreature) {
        Member->IsCreature = false;
        return Member;
    }

    Member->IsCreature = true;

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id, armor_class, hitpoints_avg FROM monsters WHERE name = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, CreatureName, -1, SQLITE_STATIC);

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

    return Member;

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