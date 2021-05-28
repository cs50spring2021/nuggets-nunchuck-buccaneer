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
#include "file.h"
#include "grid.h"



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
    // Load the Hole map
    FILE* gridFile = fopen("./maps/hole.txt", "r");
    char* readMap = file_readUntil(gridFile, NULL);
    grid_t* holeGrid = grid_new(readMap);
    free(readMap);
    fclose(gridFile);
    //Load the challenge map
    gridFile = fopen("./maps/challenge.txt", "r");
    readMap = file_readUntil(gridFile, NULL);
    grid_t* challengeGrid = grid_new(readMap);
    free(readMap);
    fclose(gridFile);
    //Start tests
    testSetPlayerID('B');
    int c;
    int stage = 0;
    while((c = getch()) != 'x'){
        if(c == 'c'){
            if(stage == 0){
                pos2D_t* dim = pos2D_new(30,15);
                ensureDimensions(dim);
                pos2D_delete(dim);
                displayHeader(0, 0, 0);
                displayAction("Unknown Keystroke");
            }
            if(stage == 1){
                pos2D_t* hw = grid_getWidthheight(holeGrid);
                ensureDimensions(hw);
                pos2D_delete(hw);
                displayHeader(5, 10, 15);
            }
            if(stage == 2){
                char* gridDisplay = grid_toString(holeGrid);
                display(gridDisplay);
                free(gridDisplay);
            }
            if(stage == 3){
                pos2D_t* hw = grid_getWidthheight(challengeGrid);
                ensureDimensions(hw);
                pos2D_delete(hw);
                displayHeader(0, -1, 15);
                char* gridDisplay = grid_toString(challengeGrid);
                display(gridDisplay);
                free(gridDisplay);
            }
            if(stage == 4){
                break;
            }
            stage++;
        }
        c = 'n';
    }
    quitClient("End of test");
    printf("ENDED");
}
