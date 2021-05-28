#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "pos2D.h"
#include "mem.h"
#include "network.h"
void displayGrid(const char* grid);
void displayHeader(int n, int p, int r, char ID);
void displayAction(const char* message);
void ensureDimensions(pos2D_t* display_hW);
void clientQuit(char* explanation);
void clearHeader(void);
