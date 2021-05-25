
/*

    grid.h
    Struct to hold a map of characters that can be
    interacted with 2d coordinates to get and set. Has toString and parseFromString

	Nunchuck-Buccaneers
    CS50, 5/19/2021

*/

#include "pos2D.h"

#ifndef __GRID_H
#define __GRID_H

typedef struct grid grid_t;

/******************* grid_new *********************
Builds a new grid from a string of characters
Caller Provides:
    A string that is a 2D grid of characters, formatted thusly "abcd\nefgh"
We Do:
    Not save the given string but instead copy it
We Return:
    NULL on any error
    A filled grid of characters
Caller is Responsible for:
    Later calling grid_delete
*/
grid_t* grid_new(char* readString);

/******************* grid_delete *********************
Deletes a given grid and frees everything within it
Caller Provides:
    A grid struct to delete
We Do:
    Free the grid and it's contents, if grid provided is NULL we do nothing
*/
void grid_delete(grid_t* grid);


/******************* grid_getPos *********************
Gets the value of a particular grid spot but gives all wall chars as '+'
Caller Provides:
    A grid struct to look into
    A pos to look into that grid struct
We Return:
    Null (0) on out of bounds or error otherwise we return the character found in 
        the grid
    All wall characters should be returned as '+'
    Just a char primitive
*/
char grid_getPos(grid_t* grid, pos2D_t* pos);

/******************* grid_setPos *********************
Sets a particular spot on a grid to a given char
Caller Provides:
    A grid struct to set into
    A pos to set in that grid struct
    A char to set in that spot
We Do:
    Nothing if there is any error
    Otherwise we replace the old character in that grid spot with the received character
*/
void grid_setPos(grid_t* grid, pos2D_t* pos, char value);

/******************* grid_toString *********************
Returns a string representing the grid
Caller Provides:
    A grid struct to get the string for
We Return:
    Null on any error
    Otherwise a char* string that is formatted thusly "abcd\nefgh"
    char* is malloced
Caller is responsible For:
    Later freeing the returned string
*/
char* grid_toString(grid_t* grid);

/******************* grid_getWidthheight *********************
Get the grid width and height
Caller Provides:
    A grid struct to get the widthHeight for
We Return:
    Null on any error
    A pos2D with x as width and y as height
Caller is responsible For:
    Later freeing the returned pos2D
*/
pos2D_t* grid_getWidthheight(grid_t* grid);

#endif // __GRID_H
