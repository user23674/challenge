#ifndef GAMEBAR_H
#define GAMEBAR_H

#include "rocket.h"   // for GameState

/*
  The game bar is a telemetry panel drawn at the top-middle of the screen.

  It is drawn directly onto stdscr (not a separate ncurses WINDOW) so it
  flows through the same single refresh as the rest of the playfield. This
  avoids the overlapping-window flicker you get when getch() implicitly
  refreshes stdscr over a floating window.

  Call drawGameBar every frame, BEFORE getch(), so each refresh paints a
  complete frame that already includes the bar.
*/

// Panel size in character cells. Content lives on rows 1..(GAMEBAR_H-2).
#define GAMEBAR_W 30
#define GAMEBAR_H 8

void drawGameBar(GameState *gs);

#endif
