#include "rocket.h"
#include "app.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>

// #define M_PI 3.14159265
#define GRAV 0.2f
#define THRUST_SCALE 0.05f
#define TERMINAL_VELOCITY 5.0f
#define FUEL_BURN_RATE 2.0f


void initRocket(GameState *gameState, int starting_pos_x, int starting_pos_y) {
    gameState->rocket.x = (float)starting_pos_x;
    gameState->rocket.y = (float)starting_pos_y;
    gameState->rocket.rotation = 90;
    gameState->rocket.velocityX = 0;
    gameState->rocket.velocityY = 0;
    gameState->rocket.mass = 10;
    gameState->rocket.thrust = false;
    gameState->rocket.fuel = 100;
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
    if (gameState->rocket.rotation <= 95 && gameState->rocket.rotation >= 85  ) {
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x+1, '\\');
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x-1, '/');
    }
    else if (gameState->rocket.rotation > 95 && gameState->rocket.rotation < 165  ) {   
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x, '\\');
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x+1, '\\');
    }

    else if (gameState->rocket.rotation < 95 && gameState->rocket.rotation > 15  ) {   
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x, '/');
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x-1, '/');
    }




    else if (gameState->rocket.rotation >= 165 ) {
        mvaddch(gameState->rocket.y, gameState->rocket.x + 1, '<');
    }
    else if (gameState->rocket.rotation <= 15) {
        mvaddch(gameState->rocket.y, gameState->rocket.x - 1, '>');
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
    float angle_rad = gs->rocket.rotation * (M_PI / 180.0f);
    
    // Use 1.0f when thrust is on, 0.0f when off
    if gs->rocket.thrust == true
    
    float thrust_x = thrust_magnitude * cosf(angle_rad) * THRUST_SCALE;
    float thrust_y = -thrust_magnitude * sinf(angle_rad) * THRUST_SCALE;
    
    float accel_x = thrust_x / gs->rocket.mass;
    float accel_y = (thrust_y / gs->rocket.mass) + GRAV;
    
    gs->rocket.velocityX += accel_x * delta_time;
    gs->rocket.velocityY += accel_y * delta_time;
    
    // Apply terminal velocity
    float speed = sqrtf(gs->rocket.velocityX * gs->rocket.velocityX + 
                        gs->rocket.velocityY * gs->rocket.velocityY);
    if (speed > TERMINAL_VELOCITY) {
        float scale = TERMINAL_VELOCITY / speed;
        gs->rocket.velocityX *= scale;
        gs->rocket.velocityY *= scale;
    }
    
    gs->rocket.x += gs->rocket.velocityX * delta_time;
    gs->rocket.y += gs->rocket.velocityY * delta_time;
    
    // Consume fuel only when thrust is on
    if (gs->rocket.thrust && gs->rocket.fuel > 0) {
        gs->rocket.fuel -= FUEL_BURN_RATE * delta_time;
        if (gs->rocket.fuel < 0) gs->rocket.fuel = 0;
    }

    clamp(gs, &(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
}
