/* 
 * gameInfotest.c - test program for `gameInfo` module
 *
 * Nunchuck-buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "pos2D.h"
#include "message.h"
#include "gameInfo.h"
#include "playerInfo.h"
#include "map.h"
#include "grid.h"

/************** main **************/
int
main(int argc, char* argv[])
{
    gameInfo_t* info = gameInfo_newGameInfo(100, 500, "../maps/big.txt");
    
    // initialize player/spectator addresses
    addr_t address1 = message_noAddr();
    message_setAddr("localhost", "4242", &address1);
    pos2D_t* pos1 = pos2D_new(50, 50);

    addr_t address2 = message_noAddr();
    message_setAddr("localhost", "4242", &address2);
    pos2D_t* pos2 = pos2D_new(60, 60);

    addr_t address3 = message_noAddr();
    message_setAddr("localhost", "4242", &address3);

    gameInfo_addPlayer(info, &address1, pos1, "testplayer1");    
    playerInfo_t* player1 = gameInfo_getPlayer(info, &address1);
    printf("%s added!\n", player1->username);
   
    playerInfo_t* player2 = gameInfo_getPlayer(info, &address2);
    gameInfo_addPlayer(info, &address2, pos2, "testplayer2");
    printf("%s added!\n", player2->username);

    playerInfo_t* spectator = gameInfo_getPlayer(info, &address3);
    gameInfo_addSpectator(info, &address3);
    printf("%s added!\n", spectator->username);

    while (player1->score + player2->score < 500) {
        char* topBar;
        int prevAmt;
        int i = rand() % 2;
        if (i == 0) {
            prevAmt = player1->score;
            gameInfo_pickupGold(info, &address1);
            char message[40];
            sprintf(message, "GOLD recieved testplayer1: %d\n", player1->score - prevAmt);
            topBar = gameInfo_topBar(info, &address1, message);
        } else {
            prevAmt = player2->score;
            gameInfo_pickupGold(info, &address2);
            char message[40];
            sprintf(message, "GOLD recieved testplayer1: %d\n", player2->score - prevAmt);
            topBar = gameInfo_topBar(info, &address2, message);
        }
        printf("%s", topBar);
        mem_free(topBar);
    }

    char* scoreboard = gameInfo_createScoreBoard(info);
    printf("%s", scoreboard);

    char* baseGridString = grid_toString(map_getBaseGrid(gameInfo_getMap(info)));
    printf("%s", baseGridString);

    pos2D_set(player1->pos, 51, 50);
    pos2D_set(player2->pos, 61, 60);

    gameInfo_updateSightGrid(info, &address1);
    gameInfo_updateSightGrid(info, &address2);

    gameInfo_removePlayer(info, spectator->address);
    gameInfo_delete(info);

    mem_free(scoreboard);
    mem_free(baseGridString);
    return 0; // successful run!
}