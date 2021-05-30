/* 
 * serverCmds.h - header files for serverCmds.c, holds prototypes for the functions in
 *            serverCmds.c so they can be called in network
 * 
 * Nunchuck Buccaneers
 * cs50 - Spring 2021
 * 05/28/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "pos2D.h"
#include "mem.h"
#include "network.h"
#include "gameInfo.h"



/* ********** prototypes for functions in serverCmds.c *********** */
/* for function descriptions see paragraph comments in serverCmds.c */

bool movePlayer(gameInfo_t* gameinfo, addr_t addr, char input);
void joinUser(gameInfo_t* gameinfo, addr_t player, char* playerName);
bool leaveUser(gameInfo_t* gameinfo, addr_t player);
void endGame(gameInfo_t* gameinfo);
gameInfo_t* initializeGame(char* mapFile);
