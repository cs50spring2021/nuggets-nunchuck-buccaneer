/* 
 * player.c - sends keystrokes and join msg; Displays the visible map for players
 *            and the end game screen
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

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */
void display(const char* grid);
void joinFail(pos2D_t* display_heightWidth);

/**************** local functions ****************/
/* not visible outside this file */


/* ***************** main ********************** */
/*
 * parses args, connects to the server, Send Join message, starts nCurses, 
 * starts NetworkPlayer(MessageLoop)
 */
int
main(const int argc, char *argv[])
{

}

/**************** display()  ****************/
/*
 * Display the grid (the map) that the server sends to the client
 * Parameters: grid - string representation of the map that the client can read
 * Returns: Nothing
 */
void display(const char* grid)
{

}


/**************** joinFail() ****************/
/*
 * This function is called when a user fails to join. It sends a join message
 * after resizing
 * Parameters: display_heightWidth - the height and width of the grid's display
 * Returns: Nothing
 */
void joinFail(pos2D_t* display_heightWidth)
{

}

