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