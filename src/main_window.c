#include "main_window.h"
#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"
#include "db_query.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */

static void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
static void SearchButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);

void BuildEncounterWindow(AppState * state);
void CreatureDatabaseWindow(AppState * state);
void PlayerDatabaseWindow(AppState * state);
void MakeCreatureHeader(int i);
void FillStats(void);

/*========================================================================* 
 *  SECTION - Local variables
 *========================================================================* 
 */

char * CreatureNames[7] = {"Aboleth", "Acolyte", "Adult Black Dragon", "Adult Blue Dragon", "Air Elemental", "Ape", "Azer"};
char * CreatureCR[7] = {"CR 10", "CR 10", "CR 10", "CR 10", "CR 10", "CR 10", "CR 10"};
char * CreatureSize[7] = {"Large", "Large", "Large", "Large", "Large", "Large", "Large"};
char * CreatureType[7] = {"Aberration", "Aberration", "Aberration", "Aberration", "Aberration", "Aberration", "Aberration"};
char * CreatureSource[7] = {"SRD", "SRD", "SRD", "SRD", "SRD", "SRD", "SRD"};

int FirstVisible = 0;
int ViewHeight = 0;

/*========================================================================* 
 *  SECTION - Global Functions 
 *========================================================================* 
 */
Clay_RenderCommandArray MainWindow(AppState * state)
{
    Clay_BeginLayout();

    // Define one element that covers the whole screen
    CLAY(CLAY_ID("OuterContainer"), {MainScreenLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        switch (WindowState){
            case 0:
            /* Main label */
            CLAY_TEXT(CLAY_STRING("GUIDNBATTER"), CLAY_TEXT_CONFIG(MainLabelTextConfig));
            /* Start button */
            CLAY(CLAY_ID("StartButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Start Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(StartEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Build button */
            CLAY(CLAY_ID("BuildButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Build Encounter"), CLAY_TEXT_CONFIG(ButtonTextConfig)); 
                Clay_OnHover(BuildEncounterButtonCallback, (intptr_t)WindowState);
            };

            /* Creature DB button */
            CLAY(CLAY_ID("CreatureDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Creature Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(CreatureDatabaseButtonCallback, (intptr_t)WindowState);
            };

            /* Player DB button */ 
            CLAY(CLAY_ID("PlayerDatabaseButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                CLAY_TEXT(CLAY_STRING("Player Database"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                Clay_OnHover(PlayerDatabaseButtonCallback, (intptr_t)WindowState);
                
            };
            break;

        case START_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("StartEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_BLUE}) {
                Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
            };
            break;
        
        case BUILD_ENCOUNTER_SCREEN:
            CLAY(CLAY_ID("BuildEncounterHeader"), { HeadLabelWindow,.cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_ORANGE}) {
                Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
            };
            break;
        
        case CREATURE_DB_SCREEN:
            CreatureDatabaseWindow(state);
            break;

        case PLAYER_DB_SCREEN:
            PlayerDatabaseWindow(state);            
            break;

        case ADD_STAT_SCREEN:
            CreatureDatabaseWindow(state);
            break;

        default:
            break;
        }
    };

    return Clay_EndLayout();
}

/*========================================================================* 
 *  SECTION - Local Functions 
 *========================================================================*
 */

void BuildEncounterWindow(AppState * state) {

}

void CreatureDatabaseWindow(AppState * state) {
    /* Creature database window*/
    
    CLAY(CLAY_ID("CreatureDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("CreatureDBSidebar"), {SidebarLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("CreatureDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("CreatureDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomLayoutConfig ) {
                CLAY(CLAY_ID("CreatureTextBox"), {
                                                    SingleLineInputLayoutConfig,
                                                    .backgroundColor = COLOR_GRAY_BG,
                                                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                                                    .border = {
                                                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                                                .color = COLOR_WHITE
                                                            }
                                                }){
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(TypedText, CLAY_TEXT_CONFIG(InputTextConfig));
                    
                };

                CLAY(CLAY_ID("CreatureDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, (intptr_t)WindowState);
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("CreatureDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            CLAY(CLAY_ID("CreatureHeaderContainer"), {  
                CreatureButtonContainerLayoutConfig,
                .backgroundColor = COLOR_TRANSPARENT,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
                .clip = {true, true, Clay_GetScrollOffset()}
            }) {
                for (int i = 0; i < TotalCreatures; i++) {
                    if (HeadersToShow[i] != -1) {
                        MakeCreatureHeader(i);
                    }                    
                }
            }
            if (WindowState == ADD_STAT_SCREEN) {
                FillStats();
                
            }
        };
    };
}

void PlayerDatabaseWindow(AppState * state) {
    /* Player database window*/
    CLAY(CLAY_ID("PlayerDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)} ) {
        
        /* Sidebar for option buttons */
        CLAY(CLAY_ID("PlayerDBSidebar"), SidebarLayoutConfig) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("PlayerDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState);
                    CLAY_TEXT(CLAY_STRING("Return Home"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Add"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Remove"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
                CLAY(CLAY_ID("PlayerDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    CLAY_TEXT(CLAY_STRING("Edit"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };
            };
            
            CLAY(CLAY_ID("SidebarBottom"), SidebarBottomLayoutConfig) {
                CLAY(CLAY_ID("PlayerTextBox"), {
                                                    SingleLineInputLayoutConfig,
                                                    .backgroundColor = COLOR_GRAY_BG,
                                                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                                                    .border = {
                                                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                                                .color = COLOR_WHITE
                                                            }
                                                }){

                    /* Create char* and set equal to the overall buffer that reads keyboard input */
                    char * SearchText = &TextBuffer[0];
                    /* Custom clay_string to allow for a dynamically changing char* */
                    Clay_String TypedText = {.isStaticallyAllocated = true, .length = SDL_strlen(SearchText), .chars = SearchText};
                    /* Using dynamically changing char * SearchText */
                    CLAY_TEXT(TypedText, CLAY_TEXT_CONFIG(InputTextConfig));       
                };

                CLAY(CLAY_ID("PlayerDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, (intptr_t)WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        CLAY(CLAY_ID("PlayerDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}){
            CLAY(CLAY_ID("PlayerDBHeader"), { HeadLabelWindow, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .backgroundColor = COLOR_RED}) {
            };          
        };
    };
}

void MakeCreatureHeader(int i) {
    CLAY(CLAY_IDI("CreatureHeader", i), {CreatureButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)}) {

        CLAY_AUTO_ID({NameContainerLayoutConfig}){
            CLAY_TEXT(DBPageHeaders[i].CreatureName, CLAY_TEXT_CONFIG(ButtonTextConfig));
        };

        CLAY_AUTO_ID({SizeCRContainerLayoutConfig}) {
            /* change to be just size and type */
            CLAY_AUTO_ID({CRContainerLayoutConfig}){
                CLAY_TEXT(DBPageHeaders[i].CreatureCR, CLAY_TEXT_CONFIG(ButtonTextConfig));
            };
            CLAY_AUTO_ID({SizeContainerLayoutConfig}){
                CLAY_TEXT(DBPageHeaders[i].CreatureSize, CLAY_TEXT_CONFIG(ButtonTextConfig));
            };
        };
        CLAY_AUTO_ID({TypeSrcContainerLayoutConfig}) {
            CLAY_AUTO_ID({TypeContainerLayoutConfig}) {
                CLAY_TEXT(DBPageHeaders[i].CreatureType, CLAY_TEXT_CONFIG(ButtonTextConfig));
            };
            CLAY_AUTO_ID({SourceContainerLayoutConfig}) {
                //CLAY_TEXT(CreatureSourceText, CLAY_TEXT_CONFIG(ButtonTextConfig));
            };
        };
        Clay_OnHover(CallStatBlockCallback, (intptr_t)i);
    };
}

void FillStats(void) {
    CLAY(CLAY_ID("StatPage"), {StatPageContainer, .backgroundColor = (WindowState == CREATURE_DB_SCREEN) ? COLOR_TRANSPARENT : COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX), .clip = {false, true, Clay_GetScrollOffset()}}) {
        // TODO: need a bunch of elements to format the stat page and fill them with a bunch of CLAY_STRINGS

        CLAY(CLAY_ID("NameContainer"), {
            StatPageDivider,
            .backgroundColor = COLOR_TRANSPARENT,
            .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }
        }) {
            CLAY_TEXT(StatName, CLAY_TEXT_CONFIG(StatPageTextConfig));
        };
        CLAY(CLAY_ID("ACHPContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY(CLAY_ID("ACContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Armor Class"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatArmorClass, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY(CLAY_ID("HPContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Hit Points"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatHitpointsAvg, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatHitpointsRoll, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }; /* Start Ability Scores container*/
        CLAY(CLAY_ID("AbilityScoresContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT}) {
            /* Ability score labels*/
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("STR"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("DEX"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("CON"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("INT"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("WIS"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("CHA"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
            } /* Ability score values*/
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatStr, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatDex, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatCon, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatInt, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatWis, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatCha, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                }
            }
        }; /* Write senses  */
        CLAY(CLAY_ID("SensesContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatSavingThrows.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSavingThrows, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(StatSkills, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(StatSenses, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(StatLanguages, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Challenge Rating"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatCr, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }; /* Write racial traits/features */
        CLAY(CLAY_ID("RacialContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(StatSpecialAbilityOne, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatSpecialAbilityOneDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            if (0 != SDL_strcmp("0", StatSpecialAbilityTwo.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSpecialAbilityTwo, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatSpecialAbilityTwoDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatSpecialAbilityThree.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSpecialAbilityThree, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatSpecialAbilityThreeDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        }; /* Write creature actions */
        CLAY(CLAY_ID("ActionsContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(StatAttack1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatAttack1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            if (0 != SDL_strcmp("0", StatAttack2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack4.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack4, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack4Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack5Type.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack5Type, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack5Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LegendaryBonusContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatActionLeg.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLeg, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLeg2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLeg2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLeg3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLeg3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LairReactionContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatActionLair.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLair, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLair2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLair3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("BonusVillainContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            // CLAY_TEXT(CLAY_STRING("Fill stats here"), CLAY_TEXT_CONFIG(StatPageTextConfig));
        };
        CLAY(CLAY_ID("VillainContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            // CLAY_TEXT(CLAY_STRING("Fill stats here"), CLAY_TEXT_CONFIG(StatPageTextConfig));
        };
        CLAY(CLAY_ID("RegionalEffectContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect1.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatEndRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatEndRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
    };
}

/*-------------------------------------------------------------------------------------------*
*                                    Button Callbacks                                        *
*--------------------------------------------------------------------------------------------*/

static void ReturnToMainScreenCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        SDL_memset(TextBuffer, 0, sizeof(TextBuffer));
        ScrollOffset = 0;
        WindowState = MAIN_SCREEN;
        for (int i =0; i < TotalCreatures; i++){
            HeadersToShow[i] = i;
        }
    }
}

static void StartEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = START_ENCOUNTER_SCREEN;
    }
}

static void BuildEncounterButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = BUILD_ENCOUNTER_SCREEN;
    }
}

static void CreatureDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        FirstVisible == 0;
        ScrollOffset = 0;
        WindowState = CREATURE_DB_SCREEN;
    }
}

static void PlayerDatabaseButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int check = (int) userData;
    /* TODO: how should this work? changes only on mouse movement after clicking */
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = PLAYER_DB_SCREEN;
    }
}

static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    int ArrayPosition = (int) userData;
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        LookUpCreatureStats(ArrayPosition);
        WindowState = ADD_STAT_SCREEN;
    }
}

static void SearchButtonCallback(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        char * SearchText = &TextBuffer[0];
        SearchCreatureNames(SearchText);
    }
}