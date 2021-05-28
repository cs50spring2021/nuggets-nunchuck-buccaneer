/* 
 * clientCmds.h - header files for clientCmds.c, holds prototypes for the functions in
 *            clientCmds.c so they can be called in network
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
/* ********** prototypes for functions in clientCmds.c *********** */
/* for function descriptions see paragraph comments in clientCmds.c */
void display(const char* grid);
void displayHeader(int goldCollected, int goldInPurse, int goldRemaining, char ID);
void displayAction(const char* message);
void ensureDimensions(pos2D_t* display_hW);
void quitClient(char* explanation);
void clearHeader(void);