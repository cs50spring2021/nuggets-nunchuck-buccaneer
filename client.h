#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "pos2D.h"
#include "mem.h"
#include "network.h"
void display(const char* grid);
void displayHeader(int goldCollected, int goldInPurse, int goldRemaining);
void displayAction(char* action);
void ensureDimensions(pos2D_t* display_hW);
void quitClient(char* message);
void testSetPlayerID(int id);
