#ifndef ENCOUNTER_DB_H
#define ENCOUNTER_DB_H

#include "clay.h"
#include "stdio.h"
#include "stdint.h"
#include "global.h"

/*========================================================================* 
 *  SECTION - Defines
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Global Prototypes
 *========================================================================* 
 */
void EncounterDatabaseWindow(AppState * state);
void MakeEncounterHeader(int i, int CallingWindow);

 #endif /* ENCOUNTER_DB_H */