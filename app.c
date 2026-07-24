
/* NOTES:
 - stdscr is the window, initialised by initscr
 - refresh() sends the data written to the imaginery window (accessed via printw()) and then 
 - mvprintw(y, x, string); moves to y,x then prints the string
 - cbreak() disables line buffering
 */

#include <locale.h>
#include "rocket.h"
#include "app.h"
#include "controls.h"
#include "gamebar.h"

#include <math.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#define N_STARS 100
#define N_DOTS 100
#define WORLD_W 200
#define WORLD_H 200


/*
Function to draw the floor with slopes based on the surfaceLevel array
*/
void draw_floor(GameState *gs) {
    int prevH = gs->maxY - gs->surface.surfaceLevel[1] - 1;
    for (int x = 1; x < gs->maxX -1; x++) {
        
        int currH = (gs->maxY - gs->surface.surfaceLevel[x] - 1);
        int nextH = (gs->maxY - gs->surface.surfaceLevel[x+1] - 1);
        
        if (nextH < currH) mvaddch((currH), x, '/');
        else if (currH > prevH) mvaddch((currH), x, '\\');
        else if (currH == prevH) mvaddch((currH), x, '_');
        else if (prevH > currH && nextH == currH ) mvaddch((currH), x, '_');
        for (int y = gs->maxY - gs->surface.surfaceLevel[x]; y < gs->maxY; y++) {  mvaddch((y), x, '#');
        prevH = currH;
}
    }
}
/*
Function to draw the starry backdrop based on the backDrop struct stars array.
*/
void draw_backdrop(GameState *gs) {
    for (int i = 0; i < N_STARS; i++) {
        int x = gs->backDrop.stars[i].x;
        int y = gs->backDrop.stars[i].y;
        if (x < 1 || x >= gs->maxX - 1 || y < 1 || y >= gs->maxY - 1) continue;
        mvprintw(y, x, "☆");
    }
    for (int i = 0; i < N_DOTS; i++) {
        int x = gs->backDrop.dots[i].x;
        int y = gs->backDrop.dots[i].y;
        if (x < 1 || x >= gs->maxX - 1 || y < 1 || y >= gs->maxY - 1) continue;
        mvprintw(y, x, ".");
    }
    
}

// mvprintw(0, 0, "hello");

/*
Initialises the star backdrop using randomness.
*/
void initBackDrop(GameState *gs) {
    gs->backDrop.stars = malloc(N_STARS * sizeof(Stars));
    for (int i = 0; i < N_STARS; i++) {
        gs->backDrop.stars[i].x = rand() % WORLD_W;
        gs->backDrop.stars[i].y = rand() % WORLD_H;
    }
    gs->backDrop.dots = malloc(N_DOTS * sizeof(Dots));
    for (int i = 0; i < N_STARS; i++) {
        gs->backDrop.dots[i].x = rand() % WORLD_W;
        gs->backDrop.dots[i].y = rand() % WORLD_H;
    }
}


/*
Initialises Surface Floor Terrain as sin wave to WORLD_W limit.
*/ 
void initFloor(GameState *gs) {
    gs->surface.surfaceLevel = malloc(WORLD_W * sizeof(int));
    for (int i = 0; i < WORLD_W; i++)
        gs->surface.surfaceLevel[i] = 8 + (int)(5 * sin(i * 0.1));
}


/*
Initialises the window settings
*/
void initWindow() {
    setlocale(LC_ALL, ""); // allows unicode
    initscr();
    start_color();
    use_default_colors(); 
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_RED, -1);
    timeout(1); // polling rate
    curs_set(0);
	noecho();
	cbreak();
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    refresh();
}
  
/*
Initialises the game state struct
*/
void initGameState(GameState *gs, WINDOW *stdscr) {
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    gs->score = 0;
    gs->landed = NULL;
    gs->maxX = maxX;
    gs->maxY = maxY;
}

/*
Detects if a collision has taken place and if so calculates if its a successful landing
*/
int collideDetect(GameState *gs) {
    int rocketX = gs->rocket.x;
    int rocketY = (gs->maxY - gs->rocket.y)  -  1;
    int surfaceY = gs->surface.surfaceLevel[rocketX];
 
    if (rocketY <= surfaceY) { // touched land
        int prevY = gs->surface.surfaceLevel[rocketX - 1];
        int nextY = gs->surface.surfaceLevel[rocketX + 1];
        if ((nextY == rocketY && prevY > rocketY) || (nextY > rocketY && prevY == rocketY) || (rocketY < surfaceY) )  {  // Is the ground slanted or below?
            gs->landed = false;
            return 1;
        }
        if (gs->rocket.velocityY >= 1 || gs->rocket.velocityY <= -1 || gs->rocket.velocityX >= 0.2 || gs->rocket.velocityX <= -0.2){
            gs->landed = false;
            return 1;
        }

        if (gs->rocket.rotation != 90){
            gs->landed = false;
            return 1;
        }

        gs->landed = true;
        return 2;
        }
        else return 0; //mvprintw(25, 10, "Landed");

    }

