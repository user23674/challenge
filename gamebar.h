#ifndef GAMEBAR_H
#define GAMEBAR_H

#include <ncurses.h>
#include "rocket.h"   // for GameState

/*
  The game bar is a separate ncurses WINDOW that floats on top of the
  playfield at the top-middle of the screen and shows live telemetry.

  Usage:
    WINDOW *bar = initGameBar(gs);   // once, after initGameState
    ...
    drawGameBar(bar, gs);            // every frame, staged on top of stdscr
    ...
    delwin(bar);                     // on shutdown
*/

// Overlay dimensions (cells). Content lives on rows 1..(GAMEBAR_H-2).
#define GAMEBAR_W 30
#define GAMEBAR_H 8

WINDOW *initGameBar(GameState *gs);
void    drawGameBar(WINDOW *bar, GameState *gs);

#endif
