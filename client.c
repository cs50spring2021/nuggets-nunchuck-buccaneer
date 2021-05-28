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
void displayerHeader(int n, int p, int r, char ID);
void displayAction(const char* message);
void ensureDimensions(pos2D_t* display_hW);
void clientQuit(char* explanation);

/**************** local function prototypes ****************/
/* not visible outside this file */
void clearHeader(void);

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
    int y;
    int x;
    int maxX;
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

  //initialize ncurses
  initscr();
  noecho();

  // I believe that this initializes the background and writing colors
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(1));
    
  //Start the network 
  // We pass 'stderr' to errorFile because we will redirect log messages to 
  // stderr when calling them, then redirect stderr to a file in the command line
  startNetworkClient(hostname, port, stderr, playerName);
    
  // startNetworkClient exits after it recieves a 'QUIT' message
  exit(0);
}
#endif