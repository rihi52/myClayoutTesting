#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "text_input.h"
#include "db_query.h"
#include "build_encounter.h"
#include "creature_db.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
static void FillStats(void);
static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

/*========================================================================* 
 *  SECTION - Global functions
 *========================================================================* 
 */
void CreatureDatabaseWindow(AppState * state) {
    /* Creature database window*/
    
    CLAY(CLAY_ID("CreatureDBOuterContainer"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {

        /* Sidebar for option buttons */
        CLAY(CLAY_ID("CreatureDBSidebar"), {SidebarLayoutConfig, .backgroundColor = COLOR_GRAY_BG, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            
            CLAY(CLAY_ID("SidebarTop"), {SidebarTopLayoutConfig, .backgroundColor = COLOR_TRANSPARENT, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX) }) {

                CLAY(CLAY_ID("CreatureDBHomeButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    Clay_OnHover(ReturnToMainScreenCallback, &WindowState);
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
                    .backgroundColor = (state->focusedId.id == CLAY_ID("CreatureTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .border = {
                                .width = CLAY_BORDER_ALL(INPUT_BORDER_WIDTH_PX),
                                .color = COLOR_WHITE
                            }
                }){
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("CreatureTextBox"), CurrentFocus, &DBCreatureSearch);       
                };

                CLAY(CLAY_ID("CreatureDBSearchButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                    // Clay_OnHover(ReturnToMainScreenCallback, &WindowState); TODO: Fill this in with a sql search function
                    CLAY_TEXT(CLAY_STRING("Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, &WindowState);
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
                        MakeCreatureHeader(i, CREATURE_DB_SCREEN);
                    }                    
                }
            }
            if (WindowState == ADD_STAT_SCREEN) {
                FillStats();
                
            }
        };
    };
}

void MakeCreatureHeader(int i, int CallingWindow) {
    CLAY(CLAY_IDI("CreatureHeader", i), {
        CreatureButtonLayoutConfig,
        .backgroundColor = (gAppState->focusedId.id == CLAY_IDI("CreatureHeader", i).id) ? COLOR_GRAY_SELECT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX)
    }) {

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
        if (CREATURE_DB_SCREEN == CallingWindow) {
            Clay_OnHover(CallStatBlockCallback, &HeadersToShow[i]);
        }
        else if (BUILD_ENCOUNTER_SCREEN == CallingWindow) {
            Clay_OnHover(CreatureBuildListCallback, &DBPageHeaders[i].CreatureName);
        }
    };
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 */
static void FillStats(void) {
    CLAY(CLAY_ID("StatPage"), {
        StatPageContainer,
        .backgroundColor = (WindowState == CREATURE_DB_SCREEN) ? COLOR_TRANSPARENT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
        .clip = {false, true, Clay_GetScrollOffset()}}) {

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
            if (0 != SDL_strcmp("0", StatSavingThrows.chars) && 0 != SDL_strcmp("NULL", StatSavingThrows.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSavingThrows, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatSkills.chars) && 0 != SDL_strcmp("NULL", StatSkills.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSkills, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatSenses.chars) && 0 != SDL_strcmp("NULL", StatSenses.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSenses, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatLanguages.chars) && 0 != SDL_strcmp("NULL", StatLanguages.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatLanguages, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Challenge Rating"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(StatCr, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }; /* Write racial traits/features */
        CLAY(CLAY_ID("RacialContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatSpecialAbilityOne.chars) && 0 != SDL_strcmp("NULL", StatSpecialAbilityOne.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSpecialAbilityOne, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatSpecialAbilityOneDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatSpecialAbilityTwo.chars) && 0 != SDL_strcmp("NULL", StatSpecialAbilityTwo.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatSpecialAbilityTwo, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatSpecialAbilityTwoDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatSpecialAbilityThree.chars) && 0 != SDL_strcmp("NULL", StatSpecialAbilityThree.chars)){
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
            if (0 != SDL_strcmp("0", StatAttack2.chars) && 0 != SDL_strcmp("NULL", StatAttack2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack3.chars) && 0 != SDL_strcmp("NULL", StatAttack3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack4.chars) && 0 != SDL_strcmp("NULL", StatAttack4.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack4, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack4Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack5.chars) && 0 != SDL_strcmp("NULL", StatAttack5.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack5, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack5Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatAttack6.chars) && 0 != SDL_strcmp("NULL", StatAttack6.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatAttack5, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatAttack5Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LegendaryBonusContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatActionLeg.chars) && 0 != SDL_strcmp("NULL", StatActionLeg.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("The creature can take 3 legendary actions, choosing from the options below. Only one legendary action can be used at a time and only at the end of another creature's turn. The creature regains spent legendary actions at the start of its turn."), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLeg2.chars) && 0 != SDL_strcmp("NULL", StatActionLeg2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLeg2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLeg3.chars) && 0 != SDL_strcmp("NULL", StatActionLeg3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLeg3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLeg3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LairReactionContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", StatActionLair.chars) && 0 != SDL_strcmp("NULL", StatActionLair.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLair, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLair2.chars) && 0 != SDL_strcmp("NULL", StatActionLair2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(StatActionLair2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatActionLair3.chars) && 0 != SDL_strcmp("NULL", StatActionLair3.chars)){
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
            if (0 != SDL_strcmp("0", StatRegionalEffect.chars) && 0 != SDL_strcmp("NULL", StatRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect1.chars) && 0 != SDL_strcmp("NULL", StatRegionalEffect1.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect2.chars) && 0 != SDL_strcmp("NULL", StatRegionalEffect2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatRegionalEffect3.chars) && 0 != SDL_strcmp("NULL", StatRegionalEffect3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatRegionalEffect3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (0 != SDL_strcmp("0", StatEndRegionalEffect.chars) && 0 != SDL_strcmp("NULL", StatEndRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(StatEndRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
    };
}

static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = elementId;
        int * ArrayPosition = (int *) userData;
        LookUpCreatureStats(*ArrayPosition);
        WindowState = ADD_STAT_SCREEN;
    }
}