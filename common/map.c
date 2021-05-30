
/*
 * map.c
 *
 * impliments the map struct and related functions layed out in map.h
 * see map.h for more information
 *
 * William French
 * Nunchuck-Buccaneers
 * CS50 - spring 2021
 * 05/21/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "pos2D.h"
#include "grid.h"
#include "mem.h"
#include "playerInfo.h"

/* ******************************************************** */

/* ********** map Struct ********* */
typedef struct map {
    grid_t* baseGrid;
    grid_t* gameGrid;
} map_t;

/* ******************************************************** */

/* ********** map_new() ********** */
/* see map.h for description */
map_t* map_new(char* mapFile)
{
    // NULL pointer check
    if (mapFile == NULL) {
        fprintf(stderr, "map_new(): NULL 'mapFile' passed\n");
        return NULL;
    }

    // open the file, read it's contents, close it
    FILE* fp;
    if ( (fp = fopen(mapFile, "r+")) == NULL) {
        fprintf(stderr, "map_new(): could not open %s for writing\n", 
                mapFile);
        return NULL;
    }

    char* map_string = file_readFile(fp);
    fclose(fp);
    // allocate memory for the new struct
    map_t* map = mem_malloc_assert(sizeof(map_t), "map_new(): out of memory\n");

    // create the gameGrid and baseGrid using the map_string
    map->baseGrid = grid_new(map_string);
    map->gameGrid = grid_new(map_string);
    // grid_new() coppies the string, so free our copy
    free(map_string);

    return map;
}

/* ********** map_getBaseGrid ********** */
/* see map.h for description */
grid_t* map_getBaseGrid(map_t* map)
{
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_getBaeGrid(): NULL passed to 'map'\n");
        return NULL;
    }
    return map->baseGrid;
}

/* ********** map_getVisibleMap() ********** */
/* see map.h for description */
grid_t* map_getVisibleMap(map_t* map, grid_t* sightGrid)
{
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_getVisibleMap(): passed NULL pointer 'map'\n");
        return NULL;
    }
    if (sightGrid == NULL) {
        fprintf(stderr, "map_getVisibleMap(): passed NULL pointer 'sightGrid'\n");
        return NULL;
    }
    
    // Check that sightGrid and map->gameGrid are the same dimensions
    pos2D_t* wH_gameGrid = grid_getWidthheight(map->gameGrid);
    pos2D_t* wH_sightGrid = grid_getWidthheight(sightGrid);
    int width;
    int width2;
    if ((width = pos2D_getX(wH_gameGrid)) != (width2 = pos2D_getX(wH_sightGrid))) {
        fprintf(stderr, "map_getVisibleMap(): grids width missmatch, "
                "gameGrid: %d, sightGrid %d\n", width, width2);
        pos2D_delete(wH_gameGrid);
        pos2D_delete(wH_sightGrid);
        return NULL;
    }
    int height;
    int height2;
    if ((height = pos2D_getY(wH_gameGrid)) != (height2 = pos2D_getY(wH_sightGrid))) {
        fprintf(stderr, "map_getVisibleMap(): grids height missmatch, "
                " gameGrid: %d, sightGrid %d\n", height, height2);
        pos2D_delete(wH_gameGrid);
        pos2D_delete(wH_sightGrid);
        return NULL;
    }
    pos2D_delete(wH_gameGrid);
    pos2D_delete(wH_sightGrid);
    // take the map grids as strings, start an outGrid
    char* gameString = grid_toString(map->gameGrid);
    char* baseString = grid_toString(map->baseGrid);
    char* sightString = grid_toString(sightGrid);
    int buffer = ((height * (width +1)) + 1) * sizeof(char);
    char* outString = mem_malloc_assert(buffer, "map_getVisibleMap(): "
            "out of memeory\n");

    // loop through the index of the gridStrings
    int i = 0;
    while (sightString[i] != '\0') {
        // assign char to outString[i] based on sightSring[i]
        if (sightString[i] == '\n') {
            outString[i] = '\n';
        } else if (sightString[i] == '0') {
            outString[i] = ' ';
        } else if (sightString[i] == '1') {
            outString[i] = baseString[i];
        } else if (sightString[i] == '2') {
            outString[i] = gameString[i];
        } else {
            fprintf(stderr, "map_getVisibleMap(): sightGrid contains unexpected "
                    "character '%c'\n", sightString[i]);
            outString[i] = sightString[i];
        }
        i++;
    }
    outString[i] = '\0';

    // free the strings used to make outString
    free(gameString);
    free(baseString);
    free(sightString);
    
    // create the new outGrid and return it
    grid_t* outGrid = grid_new(outString);
    free(outString);
    return outGrid;
}

/* ********** map_getGamePos() ********** */
/* see map.h for decription */
char map_getGamePos(map_t* map, pos2D_t* pos)
{
    if (map == NULL) {
        fprintf(stderr, "map_getGamePos(): HULL 'map' passed");
        return '\0';
    }
    char c = grid_getPos(map->gameGrid, pos);
    return c;
}

