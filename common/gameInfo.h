/*
 * gameInfo.h - header file for the gameInfo module
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
#include "string.h"

#ifndef __GAMEINFO_H
#define __GAMEINFO_H

/*********************** local types **********************/
typedef struct playerInfo {
    grid_t* sightGrid;
    pos2D_t* pos;
    int score;
    int playerID;
    addr_t* address;
    char* username;
} playerInfo_t;

/********************** global types **********************/
typedef struct gameInfo gameInfo_t;

/*********************** global functions ************************/
/****************** gameInfo_newGameInfo ******************/
/*
 * Creates a new gameInfo struct (piles of gold, score, and mapFile).
 * Caller provides:
 *     piles of gold remaining in the game,
 *     total score remaining in the game,
 *     the mapFile .txt file passed in to display
 * We return:
 *     pointer to a gameInfo struct,
 *     NULL if an error occurred while creating the pointer.
 * Caller is responsible for:
 *     later calling gameInfo_delete.
 */
gameInfo_t* gameInfo_newGameInfo(int piles, int score, char* mapFile);

/******************* gameInfo_add Player ******************/
/*
 * Adds a player to the gameInfo player array
 * Caller provides:
 *     desired gameInfo struct,
 *     player address,
 *     player coordinates (pos2D).
 * We return:
 *     true if added,
 *     false if player limit already reached.
 * Caller is responsible for:
 *     later calling gameInfo_removePlayer.
 */
bool gameInfo_addPlayer(gameInfo_t* info, addr_t* address, pos2D_t* pos);

/****************** gameInfo_addSpectator *****************/
/*
 * Adds a specator to the gameInfo player array
 * Caller provides:
 *     desired gameInfo struct,
 *     spectator address.
 * We return:
 *     nothing,
 *     exit non-zero if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
void gameInfo_addSpectator(gameInfo_t* info, addr_t* address);

/****************** gameInfo_removePlayer *****************/
/*
 * Removes a player from the gameInfo player array
 * Caller provides:
 *     desired gameInfo struct,
 *     player address.
 * We return:
 *     nothing,
 *     exit non-zero if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
void gameInfo_removePlayer(gameInfo_t* info, addr_t* address);

/******************* gameInfo_getPlayer *******************/
/*
 * Gets player info from the players array
 * Caller provides:
 *     desired gameInfo struct,
 *     player address.
 * We return:
 *     playerInfo struct pointer,
 *     NULL if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
playerInfo_t* gameInfo_getPlayer(gameInfo_t* info, addr_t* address);

/****************** gameInfo_pickupGold *******************/
/*
 * Adds gold amount to the parameterized players address
 * Caller provides:
 *     desired gameInfo struct,
 *     player address.
 * We return:
 *     true if added,
 *     false if any errors.
 * Caller is responsible for:
 *     nothing.
 */
bool gameInfo_pickupGold(gameInfo_t* info, addr_t* address);

/*************** gameInfo_createScoreBoard ****************/
/*
 * Creates a scoreboard for the game
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     char* representing the scoreboard,
 *     NULL if errors encountered.
 * Caller is responsible for:
 *     nothing.
 */
char* gameInfo_createScoreBoard(gameInfo_t* info);

/******************** gameInfo_topBar *********************/
/*
 * grabs the topBar for a player from the players array
 * upon any changes to their playerInfo/gamestate
 * Caller provides:
 *     desired gameInfo struct,
 *     player address.
 * We return:
 *     char* representing player topbar,
 *     NULL if errors encountered.
 * Caller is responsible for:
 *     nothing.
 */
char* gameInfo_topBar(gameInfo_t* info, addr_t* address);

/******************** gameInfo_getMap *********************/
/*
 * Grabs the map stored in the struct.
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     map_t* pointer,
 *     NULL if error.
 * Caller is responsible for:
 *     nothing.
 */
map_t* gameInfo_getMap(gameInfo_t* info);

/********************* gameInfo_getSeed *******************/
/*
 * Generates a random seed for random behaviors in game functions
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     int for random seed.
 * Caller is responsible for:
 *     nothing.
 */
int gameInfo_getSeed(gameInfo_t* info);

/********************* gameInfo_delete ********************/
/*
 * Adds a player to the gameInfo player array
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     nothing,
 *     exit non-zero if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
void gameInfo_delete(gameInfo_t* info);

#endif // __GAMEINFO_H
