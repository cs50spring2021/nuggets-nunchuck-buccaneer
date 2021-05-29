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
#include "playerInfo.h"
#include "string.h"

#ifndef __GAMEINFO_H
#define __GAMEINFO_H

/********************** global types **********************/
typedef struct gameInfo gameInfo_t;

/*********************** global functions ************************/
/****************** gameInfo_newGameInfo ******************/
/*
 * Creates a new gameInfo struct (piles of gold, score, and mapFile).
 * Caller provides:
 *     piles of gold remaining in the game,
 *     total score remaining in the game,
 *     the mapFile .txt file passed in to display,
 *     seed for random functions in the game.
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
bool gameInfo_addPlayer(gameInfo_t* info, const addr_t* address, pos2D_t* pos, char* username);

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
void gameInfo_addSpectator(gameInfo_t* info, const addr_t* address);

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
void gameInfo_removePlayer(gameInfo_t* info, const addr_t* address);

/****************** gameInfo_removeSpectator *****************/
/*
 * Removes a spectator from the gameInfo player array
 * Caller provides:
 *     desired gameInfo struct,
 * We return:
 *     nothing,
 *     exit non-zero if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
 void gameInfo_removeSpectator(gameInfo_t* info);

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
playerInfo_t* gameInfo_getPlayer(gameInfo_t* info, const addr_t* address);

/******************* gameInfo_getSpectator *******************/
/*
 * Gets spectator from the players array == check to see if there is one
 * Caller provides:
 *     desired gameInfo struct,
 * We return:
 *     playerInfo pointer to the spectator slot in players,
 *     NULL if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
playerInfo_t* gameInfo_getSpectator(gameInfo_t* info);

/******************* gameInfo_getPlayerFromID *******************/
/*
 * Gets player info from the players array using playerID
 * Caller provides:
 *     desired gameInfo struct,
 *     playerID.
 * We return:
 *     playerInfo struct pointer,
 *     NULL if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
playerInfo_t* gameInfo_getPlayerFromID(gameInfo_t* info, int playerID);

/****************** gameInfo_pickupGold *******************/
/*
 * Adds gold amount to the parameterized players address
 * Caller provides:
 *     desired gameInfo struct,
 *     player address.
 * We return:
 *     amount of gold collected
 *     -1 if any errors
 * Caller is responsible for:
 *     nothing.
 */
int gameInfo_pickupGold(gameInfo_t* info, const addr_t* address);

/*************** gameInfo_createScoreBoard ****************/
/*
 * Creates a scoreboard for the game
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     char* representing the scoreboard,
 *     NULL if errors encountered.
 * Caller is responsible for:
 *     nothing
 */
char* gameInfo_createScoreBoard(gameInfo_t* info);

/******************** gameInfo_getScoreRemaining *********************/
/*
 * grabs the score remaining
 * Caller provides:
 *     desired gameInfo struct,
 * We return:
 *     int score remaining
 *     NULL if errors encountered.
 * Caller is responsible for:
 *     nothing.
 */
int gameInfo_getScoreRemaining(gameInfo_t* info);

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

/**************** gameInfo_getNumPlayers *****************/
/*
 * Grabs the number of players in the game
 * Caller provides:
 *     desired gameInfo struct pointer.
 * We return:
 *     int stored w number of players.
 *     return -1 if null pointer.
 * Caller is responsible for:
 *     nothing.
 */
int gameInfo_getNumPlayers(gameInfo_t* info);

/**************** gameInfo_updateSightGrid ****************/
/* 
 * Updates sightGrid for the player
 * Caller provides:
 *     gameInfo struct,
 *     address for player.
 * We return:
 *     true if sightGrid is updated,
 *     false if any errors are encountered.
 * Caller is responsible for:
 *     nothing.
 */
bool gameInfo_updateSightGrid(gameInfo_t* info, const addr_t* address);

/******************* gameInfo_getGoldPiles *******************/
/*
 * Getter for gold amount in gameInfo struct
 * Caller provides:
 *     desired gameInfo struct.
 * We return:
 *     gold piles left over
 * Caller is responsible for:
 *     nothing.
 */
int gameInfo_getGoldPiles(gameInfo_t* info);

/********************* gameInfo_delete ********************/
/*
 * Free all things stored in memory
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

