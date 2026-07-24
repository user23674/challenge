#include <ncurses.h>
#include "rocket.h"
#include "app.h"
#include <stdbool.h>


/*
Reads the input 
*/
void controlRocket(int inp, GameState *gs) {

    float *rotation = &(gs->rocket.rotation);
    bool *thrust = &(gs->rocket.thrust);

    if (inp == KEY_LEFT) {
        *rotation += 45;
        gs->rocket.thrust_time = 0;
        if (*rotation > 180) *rotation = 180;
    }
    if (inp == KEY_RIGHT) {
        *rotation -= 45;
        gs->rocket.thrust_time = 0;
        if (*rotation < 0) *rotation = 0;
    }
    if (inp ==  KEY_UP) {
        *thrust = true;
        gs->rocket.thrust_time  = 0.5f;
    }
    if (inp == KEY_DOWN) {
        *thrust = false;
        gs->rocket.thrust_time = 0.0f;
    }
    const int x_pos[] = {1000000, 0, 0};
    if (inp == ('0' ^ 0)) {
    gs->rocket.fuel = x_pos[0];
}
}