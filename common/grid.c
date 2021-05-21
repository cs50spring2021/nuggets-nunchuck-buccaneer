
/* 
 * grid.c
 * impliments the grid struct and related functions layed out in grid.h
 * see grid.h for more information
 * 
 * NOTE: 0,0 is defined as the upper left corner, height is minimal at the top.
 *       So the top row is height 0, the second to top row is height 1, etc.
 * 
 * Nunchuck-Buccaneers
 * CS50, 05/21/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pos2D.h"
//#include "mem.h"

/* ********** grid Struct ********** */
typedef struct grid{
    char* map;
    int height;
    int width;
} grid_t;

/* ********** grid_new() ********** */
/* detailed function description provided in grid.h */
grid_t* grid_new(char* readString)
{   
    // NULL pointer check
    if (readString == NULL) {
        fprintf(stderr, "grid_new(): null string passed\n");
        return NULL;
    }

    // create a copy of the passed map string
    char* map = mem_malloc_assert(sizeof(readString), "grid_new(): "
            "out of memory\n");
    strcpy(map, readString);
    
    // loop through every character of the map
    // the width is the number of chars before the first new line
    // the height is equal to the number of new line characters
    int i = 0;
    int width = 0;
    int height = 0;
    while (map[i] != 0) {
        if (map[i] == '\n') {
            if (height == 0) {
                width = i;
            }
            height++;
        }
        i++;
    } 

    // assign the values to the grid
    grid_t* grid = mem_malloc_assert(sizeof(grid_t), "grid_new(): "
            "out of memory\n");
    grid->map = map;
    grid->width = width;
    grid->height = height;
    return grid;
}

/* ********** grid_delete() ********** */
/* detailed function description provided in grid.h */
void grid_delete(grid_t* grid)
{
    // NULL pointer checking
    if (grid == NULL) {
        fprintf(stderr, "grid_delete(): null pointer passed\n");
        return;
    }
    if (grid->map == NULL) {
        fprintf(stderr, "grid_delete(): grid->map is null\n");
        return;
    }

    // free the contents then the grid
    free(grid->map);
    free(grid);
    return;
}

/* ********** grid_getPos() ********** */
/* detailed function description provided in grid.h */
char grid_getPos(grid_t* grid, pos2D_t* pos)
{
    // NULL pointer checking
    if (grid == NULL) {
        fprintf(stderr, "grid_getPos(): passed NULL grid\n");
        return 0;
    }
    if (pos == NULL) {
        fprintf(stderr, "grid_getPos(): passed NULL pos\n");
        return 0;
    }

    // get pos variables and check that they are valid
    int x;
    int y;
    if ((x = pos2D_getX(pos)) < 0 || (y = pos2D_getY(pos)) < 0) {
        fprintf(stderr, "grid_getPos(): pos has invalid contents\n");
        return 0;
    }

    // check that the point is in bounds
    if (x < 0 || x >= grid->width) {
        fprintf(stderr, "grid_getPos(): x coordinate %d  out of range. The "
                "width is %d\n", x, grid->width);
        return 0;
    } 
    if (y < 0 || y >= grid->height) {
        fprintf(stderr, "grid_getPos(): y coordinate %d  out of range. The  "
                "height is %d\n", y, grid->height);
        return 0;
    } 

    // return the char of the requested position
    int i = ( ((grid->width + 1) * y) + x);
    char ch = grid->map[i];
    // standardize wall chars
    if (ch == '-' || ch =='|') {
        ch = '+'; 
    }
    return ch;
}


/* ********** grid_setPos() ********** */
/* detailed function description provided in grid.h */
void grid_setPos(grid_t* grid, pos2D_t* pos, char value)
{
    // NULL pointer check
    if (grid == NULL) {
        fprintf(stderr, "grid_setPos(): NULL pointer grid passed\n");
        return;
    }
    if (pos == NULL) {
        fprintf(stderr, "grid_setPos(): NULL pointer pos passed\n");
        return;
    }
    if (value == '\0') {
        fprintf(stderr, "grid_setPos(): NULL (\\0) value passed\n");
        return;
    }

    // get pos variables and check that they are valid
    int x;
    int y;
    if ((x = pos2D_getX(pos)) < 0 || (y = pos2D_getY(pos)) < 0) {
        fprintf(stderr, "grid_getPos(): pos has invalid contents\n");
        return 0;
    }

    // check that the point is in bounds
    if (x < 0 || x >= grid->width) {
        fprintf(stderr, "grid_getPos(): x coordinate %d  out of range. The "
                "width is %d\n", x, grid->width);
        return 0;
    } 
    if (y < 0 || y >= grid->height) {
        fprintf(stderr, "grid_getPos(): y coordinate %d  out of range. The  "
                "height is %d\n", y, grid->height);
        return 0;
    } 

    // set the char of the provided position to the provided value
    int i = ( ((grid->width + 1) * y) + x);
    grid->map[i] = value;
    return;
}



/* ********** grid_toString() ********** */
/* detailed function description provided in grid.h */
char* grid_toString(grid_t* grid)
{
    // NULL pointer check
    if (grid == NULL) {
        fprintf(stderr, "grid_setPos(): NULL pointer grid passed\n");
        return;
    }
    char* map_cpy = mem_malloc_assert(

}



/* ********** grid_getWidthheight() ********** */
/* detailed function description provided in grid.h */
pos2D_t* grid_getWidthheight(grid_t* grid)
{
    // NULL pointer check
    if (grid == NULL) {
        fprintf(stderr, "grid_getWidthheight(): NULL pointer passed\n");
        return NULL;
    }

    // create the pos2D to be returned
    pos2D_t* widthheight = pos2D_new(grid->width, grid->height);
    if (widthheight == NULL) {
        fprintf(stderr, "grid_getWidthheight(): failedto make the pos2D\n");
        return NULL;
    }
    return widthheight;
}

