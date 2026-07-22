#include "rocket.h"
#include "app.h"
#include <string.h>


void initRocket(GameState *gameState, int starting_pos_x, int starting_pos_y) {
    gameState->rocket.x = starting_pos_x;
    gameState->rocket.y = starting_pos_y;
    strcpy(gameState->rocket.rotation, "N");
    gameState->rocket.mHoriz = 0;
    gameState->rocket.mVert = 0;
    mvaddch(gameState->rocket.y,gameState->rocket.x, '*' );

}

/*
  *  UP    >*   RIGHT    *<   LEFT   
  ^
*/

/*
Logic To Draw Rocket Sprite
*/
void draw_rocket(GameState *gameState) {
    mvaddch(gameState->rocket.y,gameState->rocket.x, '*' );
    if (gameState->rocket.rotation == "N") {
        mvaddch(gameState->rocket.y + 1,gameState->rocket.x, '^' );
    }
    else if (gameState->rocket.rotation == "W") {}
}



/*
Move functions to move the rocket by 1 unit
*/
void move_up(int *x , int *y, int maxX, int maxY) {
    int prevX = *x;
    int prevY = *y;
    (*y)--;
    clamp(x, y, maxX, maxY);
}

void move_down(int *x , int *y, int maxX, int maxY) {
    int prevX = *x;
    int prevY = *y;
    (*y)++;
    clamp(x, y, maxX, maxY);
}

void move_left(int *x , int *y, int maxX, int maxY) {
    int prevX = *x;
    int prevY = *y;
    (*x)--;
    clamp(x, y, maxX, maxY);
}

void move_right(int *x , int *y, int maxX, int maxY) {
    int prevX = *x;
    int prevY = *y;
    (*x)++;
    clamp(x, y, maxX, maxY);
}


/*
To stop the rocket going from out of bounds
*/
void clamp(int *x, int *y, int maxX, int maxY) {
  if (*x > maxX - 2) *x = maxX - 2;
  else if (*x < 1) *x = 1;

  else if (*y > (maxY - 2)) *y = maxY - 2;
  else if (*y < 1)  *y = 1; 
}
