/* 
 * clientCmds.c - Handles the client-side functions of the game
 *
 * nunchuck-buccaneers, May 2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "pos2D.h"
#include "mem.h"
#include "network.h"
#include <unistd.h>
#include "clientCmds.h"


/**************** global integer ****************/
static const int headerLen = 52;
static const int actionLen = 19;

/*****************************************************************************/
/************************** Local functions ********************************/

/* *********** clearHeader *********** */
/* sets the header part of the top line to blank spaces in ncurses
 * does not 'refresh()'
 */
void 
clearHeader(void)
{
    int x = 0;
    // NOTE: may need to be '<='?
    while (x <= headerLen) {
        move(0,x);
        addch(' ');
        x++;
    }
    return;
}
/* ********** clearAction ********** */
/* sets the action part of the top line to blank spaces in ncurses
 * does not 'refresh()'
 */
void
clearAction(void)
{   
    int y = 0;
    int x = 0;
    int maxX = 0;
    maxX = getmaxx(stdscr); // the width of the screen
    maxX -= 2;
    move(0, maxX - actionLen);
    x = maxX - actionLen;
    while (x <= maxX) {
        addch(' ');
        move(y,x+1);
        getyx(stdscr, y, x);
    }
}
/* ********** clearDisplay ********** */
/* sets the display part of the top line to blank spaces in ncurses
 * does not 'refresh()'
 */
void
clearDisplay(void){
    int NROWS;  // the number of rows on the client 
    int NCOLS;  // the number of columns on the client
    getmaxyx(stdscr, NROWS, NCOLS);
    // clear the old Display message
    for(int y = 1; y < NROWS; y++){
        for(int x = 0; x < NCOLS; x++){
            move(y, x);
            addch(' ');
        }
    }
}

/*************** displayHeader() ****************/
/*
 * What it does: Display just the top line header that the serve sends to the client
 * Parameters: n - number of gold left remaining
 *             p - amount of gold that this player has in their purse
 *             r - amount of gold just collected by this player (if applicable)
 *             ID - character for the player ID
 * Returns: Nothing
 */
void displayHeader(int n, int p, int r, char ID)
{   
    // NOTE: buffer may need to be changed if the message is changed
    //       n, p, r should not exeed 999
    char* header= mem_malloc_assert((headerLen * sizeof(char)) , 
            "displayerHeader: out of memory");
    if (p == -1) {
        sprintf(header, "Spectator: %d nuggets unclaimed.", r);
    } else {
        sprintf(header, "Player %c has %d nuggets (%d nuggets unclaimed).", 
                ID, p, r);
    }
    if (n != 0) {
        // buffer may need to change if the message is changed
        char* action = mem_malloc_assert((actionLen * sizeof(char)), 
                "displayerHeader: out of memeory");
        sprintf(action, "Gold received: %d", n);
        displayAction(action);
        free(action);
    }
    // clear the header part of the top line
    clearHeader();

    // set the curser to the upper left corner
    move(0,0);
    int i = 0;
    int x;
    int y;
    // loop through the header char* and print it to the screen
    while (header[i] != '\0') {
        getyx(stdscr, y, x);
        addch(header[i]);
        move(y,x+1);
        i++;
    }
    refresh();
    free(header);
    return;
}

/*************** displayAction() **************/
/*
 * What it does: displays an action on the right side of the header line
 * Parameters: message - char*, the message to be written to the right hand corner
 * Returns: nothing
 */
void
displayAction(const char* message)
{
    if (message == NULL){
        fprintf(stderr, "displayAction(): NULL 'message' passed\n");
        return;
    }
    // clear the action part of the top line
    clearAction();
    int x;
    int y;
    int NCOLS;
    int i = 0;
    // We only need the number of columns here
    NCOLS = getmaxx(stdscr);
    // set the curser to somwhere in the middle of the header
    // such that the message will reach the corner
    move(0, NCOLS - strlen(message) - 1);
    // loop through the message char* and print it to the screen
    while (message[i] != '\0') {
        getyx(stdscr, y, x);
        addch(message[i]);
        move(y,x+1);
        i++;
    }
    refresh();
    return;
}

