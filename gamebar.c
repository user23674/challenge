#include "gamebar.h"
#include "app.h"
#include <ncurses.h>
#include <math.h>
#include <string.h>

/*
  Draw the telemetry panel onto stdscr, centred along the top edge.

  Velocities are stored in px/s; with 1 px = 10 m they are multiplied by 10
  here so the bar reads in m/s. velocityY is positive downward (screen y grows
  down), so a positive Vy means the lander is descending.

  Draw this BEFORE getch() each frame so the implicit refresh getch performs
  already contains the bar (that is what prevents the flicker).
*/
void drawGameBar(GameState *gs) {
    Rocket *r = &gs->rocket;

    const int w = GAMEBAR_W;
    const int h = GAMEBAR_H;

    // Centre horizontally, pinned to the top. Stay inside the outer box.
    int x0 = (gs->maxX - w) / 2;
    if (x0 < 1) x0 = 1;
    int y0 = 0;

    // --- Border ---
    for (int i = 0; i < w; i++) {
        mvaddch(y0,         x0 + i,     ACS_HLINE);
        mvaddch(y0 + h - 1, x0 + i,     ACS_HLINE);
    }
    for (int j = 0; j < h; j++) {
        mvaddch(y0 + j,     x0,         ACS_VLINE);
        mvaddch(y0 + j,     x0 + w - 1, ACS_VLINE);
    }
    mvaddch(y0,         x0,         ACS_ULCORNER);
    mvaddch(y0,         x0 + w - 1, ACS_URCORNER);
    mvaddch(y0 + h - 1, x0,         ACS_LLCORNER);
    mvaddch(y0 + h - 1, x0 + w - 1, ACS_LRCORNER);

    // --- Title on the top border ---
    const char *title = " TELEMETRY ";
    mvprintw(y0, x0 + (w - (int)strlen(title)) / 2, "%s", title);

    // --- Data (px/s -> m/s) ---
    float vx    = r->velocityX * 10.0f;
    float vy    = r->velocityY * 10.0f;
    float speed = sqrtf(vx * vx + vy * vy);

    mvprintw(y0 + 1, x0 + 2, "Thrust : %s",         r->thrust ? "ON " : "OFF");
    mvprintw(y0 + 2, x0 + 2, "Vx     : %+6.1f m/s", vx);
    mvprintw(y0 + 3, x0 + 2, "Vy     : %+6.1f m/s", vy);   // + = descending
    mvprintw(y0 + 4, x0 + 2, "Speed  : %6.1f m/s",  speed);
    mvprintw(y0 + 5, x0 + 2, "Fuel   : %6.1f",      r->fuel);
    // Row 6 free for future data (altitude, rotation, score, status...)
}
