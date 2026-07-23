#include <ncurses.h>
#include "rocket.h"
#include "app.h"
#include <stdbool.h>


void controlRocket(int inp, GameState *gs) {

    float *rotation = &(gs->rocket.rotation);
    bool *thrust = &(gs->rocket.thrust);

    if (inp == KEY_LEFT) {
        *rotation += 45;
        if (*rotation > 180) *rotation = 180;
    }
    if (inp == KEY_RIGHT) {
        *rotation -= 45;
        if (*rotation < 0) *rotation = 0;
    }
    if (inp ==  KEY_UP) {
        *thrust = true;
        gs->rocket.thrust_time  = 0.45f;
    }
    if (inp == KEY_DOWN) {
        gs->rocket.thrust_time = 0.0f;
    }
}