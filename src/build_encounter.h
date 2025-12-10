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
typedef struct BuildListMember {
    int initiative;
    char name[BUILD_LIST_MAX][64];
    int Quantity;
    bool IsCreature;
}BuildListMember;

/*========================================================================* 
 *  SECTION - Extern variables
 *========================================================================* 
 */
extern BuildListMember BuildListMembers[BUILD_LIST_MAX];

/*========================================================================*
 *  SECTION - Global prototypes
 *========================================================================*
 */
void BuildEncounterWindow(AppState * state, int CallingScreen);
void PlayerBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void CreatureBuildListCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

#endif /* BUILD_ENCOUNTER_H */