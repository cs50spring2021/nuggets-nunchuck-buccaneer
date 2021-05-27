/* 
 * player.c - Handles the client-side of the game. Detects keystrokes and join msg; Displays the visible map for
 *            players and the end game screen
 *
 * usage: ./player hostname port [playername]
 *   where hostname is the IP address that the server's running on
 *   where port is the number which the server listens for messages on
 *   where playername, if provided, the client joins as a player
 *                 if not provided, the client joins as a view-only spectator
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

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */
void displayGrid(const char* grid);
void displayerHeader(int n, int p, int r);
void displayAction(const char* message);
void ensureDimensions(pos2D_t* display_hW);
void clientQuit(char* explanation);

/**************** local functions ****************/
/* not visible outside this file */

/**************** global integer ****************/
static int isSpectator;     // -1 if player, 1 if spectator
static char playerID;       // initialized on recieving OK message for join
                            // Note: playerID not initailized for spectator

/* ***************** main ********************** */
/*
 * What it does: parses args, connects to the server, Send Join message, 
 * starts nCurses, starts NetworkPlayer(MessageLoop).
 */
int
main(const int argc, char *argv[])
{
    
}

/*************** displayHeader() ****************/
/*
 * What it does: Display just the top line header that the serve sends to the client
 * Parameters: n - number of gold left remaining
 *             p - amount of gold that this player has in their purse
 *             r - amount of gold just collected by this player (if applicable)
 *
 * Returns: Nothing
 */
void displayHeader(int n, int p, int r)
{   
    // NOTE: buffer may need to be changed if the message is changed
    //       n, p, r should not exeed 9999
    char* header= mem_malloc_assert((88 * sizeof(char)) , 
            "displayerHeader: out of memory");
    if (isSpectator == -1) {
        sprintf(header, "Spectator: %d nuggets unclaimed.", r);
    } else {
        sprintf(header, "Player %c has %d nuggets (%d nuggets unclaimed).", 
                playerID, p, r);
    }
    if (n != 0) {
        // buffer may need to change if the message is changed
        char* action = mem_malloc_assert((20 * sizeof(char)), 
                "displayerHeader: out of memeory");
        sprintf(action, "Gold received: %d", n);
        displayAction(action);
        free(action);
    }
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

/**************** setPlayerID *****************/
/* 
 * What it does: sets the player's PlayerID from a server OK message
 * Parameters: playerID - the players playerID character
 * Returns: Nothing
 */
void
setPlayerID(char* plID) 
{
    if (plID == '\0') {
        fprintf(stderr, "setPlayerID(): NULL ('\\0') 'playerID' passed\n");
        return;
    }
    playerID = plID[0];
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
    move(1,0);
    int i;
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
    int NROWS;  // the number of rows on the client 
    int NCOLS;  // the number of columns on the client
    int displayW = pos2D_getX(display_hW);      // the number of columns on server
    int displayH = pos2D_getY(display_hW);      // the number of rows on server
    getmaxyx(stdscr, NROWS, NCOLS);
    while (displayW > NCOLS || displayH + 1 > NROWS) {
        char* printS = mem_malloc_assert((130 * sizeof(char)), 
                "ensureDimensions: out of memory");
        sprintf(printS, "adjust screen width and height to fit requirements: \n"
                "Current Width: %d, required: %d\n"
                "Current Height: %d, required: %d", 
                displayW, NROWS, displayH, NCOLS);
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
    }
    return;
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
    // exit Ncurses window
    endwin();
    printf("%s\n", explanation);
    free(explanation);
    exit(0);
}
