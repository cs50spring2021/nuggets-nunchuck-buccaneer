/*
 * gridtest.c - test program for grid.c struct and functions
 *
 * this program provides a unit test for the grid struct and related functions
 *
 * Nunchuck-Buccaneers
 * 05/21/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pos2D.h"
#include "grid.h"
#include "file.h"
#include "visibility.h"

static void testSpot(pos2D_t* player, FILE* map, grid_t* grid);

int main(){
    printf("Visibility Test\n");
    FILE* map = fopen("../maps/hole.txt", "r");
    grid_t* grid = grid_new(file_readUntil(map, NULL));
    printf("Open Floor Test\n");
    pos2D_t* player = pos2D_new(13,13);
    testSpot(player, map, grid);
    printf("Corridor Exit Test\n");
    pos2D_set(player,39,16);
    testSpot(player, map, grid);
    printf("Corridor Test\n");
    pos2D_set(player,43,16);
    testSpot(player, map, grid);
    fclose(map);
    pos2D_delete(player);
}


static void testSpot(pos2D_t* player, FILE* map, grid_t* grid){
    rewind(map);
    grid_t* sight = grid_new(file_readUntil(map, NULL));
    grid_setPos(sight, player, '@');
    pos2D_t* gridWH = grid_getWidthheight(sight);
    for(int y = 0; y < pos2D_getY(gridWH); y++){
        for(int x = 0; x < pos2D_getX(gridWH); x++){
            pos2D_t* spot = pos2D_new(x, y);
            if(!visibility_getVisibility(player, spot, grid)){
                grid_setPos(sight, spot, 'O');
            }
            pos2D_delete(spot);
        }
    }
    printf(grid_toString(sight));
}