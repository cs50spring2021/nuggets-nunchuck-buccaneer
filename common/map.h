/*   header file for map.c module
 *
 * map holds the two primary grids, baseGrid and gameGrid, and defines functions
 * for interacting with them
 *
 * William French
 * nunchuck-buccaneers
 * 05/19/21
 */

//#ifndef __MAP_H
//#define __MAP_H

#include <stdio.h>
#include "grid.h"
#include "playerInfo.h"


/* ***************************************************************** */

/* ********* Struct: Map ********** */
/* holds the base-gird and game-grid
 */
typedef struct map map_t;


/* ***************************************************************** */

/* ********** map_new() ********** */
/* initialize a new index
 * 
 * Caller provides:
 *      - the path to a file containing a properly formatted map.txt file
 *
 * We return:
 *      - NULL on error
 *      - a pointer to a newly minted map_t, with instatiated base and game grids
 *
 * Caller is responsible for:
 *      - later calling map_delete on the returned map pointer
 */
map_t* map_new(char* mapFile);


/* ********** map_getBaseGrid(map_t* map) ********** */
/* getter for the held basegrid
 * 
 * Caller provides:
 *      - a valid pointer to a map
 * 
 * We return:
 *      - NULL on error
 *      - a pointer to the held baseGrid
 */
grid_t* map_getBaseGrid(map_t* map);


/* ********** map_getVisibleMap() ********** */
/* combine the gameGrid and a sightGrid, return a grid of what is visible
 *
 * Caller provides:
 *      - a valid pointer to an initialized map
 *      - a valid pointer to a sightGrid
 *      - sightGrid and map->gameGrid must have the same dimensions
 *
 * We return:
 *      - NULL on error
 *      - a pointer to a grid of what is see-able on the gameGrid
 * 
 * Caller is responsible for:
 *      - later calling grid_delete on the returned grid_t pointer
 */
grid_t* map_getVisibleMap(map_t* map, grid_t* sightGrid);

/* ********** map_getGamePos() ********** */
/* return the character in gameGrid at a specific position
 * 
 * Caller provides:
 *      - NULL char '\0' on error
 *      - a valid pointer to a map_t
 *      - a valid pointer to a pos2D_t
 * 
 * We return:
 *      - the char at that pos
 */
char map_getGamePos(map_t* map, pos2D_t* pos);

/* ********** map_setPlayerPos() ********** */
/* sets the position of a player to the new position on gameGrid
 *  
 * Caller provides:
 *      - a valid pointer to a map_t
 *      - a valid pointer to a pos2D_t
 *      - a valid pointer to the a playerInfo_t object
 *
 * We return:
 *      - nothing
 * 
 * We do:
 *      - nothing if error
 *      - set the charcter in gameGrid at pos to the playerID
 *      - update the pos2D in the playerInfo Struct
 *          - and free the old pos2D of the player
 *      - if the player was on the map previoulsy, set the char where the 
 *        player used to be back to what it is on the baseGrid
 *          - UNLESS the players current spot is occupied by a player other
 *            than themselves. (so when players swap positions they do not write
 *            over the first one to move)
 *
 * NOTE: the caller is responsible for making sure that the player doesn't
 *       run through any walls 
 */
void map_setPlayerPos(map_t* map, pos2D_t* pos, playerInfo_t* player);


/* ********** map_clearSpot() ********** */
/* replaces a pos in gameGrid with the char in that pos from baseGrid
 *
 * Caller provides:
 *      - a valid pointer to a mpa
 *      - a valid pointer to a pos
 * 
 * We return:
 *      - nothin
 * 
 * We do:
 *      - set the gameGrid pos position to the char at that pos from baseGrid
 */
void map_clearSpot(map_t* map, pos2D_t* pos);

/* ********** map_randomEmptySquare() ********** */
/* return a random empty square from the map
 * note: an empty square means a square in the gameGrid that has a '.'
 *
 * Caller provides:
 *      - a valid pointer to a map_t
 *      - a valid int seed, to seed random numbers
 *
 * We return:
 *      - NULL on error
 *      - a pointer to a pos2D_t of an slot that is currently empty '.'
 * 
 * Caller is responsible for:
 *      - later freeing the returned pos2D_t
 */
pos2D_t* map_randomEmptySquare(map_t* map);
 
/* ********** map_putOneGold() ********** */
/* place a single pile of gold on an empty spot on the map
 * 
 * Caller provides:
 *      - a valid pointer to a map_t
 *      - a valid int seed, to seed random numbers
 * 
 * We return:
 *      - nothing
 * 
 * We do:
 *      - change one random open slot of the gameGrid to a gold pile '*'
 */
void map_putOneGold(map_t* map);

/* ********** map_getWidthHeight() ********** */
/* get the width and hight of the baseGrid (same as gameGrid)
 * 
 * Caller provides:
 *      - a valid pointer to a map_t
 * 
 * We return:
 *      - NULL on error
 *      - a pointer to a pos2D_t which contains the width in the x-coordinate and
 *        the height in the y-coordinate
 * Caller is responsible for:
 *      - later caling pos2D_delete() on the returned pos
 */
pos2D_t* map_getWidthheight(map_t* map);

/* ********** map_delete() ********** */
/* delete a map_t 
 *
 * Caller provides:
 *      - a valid pointer to a map_t
 * 
 * We return:
 *      - nothing
 *
 * We do:
 *      - delete both grids held by map
 *      - free the map
 */
void map_delete(map_t* map);

//#endif // __MAP_H