/*
(WIP)The Initial Game Menu that will allow for
- Highscore table
- Score of previous run
- Quit or retry
- see if you landed or not
*/
int gameMenu(GameState *gs) {


    char *items[] = {"WELCOME TO (FAKE) LUNAR LANDER", "New Game", "Leaderboard (WIP)", "Quit"};
    if (gs->landed==true) items[0] = "SUCCESSFUL LANDING !"; // Alter the Main Game Board Message
    if (gs->landed==false) items[0] = "YOU CRASHED :(";
    
    int size = sizeof(items) / sizeof(items[0]);
    int box_h = gs->maxY / 2, box_w = gs->maxX / 2;
    int starty = (LINES - box_h) / 2;
    int startx = (COLS - box_w) / 2;
    int choice = 1;

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // create a subwindow and draw a border around it
    WINDOW *menu = newwin(box_h, box_w, starty, startx);
    keypad(menu, TRUE);
    box(menu, 0, 0);       
    int menu_top = (box_h - size) / 2;

    int ch;
    while (1) {
        werase(menu);
        box(menu, 0, 0); 
        
        for ( int i = 0; i < size; i++) {
            int len = strlen(items[i]);
            int x = (box_w - len) / 2;
            int y = menu_top + i;

            if (i == choice) 
            {
                wattron(menu, A_REVERSE);
                mvwprintw(menu, y, x, "%s", items[i]);  
                wattroff(menu, A_REVERSE);
            } else {
                mvwprintw(menu, y, x, "%s", items[i]); 
            }
            wrefresh(menu);
        }
        wrefresh(menu);
        
        ch = wgetch(menu);
        if (ch == KEY_UP) choice = (choice > 0) ? choice - 1 : 0;
        if (ch == KEY_DOWN) choice = (choice < size - 1) ? choice + 1 : size - 1;
        if (ch == '\n') break;
    }

    if (choice == 1) { // New Game
        delwin(menu);
        return 0;
    }
    if (choice == 3) { // Quit
        delwin(menu);
        return 1;
    }




    
}



/*
Main Game Entry Function
*/
int main()
{	
    struct GameState *gs = malloc(sizeof *gs);

    // Timer for game loop
    struct timeval start;
    struct timeval end;

    /* All Config Initialisation */
    initWindow();
    initGameState(gs, stdscr);
    initBackDrop(gs);
    initFloor(gs);
    initRocket(gs,(gs->maxX / 2) , ((gs->maxY / 2) - 10)); // this initialises the rocket at the x and y coordinates
    
    /*
    Drawing Logic Will Go here
    */

    draw_floor(gs);
    draw_rocket(gs);
    draw_backdrop(gs);

    // Initialise the game loop timer

    gettimeofday(&start, NULL);

    while (1) {

        int ch; 

        /* This is so the game adapts to the terminal changing mid game, might remove later?*/
        int maxX, maxY;
        getmaxyx(stdscr, maxY, maxX);
        gs->maxX = maxX;
        gs->maxY = maxY;
        if (maxY < 20 || maxX < 20 || maxX > 200 || maxY > 200) {return 1;} // If the window gets too small then cut it
        werase(stdscr);
        
        draw_backdrop(gs);
        draw_floor(gs);
        box(stdscr, 0, 0);
        drawGameBar(gs); 
        draw_rocket(gs);

        ch = getch();
        controlRocket(ch, gs);
        gettimeofday(&end, NULL);
        double elapsed =  (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;


        // Physics Handling
        if (elapsed >= 0.016) {  // was 0.016
            update_rocket(gs, elapsed);
            draw_rocket(gs);
            refresh();
            int coll = collideDetect(gs);
            if (coll == 1 || coll == 2) {

                int sx = (int)gs->rocket.x;
                gs->rocket.y = gs->maxY - gs->surface.surfaceLevel[sx] - 2;
                gs->rocket.velocityX = 0;
                gs->rocket.velocityY = 0;

                werase(stdscr);
                draw_backdrop(gs);
                draw_floor(gs);
                box(stdscr, 0, 0);
                drawGameBar(gs);
                draw_rocket(gs);

                if (coll == 1)
                    mvprintw(gs->maxY / 2, (gs->maxX / 2) - 4, "CRASHED!");
                else if (coll == 2)
                    mvprintw(gs->maxY / 2, (gs->maxX / 2) - 4, "LANDED!");

                refresh();
                sleep(1);
                break; // crashed/landed so break out of main game loop
            }
            start = end;
        }
        draw_rocket(gs);
        refresh();
    } // end of the main game loop

    if (gameMenu(gs) == 1) {
        endwin();
    }
    if (gameMenu(gs) == 0) {
        endwin();
        main();
    }
    endwin();
}

