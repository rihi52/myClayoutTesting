#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "db_query.h"
#include "build_encounter.h"
#include "creature_db.h"
#include "slider.h"
#include "modal.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>

/*========================================================================* 
 *  SECTION - Local prototypes
 *========================================================================* 
 */
static bool IsStringValid(const char *str);
static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void AddStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void SaveNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
static void CancelNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void NewStatblockPage(void);

static char AbilityModName[NUM_ABILITIES][4] = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};

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
                if (WindowState == NEW_STATBLOCK_SCREEN) {
                    CLAY(CLAY_ID("CreatureDBSaveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Save"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                        Clay_OnHover(SaveNewStatBlockCallback, &WindowState);
                    };
                    CLAY(CLAY_ID("CreatureDBCancelButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Cancel"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                        Clay_OnHover(CancelNewStatBlockCallback, &WindowState);
                    };
                }
                else {
                    CLAY(CLAY_ID("CreatureDBAddButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        Clay_OnHover(AddStatBlockCallback, &WindowState);
                        CLAY_TEXT(CLAY_STRING("Add New"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                    CLAY(CLAY_ID("CreatureDBRemoveButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Remove Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                    CLAY(CLAY_ID("CreatureDBEditButton"), {MainScreenButtonLayoutConfig, .backgroundColor = COLOR_BUTTON_GRAY, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
                        CLAY_TEXT(CLAY_STRING("Edit Not Working"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    };
                }
                /* TODO: Maybe slider filtering for CR? */
                // Clay_String BarName = {true, 10, "BarName"};
                // SliderBar(BarName, 150, 10, 10, COLOR_GREEN, COLOR_BLUE);
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
                    CLAY_TEXT(CLAY_STRING("Name Search"), CLAY_TEXT_CONFIG(ButtonTextConfig));
                    Clay_OnHover(SearchButtonCallback, &WindowState);
                };                
            };
        };

        /* Main content containing monster lists and stats*/
        
        CLAY(CLAY_ID("CreatureDBContentWindow"), {LTRParentWindowLayoutConfig, .backgroundColor = COLOR_BLACK, .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX)}) {
            if (WindowState == NEW_STATBLOCK_SCREEN) {
                    NewStatblockPage();
            }
            else {
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
                CLAY(CLAY_ID("HeaderScrollBarContainer"), {
                    .layout = {
                            .sizing = { CLAY_SIZING_PERCENT(.01), CLAY_SIZING_PERCENT(1.0) },
                            .padding = { 0, 0, 0, 0},
                            .childGap = 2,
                            .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        },
                        .backgroundColor = COLOR_TRANSPARENT,
                }){
                    CLAY(CLAY_ID("HeaderScrollBar"), {
                        .layout = {
                            .sizing = { CLAY_SIZING_PERCENT(1), CLAY_SIZING_FIXED(10) },
                            .padding = { 0, 0, 0, 0},
                            .childGap = 2,
                            .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        },
                        .backgroundColor = COLOR_BUTTON_GRAY,
                        .floating = {
                            .parentId = CLAY_ID("CreatureHeaderContainer").id,
                        },
                    }) {

                    }
                }

                if (WindowState == SHOW_STAT_BLOCK) {
                    FillStats();                
                }            
            }
        };
        if (gAppState->IsModalOpen) {
            Clay_String ModalText = { true, SDL_strlen(gAppState->ModalMessage), gAppState->ModalMessage };

            CreateBottomModal(gAppState->ModalParentId, ModalText);
            ModalTimer -= deltaTime;
            if (ModalTimer <= 0.0) {
                gAppState->IsModalOpen = false;
                gAppState->ModalParentId = CLAY_ID("NULL");
                memset(gAppState->ModalMessage, 0, sizeof(gAppState->ModalMessage));
                memset(gAppState->EncounterSaved, 0, sizeof(gAppState->EncounterSaved));
                ModalTimer = MODAL_TIMER;
            }
        }
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

void FillStats(void) {
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
            CLAY_TEXT(gAppState->CurrentStatBlock.StatName, CLAY_TEXT_CONFIG(StatPageTextConfig));
        };
        CLAY(CLAY_ID("ACHPSpeedContainer"), {
            StatPageDivider, 
            .backgroundColor = COLOR_TRANSPARENT, 
            .border = { 
                .width = { 
                    .bottom = 5
                }, 
                .color = COLOR_BLACK 
            }
        }) {
            CLAY(CLAY_ID("ACContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Armor Class"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(gAppState->CurrentStatBlock.StatArmorClass, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY(CLAY_ID("HPContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Hit Points"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(gAppState->CurrentStatBlock.StatHitpointsAvg, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(gAppState->CurrentStatBlock.StatHitpointsRoll, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            CLAY(CLAY_ID("SpeedContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Speed"), CLAY_TEXT_CONFIG(StatPageTextConfig));               
            }
            CLAY(CLAY_ID("SpeedTypeContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpeedType, CLAY_TEXT_CONFIG(StatPageTextConfig));
                } 
            CLAY(CLAY_ID("SpeedDistanceContainer"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}){
                for(int i = 0; i < SPEED_TYPES; i++){
                    if(IsStringValid(gAppState->CurrentStatBlock.SpeedValues[i].chars)){
                        CLAY_TEXT(gAppState->CurrentStatBlock.SpeedValues[i], CLAY_TEXT_CONFIG(StatPageTextConfig));
                    }
                }
            }
        }; /* Start Ability Scores container*/
        CLAY(CLAY_ID("AbilityScoresContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT}) {
            /* Ability score labels*/
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                // for(int i = 0; i < NUM_ABILITIES; i++){
                //     CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //         Clay_String abilityStr = {
                //             .chars = AbilityModName[i],
                //             .length = SDL_strlen(AbilityModName[i])
                //         };
                //         CLAY_TEXT(abilityStr, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                //     }
                // };
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
                for(int i = 0; i < NUM_ABILITIES; i++){
                    CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(gAppState->CurrentStatBlock.StatValues[i] CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                    }
                };
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatStr, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatDex, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatCon, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatInt, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatWis, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
                // CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                //     CLAY_TEXT(gAppState->CurrentStatBlock.StatCha, CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
                // }
            }
        }; /* Write senses  */
        CLAY(CLAY_ID("SensesContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            // 0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatSavingThrows.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatSavingThrows.chars)
            if (IsStringValid(gAppState->CurrentStatBlock.StatSavingThrows.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSavingThrows, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatSkills.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSkills, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatSenses.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSenses, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatLanguages.chars)){
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatLanguages, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Challenge Rating"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(gAppState->CurrentStatBlock.StatCr, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
        }; /* Write racial traits/features */
        CLAY(CLAY_ID("RacialContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (IsStringValid(gAppState->CurrentStatBlock.StatSpecialAbilityOne.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityOne, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityOneDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatSpecialAbilityTwo.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityTwo, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityTwoDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatSpecialAbilityThree.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityThree, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatSpecialAbilityThreeDesc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        }; /* Write creature actions */
        CLAY(CLAY_ID("ActionsContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatAttack2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatAttack3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatAttack4.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack4, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack4Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatAttack5.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack5, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack5Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatAttack6.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack6, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatAttack6Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LegendaryBonusContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLeg.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("The creature can take 3 legendary actions, choosing from the options below. Only one legendary action can be used at a time and only at the end of another creature's turn. The creature regains spent legendary actions at the start of its turn."), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLeg2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLeg3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLeg3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
        CLAY(CLAY_ID("LairReactionContainer"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLair.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLair2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatActionLair3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
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
            if (IsStringValid(gAppState->CurrentStatBlock.StatRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatRegionalEffect1.chars)chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatRegionalEffect1, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatRegionalEffect2.chars)chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatRegionalEffect2, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatRegionalEffect3.chars)chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatRegionalEffect3, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
            if (IsStringValid(gAppState->CurrentStatBlock.StatEndRegionalEffect.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(gAppState->CurrentStatBlock.StatEndRegionalEffect, CLAY_TEXT_CONFIG(StatPageTextConfig));
                }
            }
        };
    };
}

void NewStatblockPage(void) {
    CLAY(CLAY_ID("StatPageFill"), {
        StatPageContainer,
        .backgroundColor = (WindowState == CREATURE_DB_SCREEN) ? COLOR_TRANSPARENT : COLOR_BUTTON_GRAY,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_LG_PX),
        .clip = {false, true, Clay_GetScrollOffset()}
    }) {

        CLAY(CLAY_ID("NameContainerFill"), {
            StatPageDivider,
            .backgroundColor = COLOR_TRANSPARENT,
            .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }
        }) {
            CLAY_TEXT(CLAY_STRING("Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
            CLAY(CLAY_ID("NameTextBox"), {
                SingleLineInputLayoutConfig,
                .backgroundColor = (gAppState->focusedId.id == CLAY_ID("NameTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
            }) {
                Clay_OnHover(FocusWindowCallback, gAppState);
                uint32_t CurrentFocus = gAppState->focusedId.id;
                FocusAndWriteTextBox(CLAY_ID("NameTextBox"), CurrentFocus, &StatNameTextBox); 
            }        
        };
        CLAY(CLAY_ID("ACHPContainerFill"), {
            StatPageDivider,
            .backgroundColor = COLOR_TRANSPARENT,
            .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }
        }) {
            CLAY(CLAY_ID("ACContainerFill"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Armor Class"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("ACTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("ACTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("ACTextBox"), CurrentFocus, &StatArmorClassTextBox); 
                } 
            }
            CLAY(CLAY_ID("HPContainerFill"), {StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Hit Points (Not Dice)"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("HPTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("HPTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("HPTextBox"), CurrentFocus, &StatHitpointsAvgTextBox); 
                } 
            }
        }; /* Start Ability Scores container*/
        CLAY(CLAY_ID("AbilityScoresContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT}) {
            CLAY_TEXT(CLAY_STRING("Ability Scores (not modifiers)"), CLAY_TEXT_CONFIG(StatPageAbilityScoreTextConfig));
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
                    CLAY(CLAY_ID("STRTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("STRTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("STRTextBox"), CurrentFocus, &StatStrTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY(CLAY_ID("DEXTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("DEXTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("DEXTextBox"), CurrentFocus, &StatDexTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY(CLAY_ID("CONTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("CONTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("CONTextBox"), CurrentFocus, &StatConTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY(CLAY_ID("INTTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("INTTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("INTTextBox"), CurrentFocus, &StatIntTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY(CLAY_ID("WISTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("WISTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("WISTextBox"), CurrentFocus, &StatWisTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageAbilityDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY(CLAY_ID("CHATextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("CHATextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("CHATextBox"), CurrentFocus, &StatChaTextBox);
                    }
                }
            }
        }; /* Write senses  */
        CLAY(CLAY_ID("SensesContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Proficiency Bonus (unsigned, eg: 2 instead of +2)"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("ProfBonusTextBox"), {
                        ShortSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("ProfBonusTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("ProfBonusTextBox"), CurrentFocus, &StatProfBonusTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Saving Throws (comma separated list of ability score abbreviations)"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SavingThrowTextBox"), {
                        LongSingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SavingThrowTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SavingThrowTextBox"), CurrentFocus, &StatSavingThrowsTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Skills"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SkillsTextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SkillsTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SkillsTextBox"), CurrentFocus, &StatSkillsTextBox); 
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Senses"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SensesTextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SensesTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SensesTextBox"), CurrentFocus, &StatSensesTextBox); 
                    }
                }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Languages"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("LanguagesTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("LanguagesTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("LanguagesTextBox"), CurrentFocus, &StatLanguagesTextBox);
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Challenge Rating"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("CRRatingTextBox"), {
                    SingleLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("CRRatingTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("CRRatingTextBox"), CurrentFocus, &StatCrTextBox);
                }
            }
        }; /* Write racial traits/features */
        CLAY(CLAY_ID("RacialContainerFill"), {
            StatPageDivider,
            .backgroundColor = COLOR_TRANSPARENT,
            .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }
        }) {
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Special Ability One Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SpecialAbility1NameTextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbility1NameTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SpecialAbility1NameTextBox"), CurrentFocus, &StatSpecialAbilityOneTextBox);
                    }
                }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Special Ability One"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("SpecialAbilityOneTextBox"), {
                    MultiLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbilityOneTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .clip = {false, true, Clay_GetScrollOffset()}
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("SpecialAbilityOneTextBox"), CurrentFocus, &StatSpecialAbilityOneDescTextBox);
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Special Ability Two Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SpecialAbility2NameTextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbility2NameTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SpecialAbility2NameTextBox"), CurrentFocus, &StatSpecialAbilityTwoTextBox);
                    }
                }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Special Ability Two"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("SpecialAbilityTwoTextBox"), {
                    MultiLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbilityTwoTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .clip = {false, true, Clay_GetScrollOffset()}
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("SpecialAbilityTwoTextBox"), CurrentFocus, &StatSpecialAbilityTwoDescTextBox);
                }
            }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Special Ability Three Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("SpecialAbility3NameTextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbility3NameTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("SpecialAbility3NameTextBox"), CurrentFocus, &StatSpecialAbilityThreeTextBox);
                    }
                }
            CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_TEXT(CLAY_STRING("Special Ability Three"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                CLAY(CLAY_ID("SpecialAbilityThreeTextBox"), {
                    MultiLineInputLayoutConfig,
                    .backgroundColor = (gAppState->focusedId.id == CLAY_ID("SpecialAbilityThreeTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                    .clip = {false, true, Clay_GetScrollOffset()}
                }) {
                    Clay_OnHover(FocusWindowCallback, gAppState);
                    uint32_t CurrentFocus = gAppState->focusedId.id;
                    FocusAndWriteTextBox(CLAY_ID("SpecialAbilityThreeTextBox"), CurrentFocus, &StatSpecialAbilityThreeDescTextBox);
                }
            }
        }; /* Write creature actions */
        CLAY(CLAY_ID("ActionsContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack One Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack1TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack1TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack1TextBox"), CurrentFocus, &StatAttack1TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack One Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack1DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack1DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack1DescTextBox"), CurrentFocus, &StatAttack1DescTextBox);
                    }
                }
            }
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Two Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack2TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack2TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack2TextBox"), CurrentFocus, &StatAttack2TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Two Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack2DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack2DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack2DescTextBox"), CurrentFocus, &StatAttack2DescTextBox);
                    }
                }
            }
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Three Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack3TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack3TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack3TextBox"), CurrentFocus, &StatAttack3TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Three Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack3DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack3DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack3DescTextBox"), CurrentFocus, &StatAttack3DescTextBox);
                    }
                }
            }
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Four Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack4TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack4TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack4TextBox"), CurrentFocus, &StatAttack4TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Four Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack4DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack4DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack4DescTextBox"), CurrentFocus, &StatAttack4DescTextBox);
                    }
                }
            }
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Five Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack5TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack5TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack5TextBox"), CurrentFocus, &StatAttack5TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Five Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack5DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack5DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack5DescTextBox"), CurrentFocus, &StatAttack5DescTextBox);
                    }
                }
            }
            CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Six Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack6TextBox"), {
                        SingleLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack6TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack6TextBox"), CurrentFocus, &StatAttack6TextBox);
                    }
                }
                CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("Attack Six Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY(CLAY_ID("StatAttack6DescTextBox"), {
                        MultiLineInputLayoutConfig,
                        .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatAttack6DescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                        .clip = {false, true, Clay_GetScrollOffset()}
                    }) {
                        Clay_OnHover(FocusWindowCallback, gAppState);
                        uint32_t CurrentFocus = gAppState->focusedId.id;
                        FocusAndWriteTextBox(CLAY_ID("StatAttack6DescTextBox"), CurrentFocus, &StatAttack6DescTextBox);
                    }
                }
            }
        };
        CLAY(CLAY_ID("LegendaryBonusContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
            if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLeg.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLeg.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_TEXT(CLAY_STRING("(Optional) The creature can take 3 legendary actions, choosing from the options below. Only one legendary action can be used at a time and only at the end of another creature's turn. The creature regains spent legendary actions at the start of its turn."), CLAY_TEXT_CONFIG(StatPageTextConfig));
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action One Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLegTextBox"), {
                            SingleLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLegTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLegTextBox"), CurrentFocus, &StatActionLegTextBox);
                        }
                    }
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action One Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLegDescTextBox"), {
                            MultiLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLegDescTextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLegDescTextBox"), CurrentFocus, &StatActionLeg1DescTextBox);
                        }
                    }
                }
            }
            if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLeg2.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLeg2.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action Two Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLeg2TextBox"), {
                            SingleLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLeg2TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLeg2TextBox"), CurrentFocus, &StatActionLeg2TextBox);
                        }
                    }
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action Two Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLegDesc2TextBox"), {
                            MultiLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLegDesc2TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLegDesc2TextBox"), CurrentFocus, &StatActionLeg2DescTextBox);
                        }
                    }
                }
            }
            if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLeg3.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLeg3.chars)){
                CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action Three Name"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLeg3TextBox"), {
                            SingleLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLeg3TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLeg3TextBox"), CurrentFocus, &StatActionLeg3TextBox);
                        }
                    }
                    CLAY_AUTO_ID({StatPageSubDivider, .backgroundColor = COLOR_TRANSPARENT}) {
                        CLAY_TEXT(CLAY_STRING("Legendary Action Three Description"), CLAY_TEXT_CONFIG(StatPageTextConfig));
                        CLAY(CLAY_ID("StatActionLegDesc3TextBox"), {
                            MultiLineInputLayoutConfig,
                            .backgroundColor = (gAppState->focusedId.id == CLAY_ID("StatActionLegDesc3TextBox").id) ? COLOR_BLACK : COLOR_GRAY_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
                            .clip = {false, true, Clay_GetScrollOffset()}
                        }) {
                            Clay_OnHover(FocusWindowCallback, gAppState);
                            uint32_t CurrentFocus = gAppState->focusedId.id;
                            FocusAndWriteTextBox(CLAY_ID("StatActionLegDesc3TextBox"), CurrentFocus, &StatActionLeg3DescTextBox);
                        }
                    }
                }
            }
        };
        /* TODO: FILL IN THE BELOW FOR TEXT BOX OPTIONS WHEN CREATING NEW CREATURE. I WOULD LIKE TO FIND A LESS REPETITIVE WAY OF DOING IT FIRST */

        // CLAY(CLAY_ID("LairReactionContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
        //     if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLair.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLair.chars)){
        //         CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair1, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair1Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //         }
        //     }
        //     if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLair2.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLair2.chars)){
        //         CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair2Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //         }
        //     }
        //     if (0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLair3.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLair3.chars)){
        //         CLAY_AUTO_ID({StatPageActionDivider, .backgroundColor = COLOR_TRANSPARENT}) {
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //             CLAY_TEXT(gAppState->CurrentStatBlock.StatActionLair3Desc, CLAY_TEXT_CONFIG(StatPageTextConfig));
        //         }
        //     }
        // };
        // CLAY(CLAY_ID("BonusVillainContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
        //     // CLAY_TEXT(CLAY_STRING("Fill stats here"), CLAY_TEXT_CONFIG(StatPageTextConfig));
        // };
        // CLAY(CLAY_ID("VillainContainerFill"), {StatPageDivider, .backgroundColor = COLOR_TRANSPARENT, .border = { .width = { .bottom = 5 }, .color = COLOR_BLACK }}) {
        //     // CLAY_TEXT(CLAY_STRING("Fill stats here"), CLAY_TEXT_CONFIG(StatPageTextConfig));
        // };
    };
}

