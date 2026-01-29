#ifndef BUILD_ENCOUNTER_H
#define BUILD_ENCOUNTER_H

#include "clay.h"
#include "global.h"

/*========================================================================* 
 *  SECTION - Defines
 *========================================================================* 
 */


/*========================================================================* 
 *  SECTION - Global structs
 *========================================================================* 
 */


/*========================================================================* 
 *  SECTION - Extern variables
 *========================================================================* 
 */


/*========================================================================*
 *  SECTION - Global prototypes
 *========================================================================*
 */
void BuildEncounterWindow(AppState * state, int CallingScreen);
void AddToBuildChain(const char *ParticipantToAdd, bool IsCreature);

void PlayerBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void CreatureBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

#endif /* BUILD_ENCOUNTER_H */