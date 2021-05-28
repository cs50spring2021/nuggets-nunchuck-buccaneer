/*
 * clienttest.c - test program for client.c
 *
 * Nunchuck-Buccaneers
 * 05/24/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "client.h"

int main(){
    fprintf(stderr, "CLIENT TEST\n");
    //initialize ncurses
    initscr();
    noecho();
    // I believe that this initializes the background and writing colors
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("CLIENT TEST");
    testSetPlayerID('B');
    ensureDimensions(pos2D_new(30,20));
    displayHeader(0, 0, 0);
    //display("|.......|\n|.......|");
    int c;
    while((c = getch()) != 'x'){

    }
    quitClient("End of test");
    printf("ENDED");
}
