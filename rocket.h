#ifndef ROCKET_H
#define ROCKET_H


typedef struct GameState GameState;
// need to add thrust and fuel
struct Rocket {
    char rotation;
    int x;
    int y;
    int mHoriz;
    int mVert;
};

typedef struct Rocket Rocket;

void initRocket(GameState *gameState, int starting_pos_x, int starting_pos_y);
void draw_rocket(GameState *gameState) ;

void clamp(int *x, int *y, int maxX, int maxY); 

void move_up(int *x , int *y, int maxX, int maxY);
void move_down(int *x , int *y, int maxX, int maxY);
void move_left(int *x , int *y, int maxX, int maxY);
void move_right(int *x , int *y, int maxX, int maxY);

#endif