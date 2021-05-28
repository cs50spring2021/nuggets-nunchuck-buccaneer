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
    //noecho();
    // I believe that this initializes the background and writing colors
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("CLIENT TEST");
    attroff(COLOR_PAIR(1));
    displayHeader(0, 0, 0);
    //display("|.......|\n|.......|");
    //quitClient("End of test");
}
