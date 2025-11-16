#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "clay.h"
#include "global.h"

#define MAX_VISIBLE_HEADERS 12


Clay_RenderCommandArray MainWindow(AppState * state);
void MakeCreatureHeader(int i, int CallingWindow);

#endif /* MAIN_WINDOW_H */