/**************** displayGrid()  ****************/
/*
 * What it does: Display the grid (the map) that the server sends to the client
 * Parameters: grid - string representation of a map that the client can read
 * Assume: The string representation is valid. If the player has resized their
 *         display window since they originally joined, display as much of the
 *         map on the screen as you can.
 * Returns: Nothing
 */
void
display(const char* grid)
{
    if (grid == NULL) {
        fprintf(stderr, "displayGrid(): NULL 'grid' passed\n");
        return;
    }
    clearDisplay();
    move(1,0);
    int i = 0;
    int x;
    int y;
    while (grid[i] != '\0') {
        getyx(stdscr, y, x);
        if (grid[i] == '\n'){
            move(y + 1, 0);
        } else {
            addch(grid[i]);
            move(y, x +1);
        }
        i++;
    }
    refresh();
    return;
}

/**************** ensureDimensions() ****************/
/*
 * What it does: This function is called when a user fails to join. It sends 
 * a join message after resizing.
 * Parameters: display_HW - the height and width of the grid's display
 * Returns: Nothing
 */
void
ensureDimensions(pos2D_t* display_hW)
{
    if(display_hW == NULL){
        fprintf(stderr, "ensureDimensions(): NULL 'pos' passed\n");
        return;
    }
    if(pos2D_getX(display_hW) <= 0 || pos2D_getY(display_hW) <= 0){
        fprintf(stderr, "ensureDimensions(): Negative HW passed\n");
        return;
    }
    //Reset Screen
    clearAction();
    clearHeader();
    clearDisplay();
    refresh();
    int NROWS;  // the number of rows on the client 
    int NCOLS;  // the number of columns on the client
    int displayW = pos2D_getX(display_hW);      // the number of columns on server
    int displayH = pos2D_getY(display_hW);      // the number of rows on server

    // if displayW is less than the width required to print header + action
    // set displayW to the width of header + action + 1
    int minTotalTopWidth = headerLen + actionLen + 1;
    if (displayW < minTotalTopWidth) {
        displayW = minTotalTopWidth;
    }
    displayH++;
    getmaxyx(stdscr, NROWS, NCOLS);
    while (displayW > NCOLS || displayH > NROWS) {
        char* printS = mem_malloc_assert((130 * sizeof(char)), 
                "ensureDimensions: out of memory");
        sprintf(printS, "adjust screen width and height to fit requirements: \n"
                "Current Width: %d, required: %d\n"
                "Current Height: %d, required: %d", 
                NCOLS, displayW, NROWS, displayH);
        int i = 0;
        // set the curser to the upper left corner
        move(0,0);
        int x;
        int y;
        // loop through the string to print, and print it in ncurses
        while (printS[i] != '\0') {
            getyx(stdscr, y, x);
            if (printS[i] == '\n'){
                move(y + 1, 0);
            } else {
                addch(printS[i]);
                move(y, x +1);
            }
            i++;
        }
        refresh();
        free(printS);
        getmaxyx(stdscr, NROWS, NCOLS);
    }
    // clear the ensureDimensions message
    for(int y = 0; y < NROWS; y++){
        for(int x = 0; x < NCOLS; x++){
            move(y, x);
            addch(' ');
        }
    }
}

/***************** quitClient *****************/
/*
 * What it does: quits curses, prints a quit explanation string, exits 0
 *               all free the passed explanation, since we exit here
 * Parameters: explanation - a string passed by server to be printed after quit
 * return: nothing, this function exits the program.
 */
void
quitClient(char* explanation)
{
    if(explanation == NULL){
        fprintf(stderr, "quitClient(): NULL explanation passed\n");
        return;
    }
    // exit Ncurses window
    endwin();
    printf("%s\n", explanation);
}

