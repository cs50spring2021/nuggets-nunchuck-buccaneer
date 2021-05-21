/* 
 * pos2D.h - header file for CS50 pos2D module.
 *
 * A struct wrapper that stores an (x,y) point in x and y coordinates. The
 * function also has getX and getY to make access of the coordinates simple.
 * Finally, the function has new and delete function.
 *
 * Nunchuck-buccaneers, May 2021
 */

<<<<<<< HEAD
=======
#ifndef __POS2D_H
#define __POS2D_H

>>>>>>> abcd76e (upaded some headers and what-not)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct pos2D pos2D_t;

/**************** functions ****************/

/**************** pos2D_new ****************/
/* 
 * Creates a new (x,y) point that stores the location provided by the x and y
 * coordinates.
 * Caller provides:
 *   x and y coordinates - The coordinates must be greater than or equal to 0 
 *                         and within the dimensions of the display window and
 *                         within empty spots in the map.
 * We return:
 *   pointer to a the pos2D point
 *   NULL if an error occurred while creating the pointer.
 * Caller is responsible for:
 *   later calling pos2D_delete.
 */
pos2D_t* pos2D_new(int x, int y);

/**************** pos2D_set ****************/
/* Create a new (x,y) position point
 *
 * Caller provides:
 *   x and y values > 0
 * We return:
<<<<<<< HEAD
 *   return true if set; return false if error.
=======
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
>>>>>>> abcd76e (upaded some headers and what-not)
 */
bool pos2D_set(pos2D_t* pos, int x, int y);

/**************** pos2D_delete ****************/
/* Delete the pos2D struct that is provided to this function.
 *
 * Caller provides:
 *   valid pointer to a pos2D struct.
 * We return:
 *   Nothing
 * Notes:
 *   We check whether the pos2D struct provided actually exists, if it doesn't
 *   we just exit out of this function.
 */
void pos2D_delete(pos2D_t* pos);

/**************** pos2D_getX ****************/
/* Gets the x-coordinate for a pointer struct.
 *
 * Caller provides:
 *   valid pointer to a pos2D struct.
 * We return:
 *   int - positive int within the dimensions of the map if the struct was valid
 *         negative int if the struct passed to the function is invalid
 */
int pos2D_getX(pos2D_t* pos);

/**************** pos2D_getY ****************/
/* Gets the y-coordinate for a pointer struct.
 *
 * Caller provides:
 *   valid pointer to a pos2D struct.
 * We return:
 *   int - positive int within the dimensions of the map if the struct was valid
 *         negative int if the struct passed to the function is invalid
 */
int pos2D_getY(pos2D_t* pos);

#endif //__POS2D_H

