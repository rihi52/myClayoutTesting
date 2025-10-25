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

/* This function returns a Clay_ElementDeclaration struct used for styling a Sidebar
 * It assumes CLAY_SIZING_FIXED(width), CLAY_SIZING_GROW(0, height), and CLAY_ALING_X_CENTER */
Clay_LayoutConfig MakeFixedWidthLimitHeightWindow(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection) {

    return (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_FIXED(width), CLAY_SIZING_GROW(0, (height)) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, yChildAlign },
    .layoutDirection = layoutDirection
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Sidebar
 * It assumes CLAY_SIZING_FIXED(width), CLAY_SIZING_GROW(0, height), and CLAY_ALING_X_CENTER */
Clay_ElementDeclaration MakeFixedWidthGrowHeightWindow(uint16_t width, uint16_t padding, uint16_t childGap, uint16_t xChildAlign, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR, bool isScroll, Clay_Vector2 scrollOffset) {

    Clay_LayoutConfig WindowConfig = {
    .sizing = { CLAY_SIZING_FIXED(width), CLAY_SIZING_GROW(0) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { xChildAlign, yChildAlign },
    .layoutDirection = layoutDirection
    };

    Clay_ClipElementConfig ClipConfig = {
        .horizontal = false,
        .vertical = isScroll,
        .childOffset = scrollOffset
    };    

    return (Clay_ElementDeclaration) {
        .layout = WindowConfig,
        .backgroundColor = COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(10),
        .clip = ClipConfig
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Statblock Header
 * It assumes CLAY_SIZING_FIXED(0) for width, CLAY_SIZING_FIXED(height), CLAY_ALIGN_X_CENTER, and CLAY_ALIGN_Y_CENTER */
Clay_ElementDeclaration MakeStatblockHeaderStyle( uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR) {

    Clay_LayoutConfig StatblockHeaderConfig = {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(height) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, yChildAlign },
    .layoutDirection = layoutDirection
    };

    return (Clay_ElementDeclaration) {
        .layout = StatblockHeaderConfig,
        .backgroundColor = COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(10)
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Parent Window
 * It assumes CLAY_SIZING_GROW(0) for width, and CLAY_SIZING_GROW(0) for height */
Clay_LayoutConfig MakeParentWindowStyle(uint16_t padding, uint16_t childGap, uint16_t xChildAlign, uint16_t yChildAlign, uint16_t layoutDirection) {

    return (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { xChildAlign, yChildAlign },
    .layoutDirection = layoutDirection
    };

    // return (Clay_ElementDeclaration) {
    //     .layout = ParentWindowConfig,
    //     .backgroundColor = COLOR,
    //     .cornerRadius = CLAY_CORNER_RADIUS(10)
    // };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Sidebar
 * It assumes CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0), CLAY_ALING_X_CENTER, and CLAY_ALIGN_Y_CENTER */
Clay_ElementDeclaration MakeSidebarStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR) {

    Clay_LayoutConfig ContentWindowConfig = {
    .sizing = { CLAY_SIZING_FIXED(width), CLAY_SIZING_GROW(0, height) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = layoutDirection
    };

    return (Clay_ElementDeclaration) {
        .layout = ContentWindowConfig,
        .backgroundColor = COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(10)
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Button
 * It assumes CLAY_SIZING_FIXED(width), CLAY_SIZING_FIXED(height), CLAY_ALING_X_CENTER, CLAY_ALIGN_Y_CENTER, and CLAY_TOP_TO_BOTTOM */
Clay_ElementDeclaration FixedContainerTTBStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, Clay_Color COLOR) {

    Clay_LayoutConfig ContentWindowConfig = {
    .sizing = { CLAY_SIZING_FIXED(width), CLAY_SIZING_FIXED(height) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
    };

    return (Clay_ElementDeclaration) {
        .layout = ContentWindowConfig,
        .backgroundColor = COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(10)
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a Sidebar
 * It assumes CLAY_SIZING_FIXED(width), CLAY_SIZING_FIXED(height), CLAY_ALING_X_CENTER, CLAY_ALIGN_Y_CENTER, and CLAY_TOP_TO_BOTTOM */
Clay_TextElementConfig TextConfig(Clay_Color COLOR, uint16_t fontId, uint16_t fontSize, uint16_t textAlignment, uint16_t wrapMode) {

    return (Clay_TextElementConfig) {
        .textColor = COLOR_WHITE,
        .fontId = fontId,
        .fontSize = fontSize,
        .textAlignment = textAlignment,
        .wrapMode = wrapMode
    };
}

/* This function returns a Clay_ElementDeclaration struct used for styling a single line text container
 * It assumes CLAY_SIZING_FIXED(width), CLAY_SIZING_FIXED(height), CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER, and CLAY_TOP_TO_BOTTOM */
Clay_ElementDeclaration SingleLineTextContainerStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, Clay_Color COLOR, uint16_t cornerRadius, uint16_t borderWidthAll, Clay_Color BORDER_COLOR) {

    Clay_LayoutConfig SingleLineTextContainerLayout = {
    .sizing = { CLAY_SIZING_FIXED(width), CLAY_SIZING_FIXED(height) },
    .padding = { padding, padding, padding, padding},
    .childGap = childGap,
    .childAlignment =  { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
    };

    return (Clay_ElementDeclaration) {
        .layout = SingleLineTextContainerLayout,
        .backgroundColor = COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(cornerRadius),
        .border = {
            .width = {
                .left   = borderWidthAll,
                .right  = borderWidthAll,
                .top    = borderWidthAll,
                .bottom = borderWidthAll
            },
            .color = BORDER_COLOR
        }
    };
}

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