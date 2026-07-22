
#include "rocket.h"
#include <ncurses.h>


struct Stars {int x, y;};
typedef struct Stars Stars;


struct BackDrop {
    Stars *stars;
};
typedef struct BackDrop BackDrop;

struct Surface {
    int *surfaceLevel;
};
typedef struct Surface Surface;

struct GameState {
    Rocket rocket;
    BackDrop backDrop;
    Surface surface;
    int score;
    int maxX;
    int maxY;
};
typedef struct GameState GameState;




void initGameState(GameState *gs, WINDOW *stdscr);