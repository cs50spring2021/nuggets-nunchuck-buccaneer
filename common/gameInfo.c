/*
 * gameInfo.c - implementation for the `gameInfo` module
 * 
 * *gameInfo* stores info held in the game
 * 
 * *playerInfo* nested struct stores info about the players
 * held in the gameInfo player array
 * 
 * Nunchuck Buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mem.h"
#include "pos2D.h"
#include "grid.h"
#include "map.h"
#include "message.h"
#include "visibility.h"
#include "string.h"

/*********************** local types **********************/
/*
typedef struct playerInfo {
    grid_t* sightGrid;
    pos2D_t* pos;
    int score;
    int playerID;
    addr_t* address;
    char* username;
} playerInfo_t;
*/

/********************** global types **********************/
typedef struct gameInfo {
    playerInfo_t** players;
    int goldPiles;
    int goldScore;
    int numPlayers;
    map_t* map;
} gameInfo_t;

/*********************** global functions ************************/
/****************** gameInfo_newGameInfo ******************/
/* see gameInfo.h for description */
gameInfo_t* 
gameInfo_newGameInfo(int piles, int score, char* mapFile)
{
    if (piles < 0 || score < 0 || mapFile == NULL) {
        fprintf(stderr, "gameInfo_newGameInfo: invalid/NULL input\n");
        return NULL;
    }

    gameInfo_t* gameInfo = mem_malloc_assert(sizeof(gameInfo_t), "gameInfo_newGameInfo: memory allocation error\n");
    gameInfo->players = mem_calloc_assert(26, sizeof(playerInfo_t), "gameInfo_newGameInfo: memory allocation error\n");
    gameInfo->goldPiles = piles;
    gameInfo->goldScore = score;
    gameInfo->numPlayers = 0;
    gameInfo->map = map_new(mapFile);

    return gameInfo;
}

/******************* gameInfo_addPlayer *******************/
/* see gameInfo.h for description */
bool
gameInfo_addPlayer(gameInfo_t* info, addr_t* address, pos2D_t* pos, char* username)
{
    // arg checking
    if (info == NULL || address == NULL || pos == NULL || username == NULL) {
        fprintf(stderr, "gameInfo_addPlayer: invalid/NULL input\n");
        return false;
    }

    // create a new player
    playerInfo_t* player = mem_malloc_assert(sizeof(playerInfo_t), "gameInfo_addPlayer: memory allocation error\n");

    // add all information
    player->pos = pos;
    player->score = 0;
    player->address = address;
    player->username = username;

    // handle for number of players
    if (info->numPlayers < 25) player->playerID = info->numPlayers + 1;
    else return false;

    // create the players initial sightGrid
    char* mapString = grid_toString(map_getBaseGrid(gameInfo_getMap(info)));
    char* sightGridString_init = mem_malloc_assert(strlen(mapString) + 1, "gameInfo_addPlayer: memory allocation error\n");

    // create an empty sightGrid string
    char currSpot;
    for (int i = 0; i < strlen(mapString); i++) {
        currSpot = mapString[i];
        if (currSpot == '\n') sightGridString_init[i] = '\n'; // new line
        else if (currSpot == '\0') sightGridString_init[i] = '\0'; // end of the string
        else sightGridString_init[i] = '0'; // empty spot
    }
    
    // create the empty sightGrid
    grid_t* sightGridNew = grid_new(sightGridString_init);
    player->sightGrid = sightGridNew;
    gameInfo_updateSightGrid(info, player->address);

    // update gameInfo struct
    info->players[info->numPlayers] = player;
    info->numPlayers++;

    // player created!
    return true;
}

/****************** gameInfo_addSpectator *****************/
/* see gameInfo.h for description */
void 
gameInfo_addSpectator(gameInfo_t* info, addr_t* address)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_addSpectator: NULL/invalid gameInfo pointer or address pointer\n");
        exit (1);
    }

    // insert a new player with null information into the players array
    playerInfo_t* spectator = mem_malloc_assert(sizeof(playerInfo_t), "memory allocation error\n");
    spectator->sightGrid = NULL;
    spectator->pos = NULL;
    spectator->score = 0;
    spectator->playerID = 25;
    spectator->address = address;
    spectator->username = "spectator";

    info->players[25] = spectator;
}

/****************** gameInfo_removePlayer *****************/
/* see gameInfo.h for description */
void 
gameInfo_removePlayer(gameInfo_t* info, addr_t* address)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_removePlayer: NULL/invalid gameInfo pointer or address pointer\n");
        exit (1);
    }

    // remove player from the list, decrement numPlayers, and free all info
    playerInfo_t* player = gameInfo_getPlayer(info, address);

    // remove player from list and change gameInfo
    info->players[player->playerID] = NULL;
    info->numPlayers--;

    // free from memory
    grid_delete(player->sightGrid);
    pos2D_delete(player->pos);
    mem_free(player);
}

/******************* gameInfo_getPlayer *******************/
/* see gameInfo.h for description */
playerInfo_t* 
gameInfo_getPlayer(gameInfo_t* info, addr_t* address)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_getPlayer: NULL gameInfo pointer or address pointer\n");
        return NULL;
    }

    // return player info for given address
    playerInfo_t* player = NULL;
    int i = 0;
    while (player == NULL && i < info->numPlayers) {
        if (message_eqAddr(*address, *(info->players[i]->address))) {
            player = info->players[i];
        }
        i++;
    }

    // see if player was assigned
    if (player == NULL) {
        fprintf(stderr, "gameInfo_getPlayer: player does not exist!\n");
        return NULL;
    }

    // return player
    return player;
}