/* ********** map_setPlayerPos() ********** */
/* see map.h for description */
void map_setPlayerPos(map_t* map, pos2D_t* pos, playerInfo_t* player)
{
    // NULL pointer checks
    if (map == NULL) {
        fprintf(stderr, "map_setPlayerPos(): NULL pointer 'map'\n");
        return;
    }
    if (pos == NULL) {
        fprintf(stderr, "map_setPlayerPos(): NULL pointer 'pos'\n");
        return;
    }
    if (player == NULL) {
        fprintf(stderr, "map_setPlayerPos(): NULL pointer 'player'\n");
        return;
    }
    char ID = (player->playerID) + 65;
    // Check for NULL pointers in player->ID
    if (ID == '\0') {
        fprintf(stderr, "map_setPlayerPos(): NULL pointer 'player->playerID'\n");
    return;
    }
    
    // add the player to the new position on the gameGrid
    grid_setPos(map->gameGrid, pos, ID);
    // add the new position to the players struct (save the old position)
    pos2D_t* old_pos = pos2D_new(pos2D_getX(player->pos), pos2D_getY(player->pos)); 
    pos2D_set(player->pos, pos2D_getX(pos), pos2D_getY(pos));

    // UNLESS the player has only just entered the game
    // UNLESS this player is being swapped with another player
    // reset the old_pos char on gameGrid to the baseGrid char
    if (pos2D_getX(old_pos) != pos2D_getX(pos) || pos2D_getY(old_pos) != pos2D_getY(pos)) {
        // the char from the spot the player came from
        char old_char = grid_getPos(map->gameGrid, old_pos);
        // if the old char is this player's ID
        if (old_char == ID) {
            // reset the old spot on the grameGrid to the baseGrid char
            char spot_baseGrid = grid_getPos(map->baseGrid, old_pos);
            grid_setPos(map->gameGrid, old_pos, spot_baseGrid);
        }
            // if it is not, then a player has taken their spot
            // so we don't over write them
    }
    pos2D_delete(old_pos);
    return;
}

/* *********** map_clearSpot() ********** */
/* see map.h for description */
void map_clearSpot(map_t* map, pos2D_t* pos)
{
    if (map == NULL) {
        fprintf(stderr, "map_clearSpot(): NULL 'map' passed\n");
        return;
    }
    if (pos == NULL) {
        fprintf(stderr, "map_clearSpot(): NULL 'pos' passed\n");
        return;
    }
    char c = grid_getPos(map->baseGrid, pos);
    if (c == '\0') {
        fprintf(stderr, "map_clearSpot(): grid_getPos returned null\n");
        return;
    }
    grid_setPos(map->gameGrid, pos, c);
    return;
}

/* ********** map_randomEmptySquare() ********** */
/* see map.h for description */
pos2D_t* map_randomEmptySquare(map_t* map)
{   
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_randomEmptySquare(): NULL 'map' passed\n");
        return NULL;
    }

    char* gameString = grid_toString(map->gameGrid);
    // strlen + 1 so that % gridLength includes the last (not \0) character
    int gridLength = strlen(gameString) + 1;
    
    // select random spots in the grid until an empty one is found
    int randIndex = (rand() % gridLength);
    char c;
    while ((c = gameString[randIndex]) != '.') {
        randIndex = (rand() % gridLength);
    }

    // turn the index into a pos2D
    pos2D_t* widthHeight = grid_getWidthheight(map->gameGrid);

    // get the gridWidth, calculate the x,y coordinates of the point
    int gridWidth = pos2D_getX(widthHeight);
    int y = (int)floor(randIndex/(gridWidth+1));
    int x = randIndex - ((gridWidth + 1) * (y));

    // return a pos2D of the point
    pos2D_t* randPos = pos2D_new(x, y);
    free(gameString);
    pos2D_delete(widthHeight);
    return randPos;
}

/* ********** map_putOneGold() ********* */
/* see map.h for description */
void map_putOneGold(map_t* map) 
{
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_putOneGold(): NULL 'map' passed\n");
        return;
    }
    
    // get the pos of a random empty square
    pos2D_t* randPos = map_randomEmptySquare(map);

    // set the char of that index on the game Map to the gold symbol '*'
    grid_setPos(map->gameGrid, randPos, '*');
    pos2D_delete(randPos);
    return;
}

/* ********* map_getWidthheight() ********** */
/* see map.h for description */
pos2D_t* map_getWidthheight(map_t* map) 
{
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_getWidthheight(): NULL 'map' passed\n");
        return NULL;
    }

    return grid_getWidthheight(map->baseGrid);
}

/* ********** map_delete() ********** */
/* see map.h for description */
void map_delete(map_t* map)
{
    // NULL pointer check
    if (map == NULL) {
        fprintf(stderr, "map_delete(): NULL 'map' passed\n");
        return;
    }
    grid_delete(map->baseGrid);
    grid_delete(map->gameGrid);
    free(map);
    return;
}
