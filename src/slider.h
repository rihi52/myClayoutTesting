#ifndef SLIDER_H
#define SLIDER_H

#include "clay.h"
#include "stdio.h"
#include "stdint.h"

typedef struct SliderValues {
    uint32_t MaximumValue;
    uint32_t MinimumValue;
    uint32_t CurrentValue;
}SliderValues;

void SliderBar (Clay_String BarName, int BarWidth, int BarHeight, int BarRadius, Clay_Color PlainColor, Clay_Color HighlightedColor);

#endif /* SLIDER_H */