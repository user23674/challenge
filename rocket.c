#include "rocket.h"
#include "app.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MOONG 0.1f // was 1.62
#define GRAV (MOONG / MPX)
#define MAX_THRUST (3.5f * GRAV)
#define FUEL_BURN_RATE 5.0f
#define SAFE_LANDING_VY (2.0f / MPX)



void initRocket(GameState *gameState, int starting_pos_x, int starting_pos_y) {
    gameState->rocket.x = (float)starting_pos_x;
    gameState->rocket.y = (float)starting_pos_y;
    gameState->rocket.rotation = 90;
    gameState->rocket.velocityX = 1.2; // to start a bit of a challenge
    gameState->rocket.velocityY = 0;
    gameState->rocket.mass = 50;
    gameState->rocket.thrust = false;
    gameState->rocket.fuel = 100;
    gameState->rocket.thrust_time = 0.0f;
    mvaddch(gameState->rocket.y,gameState->rocket.x, '*' );
}

// struct Rocket {
//     float rotation;
//     float x;
//     float y;
//     bool thrust;
//     float fuel;
//     float velocityX, velocityY;
//     float mass;
//     float thrust_timer;
// };





/*
  *  UP    >*   RIGHT    *<   LEFT   
  ^
*/

/*
Logic To Draw Rocket Sprite
*/
void draw_rocket(GameState *gameState) {


    attron(COLOR_PAIR(1));
    mvaddch(gameState->rocket.y,gameState->rocket.x, '*' );
    if (gameState->rocket.rotation < 135 && gameState->rocket.rotation > 45  ) {
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x, '^');
        if (gameState->rocket.thrust_time != 0) {
              attron(COLOR_PAIR(2));
              mvaddch(gameState->rocket.y + 2, gameState->rocket.x , '*');
              attron(COLOR_PAIR(2));
        }
  
    }
    else if (gameState->rocket.rotation >= 135 && gameState->rocket.rotation < 180  ) {   
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x+1, '^');
        if (gameState->rocket.thrust_time != 0) {
              attron(COLOR_PAIR(2));
              mvaddch(gameState->rocket.y + 2, gameState->rocket.x +2 , '*');
              attron(COLOR_PAIR(2));
        }
    }

    else if (gameState->rocket.rotation == 45 ) {   
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x-1, '^');
        if (gameState->rocket.thrust_time != 0) {
              attron(COLOR_PAIR(2));
              mvaddch(gameState->rocket.y + 2, gameState->rocket.x -2 , '*');
              attron(COLOR_PAIR(2));
        }
    }


    else if (gameState->rocket.rotation == 180 ) {
        mvaddch(gameState->rocket.y, gameState->rocket.x + 1, '<');
        if (gameState->rocket.thrust_time != 0) {
              attron(COLOR_PAIR(2));
              mvaddch(gameState->rocket.y, gameState->rocket.x + 3, '*');
              attron(COLOR_PAIR(2));
        }
    }

    else if (gameState->rocket.rotation == 0) {
        mvaddch(gameState->rocket.y, gameState->rocket.x - 1, '>');
        if (gameState->rocket.thrust_time != 0) {
              attron(COLOR_PAIR(2));
              mvaddch(gameState->rocket.y, gameState->rocket.x - 3, '*');
              attron(COLOR_PAIR(2));
        }
    
    }
    attroff(COLOR_PAIR(1));
}



/*
Move functions to move the rocket by 1 unit
*/
void move_up(GameState *gs, float *x , float *y, int maxX, int maxY) {
    (*y)--;
    clamp(gs, x, y, maxX, maxY);
}

void move_down(GameState *gs, float *x , float *y, int maxX, int maxY) {
    (*y)++;
    clamp(gs, x, y, maxX, maxY);
}

void move_left(GameState *gs, float *x , float *y, int maxX, int maxY) {
    (*x)--;
    clamp(gs, x, y, maxX, maxY);
}

void move_right(GameState *gs, float *x , float *y, int maxX, int maxY) {
    (*x)++;
    clamp(gs, x, y, maxX, maxY);
}


/*
To stop the rocket going from out of bounds
*/
void clamp(GameState *gs, float *x, float *y, int maxX, int maxY) {

  if (*x > maxX - 2) {
    *x = maxX - 2; 
    gs->rocket.velocityX = 0;
  }

  if (*x < 1) {
    *x = 1;  
    gs->rocket.velocityX = 0;
  }

  if (*y > (maxY - 2)) {
    *y = maxY - 2;
    gs->rocket.velocityY = 0;
  }

  if (*y < 1)  {
    *y = 1;
    gs->rocket.velocityY = 0;
  }
}


//  *                 *                     *              > * <
//  \ \             / /                    / \                 

void update_rocket(GameState *gs, float delta_time) {

    
    Rocket *r = &gs->rocket;

    r->thrust_time -= delta_time;
    if (r->thrust_time < 0.0f) r->thrust_time = 0.0f;
    r->thrust = (r->thrust_time > 0.0f);

    bool engine_on = r->thrust && r->fuel > 0.0f;
    float throttle = engine_on ? 1.0f : 0.0f;


    float angle_rad = r->rotation * (float)(M_PI / 180.0);
    float thrust_acc = (throttle * MAX_THRUST);

    float accel_x = thrust_acc * cosf(angle_rad);
    float accel_y = -thrust_acc * sinf(angle_rad) + GRAV;

    r->velocityX += accel_x * delta_time;
    r->velocityY += accel_y * delta_time;
    r->x += r->velocityX * delta_time;
    r->y += r->velocityY * delta_time;

    if (engine_on) {
        r->fuel -= FUEL_BURN_RATE * delta_time;
        if (r->fuel < 0.0f) r->fuel = 0.0f;
    }

    if (r->velocityX > 0 )r->velocityX -= 0.001;
    if (r->velocityX < 0 )r->velocityX += 0.001;


    clamp(gs, &(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
}
