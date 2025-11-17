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
    ClearClayString(&StatId);
    ClearClayString(&StatName);
    ClearClayString(&StatCr);
    ClearClayString(&StatType);
    ClearClayString(&StatSize);

    ClearClayString(&StatArmorClass);
    ClearClayString(&StatHitpointsAvg);
    ClearClayString(&StatHitDice);
    ClearClayString(&StatHitpointsRoll);

    ClearClayString(&StatSpeedType);
    ClearClayString(&StatSpeedWalk);
    ClearClayString(&StatSpeedFly);
    ClearClayString(&StatSpeedSwim);
    ClearClayString(&StatSpeedClimb);
    ClearClayString(&StatSpeedBurrow);

    ClearClayString(&StatAlignment);
    ClearClayString(&StatLegendary);

    ClearClayString(&StatStr);
    ClearClayString(&StatDex);
    ClearClayString(&StatCon);
    ClearClayString(&StatInt);
    ClearClayString(&StatWis);
    ClearClayString(&StatCha);

    ClearClayString(&StatProfBonus);

    ClearClayString(&StatThrowStr);
    ClearClayString(&StatThrowDex);
    ClearClayString(&StatThrowCon);
    ClearClayString(&StatThrowInt);
    ClearClayString(&StatThrowWis);
    ClearClayString(&StatThrowCha);

    ClearClayString(&StatSavingThrows);

    ClearClayString(&StatSkills);
    ClearClayString(&StatLanguages);
    ClearClayString(&StatSenses);

    ClearClayString(&StatRangeDarkvision);
    ClearClayString(&StatRangeTremorsense);
    ClearClayString(&StatRangeBlindsight);
    ClearClayString(&StatRangeTruesight);

    ClearClayString(&StatSpecialAbilityOne);
    ClearClayString(&StatSpecialAbilityOneDesc);
    ClearClayString(&StatSpecialAbilityTwo);
    ClearClayString(&StatSpecialAbilityTwoDesc);
    ClearClayString(&StatSpecialAbilityThree);
    ClearClayString(&StatSpecialAbilityThreeDesc);
    ClearClayString(&StatSpecialAbilityFour);
    ClearClayString(&StatSpecialAbilityFourDesc);

    ClearClayString(&StatAttack1);
    ClearClayString(&StatAttack1Desc);
    ClearClayString(&StatAttack2);
    ClearClayString(&StatAttack2Desc);
    ClearClayString(&StatAttack3);
    ClearClayString(&StatAttack3Desc);
    ClearClayString(&StatAttack4);
    ClearClayString(&StatAttack4Desc);

    ClearClayString(&StatAttack5);
    ClearClayString(&StatAttack5Desc);

    ClearClayString(&StatAttack6);
    ClearClayString(&StatAttack6Desc);

    ClearClayString(&StatActionLeg);
    ClearClayString(&StatActionLeg1);
    ClearClayString(&StatActionLeg1Desc);
    ClearClayString(&StatActionLeg2);
    ClearClayString(&StatActionLeg2Desc);
    ClearClayString(&StatActionLeg3);
    ClearClayString(&StatActionLeg3Desc);

    ClearClayString(&StatActionLair);
    ClearClayString(&StatActionLair1);
    ClearClayString(&StatActionLair1Desc);
    ClearClayString(&StatActionLair2);
    ClearClayString(&StatActionLair2Desc);
    ClearClayString(&StatActionLair3);
    ClearClayString(&StatActionLair3Desc);

    ClearClayString(&StatRegionalEffect);
    ClearClayString(&StatRegionalEffect1);
    ClearClayString(&StatRegionalEffect2);
    ClearClayString(&StatRegionalEffect3);
    ClearClayString(&StatEndRegionalEffect);

    ClearClayString(&StatEnvironment);

    ClearClayString(&StatBa1);
    ClearClayString(&StatBa1Desc);
    ClearClayString(&StatBa2);
    ClearClayString(&StatBa2Desc);
    ClearClayString(&StatBa3);
    ClearClayString(&StatBa3Desc);
    ClearClayString(&StatBa4);
    ClearClayString(&StatBa4Desc);

    ClearClayString(&StatReaction1);
    ClearClayString(&StatReaction1Desc);
    ClearClayString(&StatReaction2);
    ClearClayString(&StatReaction2Desc);
    ClearClayString(&StatReaction3);
    ClearClayString(&StatReaction3Desc);

    ClearClayString(&StatVillAction);
    ClearClayString(&StatVillAction1);
    ClearClayString(&StatVillAction1Desc);
    ClearClayString(&StatVillAction2);
    ClearClayString(&StatVillAction2Desc);
    ClearClayString(&StatVillAction3);
    ClearClayString(&StatVillAction3Desc);

    ClearClayString(&StatUtilitySpells);
    ClearClayString(&StatUtilitySpellsList);

    ClearClayString(&StatFeature1);
    ClearClayString(&StatFeature1Desc);
    ClearClayString(&StatFeature2);
    ClearClayString(&StatFeature2Desc);
    ClearClayString(&StatFeature3);
    ClearClayString(&StatFeature3Desc);
    ClearClayString(&StatFeature4);
    ClearClayString(&StatFeature4Desc);
    ClearClayString(&StatFeature5);
    ClearClayString(&StatFeature5Desc);
    
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

        StatId                = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatName              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatCr                = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatType              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSize              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatArmorClass        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatHitpointsAvg      = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatHitDice           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatHitpointsRoll     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatSpeedType         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpeedWalk         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedFly          = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedSwim         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedClimb        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedBurrow       = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatAlignment         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatLegendary         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatStr               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatDex               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatCon               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatInt               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatWis               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatCha               = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatProfBonus         = MakeClayIntString(sqlite3_column_int(stmt, col++));

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
        StatSavingThrows = MakeClayString(finalBuffer);


        StatSkills            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatLanguages         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSenses            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatRangeDarkvision   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeTremorsense  = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeBlindsight   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeTruesight    = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatSpecialAbilityOne       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityOneDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityTwo       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityTwoDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityThree     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityThreeDesc= MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityFour      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityFourDesc = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatAttack1          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack1Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack2          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack2Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack3          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack3Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack4          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack4Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatAttack5          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack5Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack6          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack6Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatActionLeg        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatActionLair       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair1      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair1Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair2      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair2Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair3      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair3Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatRegionalEffect    = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect1   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect2   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect3   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatEndRegionalEffect = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatEnvironment       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatBa1               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa1Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa2               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa2Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa3               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa3Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa4               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa4Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatReaction1         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction1Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction2         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction2Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction3         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction3Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatVillAction        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatUtilitySpells      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatUtilitySpellsList  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatFeature1           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature1Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature2           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature2Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature3           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature3Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature4           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature4Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature5           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature5Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
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
    if (TypedText.length == 0) {
        return;
    }

    size_t i = TypedText.length - 1;

    // Move backward to the start byte of the last UTF-8 character
    while (i > 0 && (TextBuffer[i] & 0xC0) == 0x80) {
        // Continuation byte (10xxxxxx), skip backwards
        i--;
    }

    // i now points to the start of the last UTF-8 character
    TypedText.length = i;
    TextBuffer[i] = '\0';
}