/****************** gameInfo_pickupGold *******************/
/* see gameInfo.h for description */
bool 
gameInfo_pickupGold(gameInfo_t* info, addr_t* address)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_pickupGold: NULL gameInfo pointer or address pointer\n");
        return false;
    }

    /* 
     * grab a random amount of gold: 
     *     add to player score
     *     decrement goldPiles and goldScore
     */
    // seed & goldAmt
    int goldAmt = rand() % info->goldScore;

    // make changes
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    player->score += goldAmt;
    info->goldPiles--;
    info->goldScore -= goldAmt;

    // gold added!
    return true;
}

/*************** gameInfo_createScoreBoard ****************/
/* see gameInfo.h for description */
char* 
gameInfo_createScoreBoard(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_createScoreBoard: NULL gameInfo pointer\n");
        return NULL;
    }

    /*
     * create scoreboard for the game:
     *     sort all of the players based on score
     *     print 1....n players with score
     */
    playerInfo_t* scoreboard[info->numPlayers];
    for (int i = 0; i < info->numPlayers; i++) {
        scoreboard[i] = info->players[i];
    }

    // sort and create a string
    // qsort(scoreboard, info->numPlayers, sizeof(playerInfo_t), scoreboardCmpFunc);
    char scoreboardLine[info->numPlayers * 50]; // 50 chars for each line

    // print out players in decreasing order to a string
    sprintf(scoreboardLine, "GAME OVER: \n");
    for (int i = 0; i < info->numPlayers; i++) {
        sprintf(scoreboardLine, "%c \t%d \t%s\n", i+65, scoreboard[i]->score, scoreboard[i]->username);
    }

    return scoreboardLine;
}

/******************* scoreboardCmpFunc ********************/
/* comparator for scoreboard qsort */
int
scoreboardCmpFunc(const void* player1, const void* player2)
{
    int player1Score = (int) player1;
    int player2Score = (int) player2;

    return player1Score - player2Score;
}

/******************** gameInfo_topBar *********************/
/* see gameInfo.h for description */
char* 
gameInfo_topBar(gameInfo_t* info, addr_t* address, char* message)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_topBar: NULL gameInfo or address pointer\n");
        return NULL;
    }

    // set message to an empty string 
    if (message == NULL) {
        message = " ";
    }
    // grab the top bar for the player and print out to a char*
    char playerTopLine[100];
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    sprintf(playerTopLine, "Player %c has %d nuggets (%d nuggets unclaimed). %s", player->playerID+65, player->score, info->goldScore, message); // need to update to handle messages
    
    return playerTopLine;
}

/******************** gameInfo_getMap *********************/
/* see gameInfo.h for description */
map_t*
gameInfo_getMap(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gmaeInfo_getMap: NULL gameInfo pointer\n");
        return NULL;
    }

    // return the game map
    return info->map;
}

/**************** gameInfo_updateSightGrid ****************/
/* see gameInfo.h for description */
bool 
gameInfo_updateSightGrid(gameInfo_t* info, addr_t* address)
{
    // arg checking
    if (info == NULL || address == NULL) {
        fprintf(stderr, "gameInfo_updateSightGrid: NULL/invalid gameInfo pointer or address pointer\n");
        return false;
    }

    // grab the player
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    /* 
     * update the player's sightGrid
     * 
     * go through from their current position and set values to 2 
     * if seeable, otherwise set to 1 if previously a 2 and change 
     * nothing otherwise.
     */

    // mapString
    char* gridString = grid_toString(player->sightGrid);

    // loop to set all non '\n' chars to 0, 1, or 2
    char currSpot;
    int height = 0;
    int i = 0;
    while (i < strlen(gridString)) {
        // grab the currSpot char as well as x and y (height)
        currSpot = gridString[i];
        pos2D_t* otherPos = pos2D_new((i / height) + (i % height), height);

        // increment height if encounters '\n'
        if (currSpot == '\n') {
            height++;
        }
        else {
            /*
             * check visibility 
             *     if '1' and not visible, keep at '1'
             *     if '2' and not visible, switch to '1'
             *     if visible, switch to '2'
             */
            if (visibility_getVisibility(player->pos, otherPos, player->sightGrid) && currSpot != '2') {
                grid_setPos(player->sightGrid, otherPos, '2');
            }
            else if (!visibility_getVisibility(player->pos, otherPos, player->sightGrid) && (currSpot == '1' || currSpot == '0')) {
                continue;
            } else if (!visibility_getVisibility(player->pos, otherPos, player->sightGrid) && currSpot == '2') {
                grid_setPos(player->sightGrid, otherPos, '1');
            }
        }
        i++;
    }

    // successfully updated!
    return true;
}

/********************* gameInfo_delete ********************/
/* see gameInfo.h for description */
void
gameInfo_delete(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_delete: NULL gameInfo pointer\n");
        exit (1);
    }

    /*
     * struct attributes:
     *  playerInfo_t** players;
     *  int goldPiles;
     *  int goldScore;
     *  int numPlayers;
     *  map_t* map;
     */

    // free memory for all players and players array
    for (int i = 0; i < info->numPlayers; i++) {
        gameInfo_removePlayer(info, info->players[i]->address);
    }
    mem_free(info->players);

    // free map memory and gameInfo memory
    map_delete(info->map);
    mem_free(info);
}