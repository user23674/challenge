
/* NOTES:
 - stdscr is the window, initialised by initscr
 - refresh() sends the data written to the imaginery window (accessed via printw()) and then 
 - mvprintw(y, x, string); moves to y,x then prints the string
 - cbreak() disables line buffering
 */


#include "rocket.h"
#include "app.h"

#include <stdio.h>
#include <ncurses.h>

void draw_floor() {
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX); 
    for (int x = 1; x < maxX -1; x++) {
        mvaddch(maxY -5, x, '-');
    }
}


// mvprintw(0, 0, "hello");


/*
Initialises the window settings
*/
void initWindow() {
    initscr();
    timeout(1); // polling rate
    curs_set(0);
	noecho();
	cbreak();
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    refresh();
}
  

/*
Initialises the game state
*/
void initGameState(GameState *gs, WINDOW *stdscr) {
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    gs->score = 0;
    gs->maxX = maxX;
    gs->maxY = maxY;
}


int main()
{	
    struct GameState *gs;

    /* All Config Initialisation */
    initWindow();
    initGameState(gs, stdscr);

    int STARTING_Y_POS = ((gs->maxY / 2) - 10); 
    int STARTING_X_POS = (gs->maxX / 2);

    initRocket(gs, STARTING_X_POS, STARTING_Y_POS);
    
    /*
    Drawing Logic Will Go here
    */

    draw_floor();
    draw_rocket(gs);


    while (1) {

        int ch;

        werase(stdscr);
        draw_floor();
        draw_rocket(gs);
        box(stdscr, 0, 0);
        refresh();





        ch = getch();
        if (ch == KEY_LEFT) 
        {
            move_left(&(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
        }

        else if (ch == KEY_RIGHT) 
        {
            move_right(&(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
        }

        else if (ch == KEY_UP) 

        {
            move_up(&(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
            
        }

        else if (ch == KEY_DOWN) 
        {
            move_down(&(gs->rocket.x), &(gs->rocket.y), gs->maxX, gs->maxY);
        }
    }
    getch();
    endwin();
}
