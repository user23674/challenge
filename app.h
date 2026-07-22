
#include "rocket.h"
#include <ncurses.h>

struct GameState {
    Rocket rocket;
    int score;
    int maxX;
    int maxY;
};

typedef struct GameState GameState;




void initGameState(GameState *gs, WINDOW *stdscr);