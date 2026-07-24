#include "gamebar.h"
#include "app.h"
#include <ncurses.h>
#include <math.h>
#include <string.h>


void drawGameBar(GameState *gs) {
    Rocket *r = &gs->rocket;

    const int w = GAMEBAR_W;
    const int h = GAMEBAR_H;

    int x0 = (gs->maxX - w) / 2;
    if (x0 < 1) x0 = 1;
    int y0 = 0;

    // --- Border ---
    for (int i = 0; i < w; i++) {
        mvaddch(y0, x0 + i,     ACS_HLINE);
        mvaddch(y0 + h - 1, x0 + i,  ACS_HLINE);
    }
    for (int j = 0; j < h; j++) {
        mvaddch(y0 + j,     x0,         ACS_VLINE);
        mvaddch(y0 + j,     x0 + w - 1, ACS_VLINE);
    }
    mvaddch(y0, x0,  ACS_ULCORNER);
    mvaddch(y0,  x0 + w - 1, ACS_URCORNER);
    mvaddch(y0 + h - 1, x0,  ACS_LLCORNER);
    mvaddch(y0 + h - 1, x0 + w - 1, ACS_LRCORNER);

    const char *title = " STATS ";
    mvprintw(y0, x0 + (w - (int)strlen(title)) / 2, "%s", title);

    float vx = r->velocityX;
    float vy = r->velocityY;

    mvprintw(y0 + 1, x0 + 2, "Thrust : %s", r->thrust ? "ON " : "OFF");
    mvprintw(y0 + 2, x0 + 2, "Vx     : %+.3f", vx);
    mvprintw(y0 + 3, x0 + 2, "Vy     : %+.3f", vy);   // + = descending
    mvprintw(y0 + 4, x0 + 2, "Fuel   : %.1f",  r->fuel);
}
