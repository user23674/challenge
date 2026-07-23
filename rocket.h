#ifndef ROCKET_H
#define ROCKET_H
#include <stdbool.h>

typedef struct GameState GameState;
// need to add thrust and fuel
struct Rocket {
    float rotation;
    float x;
    float y;
    bool thrust;
    float fuel;
    float velocityX, velocityY;
    float mass;
};

typedef struct Rocket Rocket;

void initRocket(GameState *gameState, int starting_pos_x, int starting_pos_y);
void draw_rocket(GameState *gameState) ;

void clamp(GameState *gs, float *x, float *y, int maxX, int maxY); 

void move_up(GameState *gs, float *x , float *y, int maxX, int maxY);
void move_down(GameState *gs, float *x , float *y, int maxX, int maxY);
void move_left(GameState *gs, float *x , float *y, int maxX, int maxY);
void move_right(GameState *gs, float *x , float *y, int maxX, int maxY);
void update_rocket(GameState *gs, float delta_time);




#endif