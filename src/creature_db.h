#ifndef CREATURE_DB_H
#define CREATURE_DB_H

#include "clay.h"
#include "global.h"

/*========================================================================* 
 *  SECTION - Defines
 *========================================================================* 
 */

 /*========================================================================* 
 *  SECTION - Structs
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void CreatureDatabaseWindow(AppState * state);
void MakeCreatureHeader(int i, int CallingWindow);
void FillStats(void);

#endif /* CREATURE_DB_H */