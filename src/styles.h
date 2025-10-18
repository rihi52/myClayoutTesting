#ifndef STYLES_H
#define STYLES_H

#include "clay.h"
#include <stdio.h>

#define SIDEBAR_WIDTH_PX    300
#define DB_LIST_WIDTH_PX    400

Clay_ElementDeclaration MakeFixedWidthLimitHeightWindow(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection);
Clay_ElementDeclaration MakeParentWindowStyle(uint16_t padding, uint16_t childGap, uint16_t xChildAlign, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR);
Clay_ElementDeclaration MakeSidebarStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR);
Clay_ElementDeclaration FixedContainerTTBStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, Clay_Color COLOR);
Clay_TextElementConfig  TextConfig(Clay_Color COLOR, uint16_t fontId, uint16_t fontSize, uint16_t textAlignment, uint16_t wrapMode);
Clay_ElementDeclaration SingleLineTextContainerStyle(uint16_t width, uint16_t height, uint16_t padding, uint16_t childGap, Clay_Color COLOR, uint16_t cornerRadius, uint16_t borderWidthAll, Clay_Color BORDER_COLOR);
Clay_ElementDeclaration MakeStatblockHeaderStyle( uint16_t height, uint16_t padding, uint16_t childGap, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR);
Clay_ElementDeclaration MakeFixedWidthGrowHeightWindow(uint16_t width, uint16_t padding, uint16_t childGap, uint16_t xChildAlign, uint16_t yChildAlign, uint16_t layoutDirection, Clay_Color COLOR);

/* Colors */
extern const Clay_Color COLOR_ORANGE;
extern const Clay_Color COLOR_BLUE;
extern const Clay_Color COLOR_LIGHT;
extern const Clay_Color COLOR_BLACK;
extern const Clay_Color COLOR_RED;
extern const Clay_Color COLOR_GREEN;
extern const Clay_Color COLOR_WHITE;
extern const Clay_Color COLOR_GRAY_BG;
extern const Clay_Color COLOR_BUTTON_GRAY;

/* Styles */

// TEMP
extern const Clay_LayoutConfig HeadLabelWindow;

#endif /* STYLES_H */