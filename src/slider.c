#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"

float KnobPosition = 0;

void AdjustSliderKnobCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
void SliderKnob (int KnobSize, Clay_Color KnobColor, Clay_String KnobName);

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
            .offset = { KnobPosition, 0},
            .attachPoints = {
                .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                .parent = CLAY_ATTACH_POINT_LEFT_CENTER
            },
            .attachTo = CLAY_ATTACH_TO_PARENT,
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE
        }
    }) {
        Clay_OnHover(AdjustSliderKnobCallback, NULL);
    }
}

void SliderBar (Clay_String BarName, int BarWidth, int BarHeight, int BarRadius, Clay_Color PlainColor, Clay_Color HighlightedColor) {
    Clay_ElementId BarId = CLAY_SID(BarName);
    gAppState->SliderId = BarId;
    CLAY(BarId, {
        .layout = {
            .sizing = {CLAY_SIZING_FIXED(BarWidth), CLAY_SIZING_FIXED(BarHeight)},
            .padding = CLAY_PADDING_ALL(0),
            .childGap = 0,
            .childAlignment = {CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER},
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .cornerRadius = CLAY_CORNER_RADIUS(BarRadius),
        .backgroundColor = PlainColor
    }) {
        Clay_String KnobName = {true, 10, "Name"};
        SliderKnob(BarHeight + 5, COLOR_WHITE, KnobName);
    }
}

void AdjustSliderKnobCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED) {
        gAppState->focusedId = elementId;

        Clay_ElementData ParentData = Clay_GetElementData(gAppState->SliderId);

        float BarX = ParentData.boundingBox.x;
        float BarWidth = ParentData.boundingBox.width;

        float NewX = MouseX - BarX;

        if (NewX > BarWidth) {
            KnobPosition = BarWidth;
        } else if (NewX < 0) {
            KnobPosition = 0;
        } else {
            KnobPosition = NewX;
        }
    }
}