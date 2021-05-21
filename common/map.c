
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
#include "file.h"
#include "pos2D.h"
#include "grid.h"
#include "mem.h"

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
    
    // create a copy of the string
    int buffer = (strlen(readString) + 1) * sizeof(char);
    char* map_string_cpy = mem_malloc_assert(buffer, "map_new(): out of memory\n");
    strcpy(map_string_cpy, map_string);

    // allocate memory for the new struct
    map_t* map = mem_malloc_assert(sizeof(map_t), "map_new(): out of memory\n");

    // assign the original string and the copy to the map grids
    map->baseGrid = map_string;
    map->gameGrid = map_string_cpy;

    return map;
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
    if ((width = pos2D_getX(wH_gameGrid)) != pos2D_getx(wH_gameGrid)) {
        fprintf(stderr, "map_getVisibleMap(): grid width missmatch\n");
        return NULL;
    }
    int height;
    if ((height = pos2D_getY(wH_gameGrid)) != pos2D_getY(wH_gameGrid)) {
        fprintf(stdeer, "map_getVisibleMap(): grid height missmatch\n");
        return NULL;
    }
    
    
    // take the map grids as strings, start an outGrid
    char* sightString = grid_toString(map->gameGrid);
    char* mapString = grid_toString(sightGrid);
    int buffer = ((height * (width +1)) + 1) * sizeof(char);
    char* outGrid = mem_malloc_assert(buffer, "map_getVisibleMap(): "
            "out of memeory\n");

    // loop through the index of the gridStrings
    int i = 0;
    while (sightString[i] != '\0') {
        if (sightString[i] == '\n') {
            outGrid[i] = '\n';
        } else if (sightString[i] == 0){
            outGrid[i] = ' ';
        } else if (
    }

}

