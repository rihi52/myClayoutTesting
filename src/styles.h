#ifndef STYLES_H
#define STYLES_H

#include "clay.h"
#include <stdio.h>

#define GLOBAL_RADIUS_LG_PX     12
#define GLOBAL_RADIUS_SM_PX     8
#define MAIN_WINDOW_PADDING     8
#define INPUT_BORDER_WIDTH_PX   2
#define SIDEBAR_WIDTH_PX        300
#define DB_LIST_WIDTH_PX        400
#define DB_LIST_HEIGHT_PX       100
#define MAIN_BTN_WIDTH_PX       200
#define MAIN_BTN_HEIGHT_PX      50
#define CREATURE_BTN_WIDTH_PX   300
#define CREATURE_BTN_HEIGHT_PX  80

extern Clay_LayoutConfig SidebarTopLayoutConfig;
extern Clay_LayoutConfig SidebarBottomLayoutConfig;
extern Clay_LayoutConfig MainScreenLayoutConfig;
extern Clay_LayoutConfig LTRParentWindowLayoutConfig;
extern Clay_LayoutConfig SidebarLayoutConfig;
extern Clay_LayoutConfig MainScreenButtonLayoutConfig;
extern Clay_LayoutConfig CreatureButtonLayoutConfig;
extern Clay_LayoutConfig CreatureButtonContainerLayoutConfig;
extern Clay_TextElementConfig ButtonTextConfig;
extern Clay_TextElementConfig InputTextConfig;
extern Clay_TextElementConfig MainLabelTextConfig;
extern Clay_LayoutConfig SingleLineInputLayoutConfig;
extern Clay_LayoutConfig TTBParentWindowLayoutConfig;

extern Clay_LayoutConfig StatPageContainer;
extern Clay_LayoutConfig StatPageDivider;
extern Clay_LayoutConfig StatPageSubDivider;
extern Clay_LayoutConfig StatPageAbilityDivider;
extern Clay_TextElementConfig StatPageTextConfig;
extern Clay_TextElementConfig StatPageAbilityScoreTextConfig;

/* Container for creature buttons */
extern Clay_LayoutConfig CreatureButtonLayoutConfig;

/* Container for creature name container and cr container*/
extern Clay_LayoutConfig NameContainerLayoutConfig;

/* Container for creature name */
extern Clay_LayoutConfig SizeCRContainerLayoutConfig;

/* Container for creature cr */
extern Clay_LayoutConfig CRContainerLayoutConfig;

/* Container for creature name */
extern Clay_LayoutConfig SizeContainerLayoutConfig;

/**/
/* Container for creature name container and cr container*/
extern Clay_LayoutConfig TypeSrcContainerLayoutConfig;

/* Container for creature cr */
extern Clay_LayoutConfig TypeContainerLayoutConfig;
/**/
extern Clay_LayoutConfig SourceContainerLayoutConfig;

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
extern const Clay_Color COLOR_TRANSPARENT;

/* Styles */

// TEMP
extern const Clay_LayoutConfig HeadLabelWindow;

#endif /* STYLES_H */