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
#include "mem.h"
#include "pos2D.h"
#include "grid.h"
#include "map.h"
#include "message.h"
#include "visibility.h"
#include "playerInfo.h"
#include "string.h"
#include "gameInfo.h"

/********************** global types **********************/
typedef struct gameInfo {
    playerInfo_t** players;
    int goldPiles;
    int goldScore;
    int numPlayers;
    map_t* map;
    int maxPlayers;
} gameInfo_t;

/*********************** global functions ************************/
/****************** gameInfo_newGameInfo ******************/
/* see gameInfo.h for description */
gameInfo_t* 
gameInfo_newGameInfo(int piles, int score, char* mapFile, int maxUsers)
{
    if (piles < 0 || score < 0 || mapFile == NULL) {
        fprintf(stderr, "gameInfo_newGameInfo: invalid/NULL input\n");
        return NULL;
    }

    gameInfo_t* gameInfo = mem_malloc_assert(sizeof(gameInfo_t), "gameInfo_newGameInfo: memory allocation error\n");
    gameInfo->players = mem_calloc_assert(maxUsers, sizeof(playerInfo_t*), "gameInfo_newGameInfo: memory allocation error\n");
    gameInfo->goldPiles = piles;
    gameInfo->goldScore = score;
    gameInfo->numPlayers = 0;
    gameInfo->map = map_new(mapFile);
    gameInfo->maxPlayers = maxUsers;

    return gameInfo;
}

/******************* gameInfo_addPlayer *******************/
/* see gameInfo.h for description */
playerInfo_t*
gameInfo_addPlayer(gameInfo_t* info, const addr_t* address, pos2D_t* pos, char* username)
{
    // arg checking
    if (info == NULL || pos == NULL || username == NULL) {
        fprintf(stderr, "gameInfo_addPlayer: invalid/NULL input\n");
        return NULL;
    }

    // create a new player
    playerInfo_t* player = mem_malloc_assert(sizeof(playerInfo_t), "gameInfo_addPlayer: memory allocation error\n");

    // add all information
    player->pos = pos2D_new(pos2D_getX(pos), pos2D_getY(pos));
    player->score = 0;
    player->address = address;
    char* usernameCpy = mem_malloc_assert(sizeof(char) * (strlen(username) + 1), "Mem: Addplayer Copying Username");
    strcpy(usernameCpy, username);
    player->username = usernameCpy;
    player->sightGrid = NULL;

    // handle for number of players
    if (info->numPlayers < (info->maxPlayers) - 1) {
        // find first empty spot and set playerID to that
        int i = 0;
        while ((info->players)[i] != NULL) {
            i++;
        }
        player->playerID = i;
    }
    else {
        mem_free(player);
        pos2D_delete(pos);
        return false;
    }

    // create the players initial sightGrid
    char* mapString = grid_toString(map_getBaseGrid(gameInfo_getMap(info)));
    char* sightGridString_init = mem_malloc_assert((strlen(mapString) * sizeof(char)) + 1, "gameInfo_addPlayer: memory allocation error\n");
    // create an empty sightGrid string
    char currSpot;
    for (int i = 0; i < strlen(mapString) + 1; i++) {
        currSpot = mapString[i];
        if (currSpot == '\n') sightGridString_init[i] = '\n'; // new line
        else if (currSpot == '\0') sightGridString_init[i] = '\0'; // end of the string
        else sightGridString_init[i] = '0'; // empty spot
    }

    // create the empty sightGrid
    player->sightGrid = grid_new(sightGridString_init);
    mem_free(sightGridString_init);
    mem_free(mapString);
    // update the player's sightGrid based on position
    char* gridString = grid_toString(player->sightGrid);

    // loop to set all non '\n' chars to 0, 1, or 2
    int height = 0;
    int i = 0;
    int x = 0;
    while (i < strlen(gridString)) {
        // grab the currSpot char as well as x and y (height)
        currSpot = gridString[i];
        pos2D_t* otherPos = pos2D_new(x, height);
        // increment height if encounters '\n'
        if (currSpot == '\n') {
            height++;
            x = 0;
        }
        else {
            // check visibility: if visible, switch value from 0 to 2
            if (visibility_getVisibility(player->pos, otherPos, map_getBaseGrid(info->map))) {
                grid_setPos(player->sightGrid, otherPos, '2');
            }
            x++;
        }
        pos2D_delete(otherPos);
        i++;
    }
    // update gameInfo struct
    info->players[player->playerID] = player;
    info->numPlayers++;

    // player created!
    mem_free(gridString);
    return player;
}

