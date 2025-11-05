#include "clay.h"
#include "styles.h"
#include "global.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*========================================================================*
 *  SECTION - Clay_Color definitions
 *========================================================================* 
 */
const Clay_Color COLOR_ORANGE       = (Clay_Color) {225, 138, 50, 255};
const Clay_Color COLOR_BLUE         = (Clay_Color) {111, 173, 162, 255};
const Clay_Color COLOR_LIGHT        = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_BLACK        = (Clay_Color) {0, 0, 0, 255};
const Clay_Color COLOR_RED          = (Clay_Color) {220, 0, 0, 255};
const Clay_Color COLOR_GREEN        = (Clay_Color) {0, 220, 0, 255};
const Clay_Color COLOR_WHITE        = (Clay_Color) { 255, 255, 255, 255};
const Clay_Color COLOR_GRAY_BG      = (Clay_Color) {20, 20, 20, 255};
const Clay_Color COLOR_TRANSPARENT  = (Clay_Color) {0, 0, 0, 0};
const Clay_Color COLOR_BUTTON_GRAY  = (Clay_Color) {70, 70, 70, 255};

/*========================================================================* 
 *  SECTION - Global Functions
 *========================================================================*
 */

Clay_LayoutConfig SidebarTopLayoutConfig =  {
    .sizing = { CLAY_SIZING_FIXED(SIDEBAR_WIDTH_PX), CLAY_SIZING_PERCENT(0.5)},
    .padding = { 8, 8, 8, 8},
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig SidebarBottomLayoutConfig =  {
    .sizing = { CLAY_SIZING_FIXED(SIDEBAR_WIDTH_PX), CLAY_SIZING_PERCENT(0.5)},
    .padding = { 8, 8, 8, 8},
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_BOTTOM },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig CreatureButtonContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_PERCENT(0.3), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig MainScreenLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
    .padding = { MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING},
    .childGap = 40,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig TTBParentWindowLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig LTRParentWindowLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

Clay_LayoutConfig SidebarLayoutConfig = {
    .sizing = { CLAY_SIZING_FIXED(0), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig MainScreenButtonLayoutConfig = {
    .sizing = { CLAY_SIZING_FIXED(MAIN_BTN_WIDTH_PX), CLAY_SIZING_FIXED(MAIN_BTN_HEIGHT_PX) },
    .padding = { 8, 8, 8, 8},
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

/* Container for creature buttons */
Clay_LayoutConfig CreatureButtonLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CREATURE_BTN_HEIGHT_PX) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

/* Container for creature name container and cr container*/
Clay_LayoutConfig NameContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_PERCENT(0.33) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

/* Container for creature name */
Clay_LayoutConfig SizeCRContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0.33) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

/* Container for creature cr */
Clay_LayoutConfig CRContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_PERCENT(0.5), CLAY_SIZING_GROW(0) },
    .padding = { 4, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

/**/
/* Container for creature name container and cr container*/
Clay_LayoutConfig SizeContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_PERCENT(0.5), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

/* Container for creature name */
Clay_LayoutConfig TypeSrcContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_PERCENT(0.33) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

/* Container for creature cr */
Clay_LayoutConfig TypeContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_PERCENT(0.5), CLAY_SIZING_GROW(0) },
    .padding = { 4, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};
/**/
Clay_LayoutConfig SourceContainerLayoutConfig = {
    .sizing = { CLAY_SIZING_PERCENT(0.5), CLAY_SIZING_GROW(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 8,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

Clay_LayoutConfig SingleLineInputLayoutConfig = {
    .sizing = { CLAY_SIZING_FIXED(214), CLAY_SIZING_FIXED(32) },
    .padding = { 8, 8, 8, 8},
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig StatPageContainer = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
    .padding = { MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING, MAIN_WINDOW_PADDING},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_TOP },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

Clay_LayoutConfig StatePageDivider = {
    .sizing = { CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0) },
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_TOP },
    .layoutDirection = CLAY_LEFT_TO_RIGHT
};

Clay_TextElementConfig MainLabelTextConfig = {
    .textColor = COLOR_WHITE,
    .fontId = 0,
    .fontSize = 64,
    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
    .wrapMode = CLAY_TEXT_WRAP_NONE
};

Clay_TextElementConfig ButtonTextConfig = {
    .textColor = COLOR_WHITE,
    .fontId = 0,
    .fontSize = 16,
    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
    .wrapMode = CLAY_TEXT_WRAP_NEWLINES
};

Clay_TextElementConfig InputTextConfig = {
    .textColor = COLOR_WHITE,
    .fontId = 0,
    .fontSize = 16,
    .textAlignment = CLAY_TEXT_ALIGN_LEFT,
    .wrapMode = CLAY_TEXT_WRAP_WORDS
};

/*========================================================================* 
 *  SECTION - Constant Styles
 *========================================================================* 
 */

 // TEMP
const Clay_LayoutConfig HeadLabelWindow = (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_FIXED(500), CLAY_SIZING_FIXED(100) },
    .padding = { 8, 8, 8, 8 },
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};