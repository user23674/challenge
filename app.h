
#include "rocket.h"
#include <ncurses.h>


struct Stars {int x, y;};
typedef struct Stars Stars;

struct Dots {int x, y;};
typedef struct Dots Dots;


struct BackDrop {
    Stars *stars;
    Dots *dots;
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
    bool landed; // keeps track of if the player just landed successfully
    int maxX;
    int maxY;
};
typedef struct GameState GameState;




void initGameState(GameState *gs, WINDOW *stdscr);