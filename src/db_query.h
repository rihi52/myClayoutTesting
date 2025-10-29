#ifndef DB_QUERY
#define DB_QUERY
#include "sql/sqlite3.h"
#include <SDL3/SDL.h>
#include "clay.h"

extern sqlite3 * pGuidnbatterDB;

typedef struct CreatureHeader {
char CreatureName[128];
char CreatureCR[4];
char CreatureSize[32];
char CreatureType[64];
char CreatureSource[64];
}CreatureHeader;

void DatabaseOpen(void);
void DatabaseClose(void);
CreatureHeader * LoadCreatureHeaderAlphabetical(int MonsterId);
Clay_String MakeClayString(const char * string);

#endif /* DB_QUERY */