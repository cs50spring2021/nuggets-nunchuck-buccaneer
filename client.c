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

/******************************************************************************/
/************************ Global functions *************************/
#ifndef TESTING
static bool str2int(const char string[], int* number);
/* ***************** main ********************** */
/*
 * What it does: parses args, connects to the server, Send Join message, 
 * starts nCurses, starts NetworkPlayer(MessageLoop).
 */
int
main(const int argc, char *argv[])
{
  if (!(argc == 3 || argc == 4)) {
    fprintf(stderr, "main(): incorrect number of args passed; %d\n", argc);
    exit(1);
  }

  // NOTE: Do we need to check that hostname and port are valid?
  char* hostname = argv[1]; 
  char* port = argv[2];
  char* playerName = NULL;
  int portNum = 0;
  str2int(port, &portNum);
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
  startNetworkClient(hostname, &portNum, stderr, playerName);
    
  // startNetworkClient exits after it recieves a 'QUIT' message
  exit(0);
}
#endif

/* ***************** str2int ********************** */
/*
 * Convert a string to an integer, returning that integer.
 * Returns true if successful, or false if any error. 
 * It is an error if there is any additional character beyond the integer.
 * Assumes number is a valid pointer.
 * Borrowed from CS 50 Lecture Notes.
 */
static bool str2int(const char string[], int* number)
{
  char nextchar;
  return (sscanf(string, "%d%c", number, &nextchar) == 1);
}