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
#include "clientCmds.h"
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
    int c;
    int stage = 0;
    while((c = getch()) != 'x'){
        if(c == 'c'){
            if(stage == 0){
                fprintf(stderr, "\nBasic Ensure Dimensions Test\n");
                pos2D_t* dim = pos2D_new(30,15);
                ensureDimensions(dim);
                fprintf(stderr, "\nNegative Ensure Dimensions Test\n");
                pos2D_set(dim, -1, -4);
                ensureDimensions(dim);
                pos2D_delete(dim);
                fprintf(stderr, "\nNULL Ensure Dimensions Test\n");
                ensureDimensions(NULL);
                fprintf(stderr, "\nBasic Header Test\n");
                displayHeader(0, 0, 0, 'B');
                fprintf(stderr, "\nBasic Action Test\n");
                displayAction("Unknown Keystroke");
                fprintf(stderr, "\nNULL Action Test\n");
                displayAction(NULL);
            }
            if(stage == 1){
                fprintf(stderr, "\nSecond Basic Ensure Test\n");
                pos2D_t* hw = grid_getWidthheight(holeGrid);
                ensureDimensions(hw);
                pos2D_delete(hw);
                fprintf(stderr, "\nDisplay Header Test 2\n");
                displayHeader(5, 10, 15, 'A');
            }
            if(stage == 2){
                fprintf(stderr, "\nBasic Display Test\n");
                char* gridDisplay = grid_toString(holeGrid);
                display(gridDisplay);
                free(gridDisplay);
            }
            if(stage == 3){
                fprintf(stderr, "\nBasic Third Ensure Test\n");
                pos2D_t* hw = grid_getWidthheight(challengeGrid);
                ensureDimensions(hw);
                pos2D_delete(hw);
                fprintf(stderr, "\nSpectator Header Test 2\n");
                displayHeader(0, -1, 15, 'C');
                fprintf(stderr, "\nBasic Change Display Test\n");
                char* gridDisplay = grid_toString(challengeGrid);
                display(gridDisplay);
                free(gridDisplay);
            }
            if(stage == 4){
                fprintf(stderr, "\nNULL Display Test\n");
                display(NULL);
            }
            if(stage == 5){
                break;
            }
            stage++;
        }
        c = 'n';
    }
    fprintf(stderr, "\nQuitClient NULL Test\n");
    quitClient(NULL);
    fprintf(stderr, "\nQuitClient Test\n");
    quitClient("End of test");
    printf("ENDED");
}
