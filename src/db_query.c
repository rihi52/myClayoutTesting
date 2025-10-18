#include "db_query.h"

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;

CreatureHeader DBPageHeaders[MAX_DB_COUNT] = {0};

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

    // for (int i = 0; i < MAX_DB_COUNT; i++) {
    //     DBPageHeaders[i] = {0};
    // }
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