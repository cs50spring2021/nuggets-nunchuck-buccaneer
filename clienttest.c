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
#include <stdbool.h>
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
    int c;
    int stage = 0;
    bool end = false;
    while((c = getch()) != 'x' && !end){
        if(c = 'c'){
            switch(stage){
                case (0):
                    ensureDimensions(pos2D_new(30,15));
                    displayHeader(0, 0, 0);
                    displayAction("Unknown Keystroke");
                    break;
                case (1):
                    displayHeader(5, 10, 15);
                    break;
                case (2):
                    display("|.......|\n|.......|");
                    break;
                default:
                    end = true;
            }
            stage++;
        }
        c = 'n';
    }
    quitClient("End of test");
    printf("ENDED");
}
