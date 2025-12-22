#ifndef MODAL_H
#define MODAL_H

#include "clay.h"
#include "stdio.h"
#include "stdint.h"

#define MODAL_TIMER 2.0F

extern float ModalTimer;

Clay_ElementId CreateBottomModal(Clay_ElementId ParentID, Clay_String ModalText);

#endif /* MODAL_H */