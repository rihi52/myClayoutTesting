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

#endif /* SLIDER_H */