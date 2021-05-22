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



int main(){
    printf("Visibility Test\n");
    FILE* map = fopen("../maps/hole.txt", "r");
    grid_t* grid = grid_new(file_readUntil(map, NULL));
    rewind(map);
    grid_t* sight = grid_new(file_readUntil(map, NULL));
    fclose(map);

    pos2D_t* player = pos2D_new(13,13);
    grid_setPos(sight, player, 'X');
    int xd = 14;
    int yd = 10;
    pos2D_t* spotd = pos2D_new(xd, yd);
    if(!visibility_getVisibility(player, spotd, grid)){
        grid_setPos(sight, spotd, 'Z');
    } else {
        grid_setPos(sight, spotd, '%');
    }
    pos2D_t* gridWH = grid_getWidthheight(grid);
    for(int y = 0; y < pos2D_getY(gridWH); y++){
        for(int x = 0; x < pos2D_getX(gridWH); x++){
            //printf("%d, %d\n", x, y);
            pos2D_t* spot = pos2D_new(x, y);
            if(!visibility_getVisibility(player, spot, grid)){
                grid_setPos(sight, spot, 'O');
            }
            pos2D_delete(spot);
        }
    }
    printf(grid_toString(sight));
    pos2D_delete(player);
}