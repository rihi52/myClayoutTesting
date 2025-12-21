#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"

// Rename/redo everything, just experimenting
void SliderKnob (int KnobSize, Clay_Color KnobColor, Clay_String KnobName) {
    CLAY(CLAY_SID(KnobName), {
        .layout = {
            .sizing = {CLAY_SIZING_FIXED(KnobSize), CLAY_SIZING_FIXED(KnobSize)},
            .padding = CLAY_PADDING_ALL(0),
            .childGap = 0,
            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .cornerRadius = CLAY_CORNER_RADIUS(KnobSize),
        .backgroundColor = KnobColor,
        .floating = {
            .offset = {0, 0},
            .attachPoints = CLAY_ATTACH_POINT_LEFT_CENTER,
            .attachTo = CLAY_ATTACH_TO_PARENT
        }
    }) {

    }
}

void SliderBar (Clay_String BarName, int BarWidth, int BarHeight, int BarRadius, Clay_Color PlainColor, Clay_Color HighlightedColor) {
    CLAY(CLAY_SID(BarName), {
        .layout = {
            .sizing = {CLAY_SIZING_FIXED(BarWidth), CLAY_SIZING_FIXED(BarHeight)},
            .padding = CLAY_PADDING_ALL(0),
            .childGap = 0,
            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .cornerRadius = CLAY_CORNER_RADIUS(BarRadius),
        .backgroundColor = PlainColor
    }) {
        Clay_String KnobName = {true, 10, "Name"};
        SliderKnob(5, COLOR_WHITE, KnobName);
    }
}