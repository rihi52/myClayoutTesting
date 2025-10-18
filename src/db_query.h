#ifndef DB_QUERY
#define DB_QUERY
#include "sql/sqlite3.h"
#include <SDL3/SDL.h>
#include "clay.h"

#define MAX_DB_COUNT 1000

extern sqlite3 * pGuidnbatterDB;

typedef struct CreatureHeader {
Clay_String CreatureName;
Clay_String CreatureCR;
Clay_String CreatureSize;
Clay_String CreatureType;
Clay_String CreatureSource;
}CreatureHeader;

extern CreatureHeader DBPageHeaders[MAX_DB_COUNT];

void DatabaseOpen(void);
void DatabaseClose(void);
int LoadCreatureHeaderAlphabetical(int MonsterId);
Clay_String MakeClayString(const char * string);

#endif /* DB_QUERY */