#include "../clay.h"
#include <stdio.h>

const Clay_LayoutConfig ParentWindow = (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
    .padding = { 16, 16, 16, 16},
    .childGap = 40,
    .childAlignment =  {  CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

const Clay_LayoutConfig HeadLabelWindow = (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_FIXED(400), CLAY_SIZING_FIXED(100) },
    .padding = { 8, 8, 8, 8 },
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

const Clay_LayoutConfig MainButton = (Clay_LayoutConfig) {
    .sizing = { CLAY_SIZING_FIXED(200), CLAY_SIZING_FIXED(50) },
    .padding = { 8, 8, 8, 8 },
    .childGap = 16,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

const Clay_TextElementConfig WindowLabel = (Clay_TextElementConfig) {
    .textColor = { 0, 0, 0, 255},
    .fontId = 0,
    .fontSize = 64,
    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
    .wrapMode = CLAY_TEXT_WRAP_NONE
};

const Clay_TextElementConfig ButtonLabel = (Clay_TextElementConfig) {
    .textColor = { 0, 0, 0, 255},
    .fontId = 0,
    .fontSize = 16,
    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
    .wrapMode = CLAY_TEXT_WRAP_NONE
};

const Clay_ElementDeclaration ButtonStyle = (Clay_ElementDeclaration) {
    MainButton,
    .backgroundColor = {191, 191, 191, 255},
    .cornerRadius = CLAY_CORNER_RADIUS(10)
};