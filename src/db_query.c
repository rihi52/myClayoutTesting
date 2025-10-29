#include "db_query.h"

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;


CreatureHeader DBPageHeaders[333];
static CreatureHeader LookUp;

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
    Clay_String str = {
        .isStaticallyAllocated = true,
        .length = sizeof(string),
        .chars = string
    };

    return str;
}

CreatureHeader * LoadCreatureHeaderAlphabetical(int MonsterId) {
    
    
    memset(&LookUp, 0, sizeof(LookUp));

    static char name_buffer[128]; // Persistent buffer for returned name
    name_buffer[0] = '\0'; // Clear it

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT name, cr, size, type FROM monsters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, MonsterId);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const char *Name    = sqlite3_column_text(stmt, 0);
        const char *Cr      = sqlite3_column_text(stmt, 1);
        const char *Size    = sqlite3_column_text(stmt, 2);
        const char *Type    = sqlite3_column_text(stmt, 3);
        //const char *Source  = sqlite3_column_text(stmt, 4);

        SDL_strlcpy(LookUp.CreatureName, Name, sizeof(LookUp.CreatureName));
        SDL_strlcpy(LookUp.CreatureCR, Cr, sizeof(LookUp.CreatureCR));
        SDL_strlcpy(LookUp.CreatureSize, Size, sizeof(LookUp.CreatureSize));
        SDL_strlcpy(LookUp.CreatureType, Type, sizeof(LookUp.CreatureType));
        //SDL_strlcpy(Source, LookUp.CreatureSource, sizeof(LookUp.CreatureSource));
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    return &LookUp; // Safe static buffer
}