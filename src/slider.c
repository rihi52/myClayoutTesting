#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"

/* Slider style */
Clay_LayoutConfig SliderKnob =  {
    .padding = { 0, 0, 0, 0},
    .childGap = 0,
    .childAlignment =  { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .layoutDirection = CLAY_TOP_TO_BOTTOM
};

// Rename/redo everything, just experimenting
void SliderFunction () {
    CLAY(CLAY_ID("Slider"), {SliderKnob, .cornerRadius = CLAY_CORNER_RADIUS(5)}) {

    }
}