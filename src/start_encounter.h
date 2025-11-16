#ifndef START_ENCOUNTER_H
#define START_ENCOUNTER_H

#include "clay.h"
#include "global.h"

/*========================================================================* 
 *  SECTION - Global variables
 *========================================================================* 
 */
#define START_NEW_ENCOUNTER_SCREEN  3

 /*========================================================================* 
 *  SECTION - Global variables
 *========================================================================* 
 */
extern int BuildingEncounter;
extern int StartEncounterState;

 /*========================================================================* 
 *  SECTION - Global prototypes
 *========================================================================* 
 */
void StartEncounterWindow();
void NewEncounterScreen();

#endif /* START_ENCOUNTER_H */