/****************** gameInfo_addSpectator *****************/
/* see gameInfo.h for description */
void 
gameInfo_addSpectator(gameInfo_t* info, const addr_t* address)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_addSpectator: NULL/invalid gameInfo pointer or address pointer\n");
        exit (1);
    }

    // insert a new player with null information into the players array
    playerInfo_t* spectator = mem_malloc_assert(sizeof(playerInfo_t), "memory allocation error\n");
    spectator->pos = NULL;
    spectator->score = -1;
    spectator->playerID = (info->maxPlayers) - 1;
    spectator->address = address;
    spectator->username = NULL;

    // create fully visible sightgrid
    // create the spectator's initial sightGrid
    char* mapString = grid_toString(map_getBaseGrid(gameInfo_getMap(info)));
    char* sightGridString_init = mem_malloc_assert(strlen(mapString) + 1, "gameInfo_addSpectator: memory allocation error\n");

    // create an empty sightGrid string
    char currSpot;
    for (int i = 0; i < strlen(mapString) + 1; i++) {
        currSpot = mapString[i];
        if (currSpot == '\n') sightGridString_init[i] = '\n'; // new line
        else if (currSpot == '\0') sightGridString_init[i] = '\0'; // end of the string
        else sightGridString_init[i] = '2'; // empty spot
    }
    
    // create the empty sightGrid
    grid_t* sightGridNew = grid_new(sightGridString_init);
    spectator->sightGrid = sightGridNew;
    mem_free(mapString);
    mem_free(sightGridString_init);
    // check to see if there is already a spectator
    playerInfo_t* oldSpectator = gameInfo_getSpectator(info);
    if(oldSpectator != NULL){
        char msgBuffer[81];
		sprintf(msgBuffer, "QUIT You have been replaced by a new spectator.");
        #ifdef TESTING
        fprintf(stderr, "%s\n", msgBuffer);
        #endif
        message_send(*(oldSpectator->address), msgBuffer);
        gameInfo_removeSpectator(info);
    }
    (info->players)[(info->maxPlayers) - 1] = spectator;
}

/****************** gameInfo_removePlayer *****************/
/* see gameInfo.h for description */
void 
gameInfo_removePlayer(gameInfo_t* info, const addr_t* address)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_removePlayer: NULL/invalid gameInfo pointer or address pointer\n");
        exit (1);
    }

    // remove player from the list, decrement numPlayers, and free all info
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    // remove player from list and change gameInfo
    int playerID = player->playerID;
    // free from memory

    grid_delete(player->sightGrid);
    //Check if spectator
    if(player->username != NULL){
        pos2D_delete(player->pos);
        mem_free(player->username);
    }
    mem_free(player);

    info->players[playerID] = NULL;
    info->numPlayers--;
}

/****************** gameInfo_removeSpectator *****************/
/* see gameInfo.h for description */
void 
gameInfo_removeSpectator(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "NULL gameInfo pointer\n");
        exit (1);
    }

    if (gameInfo_getSpectator(info) != NULL) {
        playerInfo_t* spectator = gameInfo_getSpectator(info);
        grid_delete(spectator->sightGrid);
        mem_free(spectator);
        (info->players)[(info->maxPlayers) - 1] = NULL;
    }
}

