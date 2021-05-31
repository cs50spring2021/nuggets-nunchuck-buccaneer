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
#include "network.h"

typedef struct loopArgs {
    gameInfo_t* gameinfo;
    char* playerID;
} loopArgs_t;

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
    //Setup stuff for loop
    loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "Mem: Test args");
    args->playerID = mem_malloc_assert(sizeof(char), "Mem: player");
    *(args->playerID) = 'B';
    addr_t server = message_noAddr();
	message_setAddr("26346", "122345", &server);

    int c;
    int stage = 0;
    while((c = getch()) != 'x'){
        if(c == 'c'){
            if(stage == 0){
                fprintf(stderr, "\nBasic Ensure Dimensions Test\n");
                handleMessage(args, server, "GRID 30 15");

                //fprintf(stderr, "\nNegative Ensure Dimensions Test\n");
                //handleMessage(args, server, "GRID -1 -4");

                //fprintf(stderr, "\nNULL Ensure Dimensions Test\n");
                //handleMessage(args, server, "GRID");

                fprintf(stderr, "\nBasic Header Test\n");
                handleMessage(args, server, "GOLD 0 0 0");

                fprintf(stderr, "\nBasic Action Test\n");
                displayAction("Unknown Keystroke");

                fprintf(stderr, "\nNULL Action Test\n");
                displayAction(NULL);
            }
            if(stage == 1){
                fprintf(stderr, "\nSecond Basic Ensure Test\n");
                pos2D_t* hw = grid_getWidthheight(holeGrid);
                char* msg = mem_malloc_assert(sizeof(char) * (30), "MEM: msg HW");
                sprintf(msg, "GRID %d %d", pos2D_getX(hw), pos2D_getY(hw));
                handleMessage(args, server, msg);
                mem_free(msg);
                pos2D_delete(hw);

                fprintf(stderr, "\nDisplay Header Test 2\n");
                handleMessage(args, server, "GOLD 5 10 15");
            }
            if(stage == 2){
                fprintf(stderr, "\nBasic Display Test\n");
                char* gridDisplay = grid_toString(holeGrid);
                char* msg = mem_malloc_assert(sizeof(char) * (30 + strlen(gridDisplay)), "MEM: msg HW");
                sprintf(msg, "DISPLAY\n%s", gridDisplay);
                handleMessage(args, server, msg);
                mem_free(msg);
                free(gridDisplay);
            }
            if(stage == 3){
                fprintf(stderr, "\nBasic Third Ensure Test\n");
                pos2D_t* hw = grid_getWidthheight(challengeGrid);
                char* msg = mem_malloc_assert(sizeof(char) * (30), "MEM: msg HW");
                sprintf(msg, "GRID %d %d", pos2D_getX(hw), pos2D_getY(hw));
                handleMessage(args, server, msg);
                mem_free(msg);
                pos2D_delete(hw);
                fprintf(stderr, "\nSpectator Header Test 2\n");
                handleMessage(args, server, "GOLD 0 -1 15");
                fprintf(stderr, "\nBasic Change Display Test\n");
                char* gridDisplay = grid_toString(challengeGrid);
                msg = mem_malloc_assert(sizeof(char) * (30 + strlen(gridDisplay)), "MEM: msg HW");
                sprintf(msg, "DISPLAY\n%s", gridDisplay);
                handleMessage(args, server, msg);
                mem_free(msg);
                free(gridDisplay);
            }
            if(stage == 4){
                fprintf(stderr, "\nNULL Display Test\n");
                handleMessage(args, server, "DISPLAY\n");
            }
            if(stage == 5){
                break;
            }
            stage++;
        }
        c = 'n';
    }
    fprintf(stderr, "\nQuitClient Test\n");
    handleMessage(args, server, "QUIT End of Test");
    printf("ENDED");
}
