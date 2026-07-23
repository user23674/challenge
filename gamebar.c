#include "gamebar.h"
#include "app.h"
#include <math.h>
#include <string.h>

/*
  Create the telemetry overlay window, centred at the top of the screen.
  Position is refreshed every frame in drawGameBar, so the value passed to
  newwin here is only the starting spot.
*/
WINDOW *initGameBar(GameState *gs) {
    int x = (gs->maxX - GAMEBAR_W) / 2;
    if (x < 0) x = 0;
    return newwin(GAMEBAR_H, GAMEBAR_W, 0, x);
}

/*
  Draw one frame of telemetry.

  Velocities are stored in px/s; with 1 px = 10 m they are multiplied by 10
  here so the bar reads in m/s.  velocityY is positive downward (screen y
  grows down), so a positive Vy means the lander is descending.

  Call this AFTER stdscr has been staged (wnoutrefresh(stdscr)) and let the
  main loop finish with doupdate() so the bar always sits on top.
*/
void drawGameBar(WINDOW *bar, GameState *gs) {
    Rocket *r = &gs->rocket;

    // Re-centre at the top each frame so it survives terminal resizes.
    int x = (gs->maxX - GAMEBAR_W) / 2;
    if (x < 0) x = 0;
    mvwin(bar, 0, x);

    werase(bar);
    box(bar, 0, 0);

    // Title, centred on the top border.
    const char *title = " TELEMETRY ";
    mvwprintw(bar, 0, (GAMEBAR_W - (int)strlen(title)) / 2, "%s", title);

    // px/s -> m/s
    float vx    = r->velocityX * 10.0f;
    float vy    = r->velocityY * 10.0f;
    float speed = sqrtf(vx * vx + vy * vy);

    mvwprintw(bar, 1, 2, "Thrust : %s",          r->thrust ? "ON " : "OFF");
    mvwprintw(bar, 2, 2, "Vx     : %+6.1f m/s",  vx);
    mvwprintw(bar, 3, 2, "Vy     : %+6.1f m/s",  vy);   // + = descending
    mvwprintw(bar, 4, 2, "Speed  : %6.1f m/s",   speed);
    mvwprintw(bar, 5, 2, "Fuel   : %6.1f",       r->fuel);
    // Row 6 is intentionally left free for future data
    // (altitude, rotation, score, landing-status, etc.)

    // Stage this window; the main loop's doupdate() paints it on top.
    wnoutrefresh(bar);
}