/******************* gameInfo_getPlayer *******************/
/* see gameInfo.h for description */
playerInfo_t* 
gameInfo_getPlayer(gameInfo_t* info, const addr_t* address)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getPlayer: NULL gameInfo pointer or address pointer\n");
        return NULL;
    }

    // return player info for given address
    int i = 0;
    while (i < info->maxPlayers) {
        if((info->players)[i] != NULL){
            fprintf(stderr, "RAN\n");
            #ifndef TESTING
            if (message_eqAddr(*address, *((info->players)[i]->address))) {
            #endif
                playerInfo_t* player = (info->players)[i];
                return player;
            #ifndef TESTING
            }
            #endif
        }
        i++;
    }

    fprintf(stderr, "gameInfo_getPlayer: player does not exist!\n");
    return NULL;
}

/******************* gameInfo_getSpectator *******************/
/* see gameInfo.h for description */
playerInfo_t*
gameInfo_getSpectator(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getSpectator: NULL gameInfo pointer\n");
        return NULL;
    }

    return (info->players)[(info->maxPlayers) - 1];
}

/******************* gameInfo_getPlayerFromID *******************/
/* see gameInfo.h for description */
playerInfo_t* 
gameInfo_getPlayerFromID(gameInfo_t* info, int playerID)
{
    // arg checking
    if (info == NULL || playerID < 0) {
        fprintf(stderr, "gameInfo_getPlayerFromID: NULL gameInfo pointer or playerID < 0\n");
        return NULL;
    }

    // search the players array and find the player with the given playerID
    for (int i = 0; i < info->maxPlayers; i++){
        if ((info->players)[i] != NULL){
            if (playerID == (info->players)[i]->playerID) {
                playerInfo_t* player = (info->players)[i];
                return player;
            }
        }
    }
    return NULL;
}

/****************** gameInfo_pickupGold *******************/
/* see gameInfo.h for description */
int
gameInfo_pickupGold(gameInfo_t* info, const addr_t* address)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_pickupGold: NULL gameInfo pointer or address pointer\n");
        return -1;
    }

    /* 
     * grab a random amount of gold: 
     *     add to player score
     *     decrement goldPiles and goldScore
     */
    // seed & goldAmt
    int goldAmt;
    if (info->goldScore > (info->maxPlayers) - 1 && info->goldPiles > 1) {
        int avgGoldScore = info->goldScore % info->goldPiles;
        goldAmt = rand() % 60 + avgGoldScore;
    } 
    else {
        goldAmt = info->goldScore;
    }

    // make changes
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    player->score += goldAmt;
    info->goldPiles--;
    info->goldScore -= goldAmt;

    // gold added!
    return goldAmt;
}

/*************************** swap *****************************/
/* helper for bubble sort method */
static void swap(playerInfo_t* xp, playerInfo_t* yp)
{
    playerInfo_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/************************** sortFunc ***************************/
/* bubble sort with using counterset for swap value lookups */
static void
sortFunc(playerInfo_t** players, int length)
{
   for (int i = 0; i < length - 1; i++)     
    for (int j = 0; j < length-i-1; j++) {
        playerInfo_t* player1 = players[j];
        playerInfo_t* player2 = players[j+1];
        if (player1->score < player2->score) {
            swap(player1, player2);
        }
    }
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
    int addedPlayers = 0;
    playerInfo_t* scoreboard[info->numPlayers];
    for (int i = 0; i < info->maxPlayers; i++) {
        if(info->players[i] != NULL){
            if(info->players[i]->username != NULL){
                scoreboard[addedPlayers] = info->players[i];
                addedPlayers++;
            }
        }
    }

    // sort and create a string
    sortFunc(scoreboard, info->numPlayers);
    char* scoreboardLine = mem_calloc_assert(info->numPlayers, 50, "memory allocation error\n"); // 50 chars for each line

    // print out players in decreasing order to a string
    sprintf(scoreboardLine, "GAME OVER: \n");
    for (int i = 0; i < info->numPlayers; i++) {
        char* playerLine = mem_malloc_assert(50, "memory allocation error\n");
        sprintf(playerLine, "%c \t%d \t%s\n", scoreboard[i]->playerID+65, scoreboard[i]->score, scoreboard[i]->username);
        strcat(scoreboardLine, playerLine);
        mem_free(playerLine);
    }

    return scoreboardLine;
}

/******************** gameInfo_getScoreRemaining *********************/
/* see gameInfo.h for description */
int 
gameInfo_getScoreRemaining(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getScoreRemaining: NULL gameInfo pointer\n");
        return -1;
    }

    return info->goldScore;
}

