#ifndef DB_QUERY
#define DB_QUERY
#include "sql/sqlite3.h"
#include <SDL3/SDL.h>

extern sqlite3 * pGuidnbatterDB;

void DatabaseOpen(void);
void DatabaseClose(void);

#endif /* DB_QUERY */