/*========================================================================* 
 *  SECTION - Local functions
 *========================================================================* 
 0 != SDL_strcmp("0", gAppState->CurrentStatBlock.StatActionLeg.chars) && 0 != SDL_strcmp("NULL", gAppState->CurrentStatBlock.StatActionLeg.chars) */

static bool IsStringValid(const char *str) {
    if (str == NULL || str[0] == '\0' || str == "") {
        return false;
    }
    return SDL_strcmp("0", str) != 0 && SDL_strcmp("NULL", str) != 0;
}

static void CallStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->focusedId = elementId;
        gAppState->IsTextInputFocused = false;
        int * ArrayPosition = (int *) userData;
        LookUpCreatureStats(*ArrayPosition);
        WindowState = SHOW_STAT_BLOCK;
    }
}

static void AddStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        WindowState = NEW_STATBLOCK_SCREEN;
    }
}

static void SaveNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        SaveNewStatBlockToDB();
        /* Saving encounter name before reset to display in modal */
        SDL_strlcpy(gAppState->CreatureSaved, StatNameTextBox.StringToDisplay.chars, sizeof(gAppState->CreatureSaved));

        RefreshDatabaseMonsters();

        WindowState = CREATURE_DB_SCREEN;
        gAppState->ActiveScreen = CREATURE_DB_SCREEN;
        ClearFocus();
        ClearTextBoxes();
        ResetVisibleCreatureHeaders();

        /* Building modal message */
        gAppState->IsModalOpen = true;
        gAppState->ModalParentId = CLAY_ID("CreatureDBOuterContainer");
        char temp[256];
        SDL_snprintf(gAppState->ModalMessage, sizeof(gAppState->ModalMessage), "%s saved successfully.", gAppState->CreatureSaved);
    }
}

static void CancelNewStatBlockCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        gAppState->ActiveScreen = CREATURE_DB_SCREEN;
        gAppState->focusedId = CLAY_ID("NULL");
        gAppState->IsTextInputFocused = false;
        ClearTextBoxes();
        WindowState = CREATURE_DB_SCREEN;
    }
}