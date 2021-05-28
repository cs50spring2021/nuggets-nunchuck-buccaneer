/* 
 * client.c - Handles the client-side of the game. Detects keystrokes and join msg; Displays the visible map for
 *            players and the end game screen
 *
 * usage: ./client hostname port [playername]
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
#include "network.h"
#include <unistd.h>

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

/**************** global types ****************/

/**************** global function prototypes ****************/
/* that is, visible outside this file */
void displayGrid(const char* grid);
void displayerHeader(int n, int p, int r);
void displayAction(const char* message);
void ensureDimensions(pos2D_t* display_hW);
void clientQuit(char* explanation);

void testSetPlayerID(char id);

/**************** local function prototypes ****************/
/* not visible outside this file */
void clearHeader(void);

/**************** global integer ****************/
static int isSpectator;     // -1 if player, 1 if spectator
static char playerID;       // initialized on recieving OK message for join
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
    int y;
    int x;
    int maxX = getmaxx(stdscr); // the width of the screen
    move(0, maxX - actionLen);
    x = maxX - actionLen;
    while (x <= maxX) {
        addch(' ');
        move(y,x+1);
        getyx(stdscr, y, x);
    }
}

/******************************************************************************/
/************************ Global functions *************************/
#ifndef TESTING
/* ***************** main ********************** */
/*
 * What it does: parses args, connects to the server, Send Join message, 
 * starts nCurses, starts NetworkPlayer(MessageLoop).
 */
int
main(const int argc, char *argv[])
{
  if (argc != 3 || argc != 4) {
    fprintf(stderr, "main(): inccoret number of args passed; %d\n", argc);
    exit(1);
  }

  // NOTE: Do we need to check that hostname and port are valid?
  char* hostname = argv[1]; 
  char* port = argv[2];
  char* playerName = NULL;
  if (argc == 4) {
    isSpectator = -1;
    playerName = argv[3];
  } else {
    isSpectator = 1;
  }

  //initialize ncurses
  initscr();
  noecho();

  // I believe that this initializes the background and writing colors
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  attron(COLOR_PAIR(1));
    
  //Start the network 
  // We pass 'stderr' to errorFile because we will redirect log messages to 
  // stderr when calling them, then redirect stderr to a file in the command line
  startNetworkClient(hostname, port, stderr, playerName);
    
  // startNetworkClient exits after it recieves a 'QUIT' message
  exit(0);
}
#endif
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
    //       n, p, r should not exeed 999
    char* header= mem_malloc_assert((headerLen * sizeof(char)) , 
            "displayerHeader: out of memory");
    if (isSpectator == -1) {
        sprintf(header, "Spectator: %d nuggets unclaimed.", r);
    } else {
        sprintf(header, "Player %c has %d nuggets (%d nuggets unclaimed).", 
                playerID, p, r);
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

/**************** setPlayerID *****************/
/* 
 * What it does: sets the player's PlayerID from a server OK message
 * Parameters: playerID - the players playerID character
 * Returns: Nothing
 */
void
setPlayerID(char* plID) 
{
    if (plID == NULL) {
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

    // if displayW is less than the width required to print header + action
    // set displayW to the width of header + action + 1
    int minTotalTopWidth = headerLen + actionLen + 1;
    if (displayW < minTotalTopWidth) {
        displayW = minTotalTopWidth;
    }

    getmaxyx(stdscr, NROWS, NCOLS);
    while (displayW > NCOLS || displayH + 1 > NROWS) {
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
        sleep(1);
        getmaxyx(stdscr, NROWS, NCOLS);
    }
    
    // clear the ensureDimensions message
    move(0,0);
    int i = 0;
    int y;
    int x;
    while (printS[i] != '\0') {
        getyx(stdscr, y, x);
        if (printS[i] == '\n'){
            move(y + 1, 0);
        } else {
            addch(' ');
            move(y, x +1);
        }
        i++;
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
}


void testSetPlayerID(char id){
    playerID = id;
}
