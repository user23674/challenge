#ifndef GAMEBAR_H

#include "rocket.h"


// Panel size in character cells. Content lives on rows 1..(GAMEBAR_H-2).
#define GAMEBAR_W 30
#define GAMEBAR_H 8

void drawGameBar(GameState *gs);

#endif
