/* 
 * gameInfotest.c - test program for `gameInfo` module
 *
 * Nunchuck-buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    gameInfo_t* info = gameInfo_newGameInfo(10, 500, "../maps/challenge.txt");
    
    // initialize player/spectator addresses
    srand(time(0));

    printf("******************* ADD PLAYER/GET PLAYER TESTING *******************\n");
    /* add player get player testing */
    addr_t address1 = message_noAddr();
    message_setAddr("localhost", "4242", &address1);
    pos2D_t* pos1 = map_randomEmptySquare(gameInfo_getMap(info));

    gameInfo_addPlayer(info, &address1, pos1, "testplayer1");  
    playerInfo_t* player1 = gameInfo_getPlayer(info, &address1);
    printf("%s added!\n", player1->username);
    printf("%s\n", grid_toString(player1->sightGrid));
    printf("%s\n", grid_toString(map_getVisibleMap(gameInfo_getMap(info), player1->sightGrid)));

    addr_t address2 = message_noAddr();
    message_setAddr("localhost", "8080", &address2);
    pos2D_t* pos2 = map_randomEmptySquare(gameInfo_getMap(info));

    gameInfo_addPlayer(info, &address2, pos2, "testplayer2");
    playerInfo_t* player2 = gameInfo_getPlayer(info, &address2);
    printf("%s added!\n", player2->username);
    printf("%s\n", grid_toString(player2->sightGrid));
    printf("%s\n", grid_toString(map_getVisibleMap(gameInfo_getMap(info), player2->sightGrid)));


    printf("******************* ADD SPECTATOR/GET SPECTATOR TESTING *******************\n");
    /* add spectator get spectator testing */
    addr_t address3 = message_noAddr();
    message_setAddr("localhost", "4343", &address3);

    gameInfo_addSpectator(info, &address3);
    playerInfo_t* spectator = gameInfo_getSpectator(info);
    printf("%s added!\n", spectator->username);

    printf("******************** SCORE TESTING **********************\n");
    /* score testing */
    while (gameInfo_getScoreRemaining(info) > 0) {
        printf("GOLD remaining: %d\n", gameInfo_getScoreRemaining(info));
        int prevAmt;
        int i = rand() % 2;
        if (i == 0) {
            prevAmt = player1->score;
            gameInfo_pickupGold(info, player1->address);
            printf("%s picked up %d, score: %d\n", player1->username, player1->score - prevAmt, player1->score);
        } else {
            prevAmt = player2->score;
            gameInfo_pickupGold(info, player2->address);
            printf("%s picked up %d, score: %d\n", player2->username, player2->score - prevAmt, player2->score);
        }
        printf("\n");
    }
    printf("GOLD remaining: %d\n", gameInfo_getScoreRemaining(info));

    printf("*********************** SCOREBOARD TESTING ********************\n");
    char* scoreboard = gameInfo_createScoreBoard(info);
    printf("%s", scoreboard);

    printf("*********************** MAP TESTING *************************\n");
    char* baseGridString = grid_toString(map_getBaseGrid(gameInfo_getMap(info)));
    printf("%s", baseGridString);

    pos2D_t* newPos1 = map_randomEmptySquare(gameInfo_getMap(info));
    pos2D_set(player1->pos, pos2D_getX(newPos1), pos2D_getY(newPos1));

    pos2D_t* newPos2 = map_randomEmptySquare(gameInfo_getMap(info));
    pos2D_set(player2->pos, pos2D_getX(newPos2), pos2D_getY(newPos2));

    gameInfo_updateSightGrid(info, player1->address);
    printf("%s\n", grid_toString(map_getVisibleMap(gameInfo_getMap(info), player1->sightGrid)));
    printf("player1 updated!\n");

    gameInfo_updateSightGrid(info, player2->address);
    printf("%s\n", grid_toString(map_getVisibleMap(gameInfo_getMap(info), player2->sightGrid)));
    printf("player2 updated!\n");

    gameInfo_removeSpectator(info);
    gameInfo_delete(info);
    mem_free(baseGridString);

    printf("Test Passed!\n");
    
    return 0; // successful run!
}