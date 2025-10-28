#include "db_query.h"

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void DatabaseOpen(void) {

    int rc = sqlite3_open("guidnbatter.db", &pGuidnbatterDB);
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