/******************** gameInfo_getMap *********************/
/* see gameInfo.h for description */
map_t*
gameInfo_getMap(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getMap: NULL gameInfo pointer\n");
        return NULL;
    }

    // return the game map
    return info->map;
}

/**************** gameInfo_getNumPlayer *****************/
/* see gameInfo.h for description */
int 
gameInfo_getNumPlayers(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getNumPlayers: NULL gameInfo pointer\n");
        return -1;
    }

    return info->numPlayers;
}

/******************* gameInfo_getGoldPiles *******************/
/* see gameInfo.h for description */
int 
gameInfo_getGoldPiles(gameInfo_t* info)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_getGoldPiles: NULL gameInfo pointer\n");
        return -1;
    }

    return info->goldPiles;
}

/**************** gameInfo_updateSightGrid ****************/
/* see gameInfo.h for description */
bool 
gameInfo_updateSightGrid(gameInfo_t* info, const addr_t* address)
{
    // arg checking
    if (info == NULL) {
        fprintf(stderr, "gameInfo_updateSightGrid: NULL/invalid gameInfo pointer or address pointer\n");
        return false;
    }
    /* 
     * update the player's sightGrid
     * 
     * go through from their current position and set values to 2 
     * if seeable, otherwise set to 1 if previously a 2 and change 
     * nothing otherwise.
     */

    // mapString
    playerInfo_t* player = gameInfo_getPlayer(info, address);
    if (player == NULL) {
        fprintf(stderr, "gameInfo_updateSightGrid: player does not exist!\n");
        return false;
    }
    char* gridString = grid_toString(player->sightGrid);

    // loop to set all non '\n' chars to 0, 1, or 2
    char currSpot;
    int height = 0;
    int i = 0;
    int x = 0;
    while (i < strlen(gridString)) {
        // grab the currSpot char as well as x and y (height)
        currSpot = gridString[i];
        pos2D_t* otherPos = pos2D_new(x, height);

        // increment height if encounters '\n'
        if (currSpot == '\n') {
            height++;
            x = 0;
        }
        else {
            /*
             * check visibility 
             *     if '1' and not visible, keep at '1'
             *     if '2' and not visible, switch to '1'
             *     if visible, switch to '2'
             */
            if (visibility_getVisibility(player->pos, otherPos, map_getBaseGrid(info->map)) && currSpot != '2') {
                grid_setPos(player->sightGrid, otherPos, '2');
            }
            else if (!visibility_getVisibility(player->pos, otherPos, map_getBaseGrid(info->map)) && currSpot == '2') {
                grid_setPos(player->sightGrid, otherPos, '1');
            }
            x++;
        }
        pos2D_delete(otherPos);
        i++;
    }

    mem_free(gridString);
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

    // free memory for all players and singular spectator
    for (int i = 0; i < (info->maxPlayers); i++) {
        if(info->players[i] != NULL){
            if(info->players[i]->username != NULL){
                gameInfo_removePlayer(info, info->players[i]->address);
            }
        }
    }
    gameInfo_removeSpectator(info);

    // free the players array
    mem_free(info->players);

    // free map memory and gameInfo memory
    map_delete(info->map);
    mem_free(info);
}
