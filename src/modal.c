#include "clay.h"
#include "styles.h"
#include "global.h"
#include "stdio.h"
#include "modal.h"

float ModalTimer = MODAL_TIMER;

/*========================================================================* 
 *  SECTION - Global Functions
 *========================================================================* 
 */
Clay_ElementId CreateBottomModal(Clay_ElementId ParentID, Clay_String ModalText) {
    CLAY_AUTO_ID({
        .layout = {
            .sizing = {CLAY_SIZING_FIT(0), CLAY_SIZING_FIXED(100)},
            .padding = {
                .left = 16,
                .right = 16,
                .top = 8,
                .bottom = 8
            },
            .childGap = 0,
            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = COLOR_MODAL_BG,
        .cornerRadius = CLAY_CORNER_RADIUS(GLOBAL_RADIUS_SM_PX),
        .floating = {
            .offset = {0, 0},
            .parentId = ParentID.id,
            .zIndex = 0,
            .attachPoints = {
                .element = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                .parent = CLAY_ATTACH_POINT_CENTER_BOTTOM
            },
            .attachTo = CLAY_ATTACH_TO_PARENT,
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE
        }
    }) {
        CLAY_TEXT(ModalText, CLAY_TEXT_CONFIG(ButtonTextConfig));
    }
}

 /*========================================================================* 
 *  SECTION - Local Functions
 *========================================================================* 
 */