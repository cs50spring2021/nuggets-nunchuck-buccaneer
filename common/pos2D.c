/* 
 * pos2D.c - implementation for `pos2D.h` header file
 *
 * A struct wrapper that stores an (x,y) point in x and y coordinates. The
 * function also has getX and getY to make access of the coordinates simple.
 * Finally, the function has new and delete function.
 *
 * Nunchuck-buccaneers, May 2021
 */

#include <stdio.h>
#include <stdbool.h>

/************************** struct **************************/
typedef struct pos2D {
    int x;
    int y;
} pos2D_t;

/**************** functions ****************/

/**************** pos2D_new ****************/
/* see pos2D.h for description */
pos2D_t*
pos2D_new(int x, int y)
{
    if (x < 0 || y < 0) {
        fprintf(stderr, "pos2D_new: invalid x or y input -- must be > 0\n");
        return NULL;
    }
    pos2D_t* pos = malloc(sizeof(pos2D_t), "memory allocation error\n");
    pos->x = x;
    pos->y = y;
    return pos;
}

/**************** pos2D_set ****************/
/* see pos2D.h for description */
bool pos2D_set(pos2D_t* pos, int x, int y)
{
    if (pos == NULL || x < 0 || y < 0) {
        fprintf(stderr, "pos2D_set: invalid/NULL pos, x, or y input -- must be non-NULL or > 0\n");
        return false;
    }

    pos->x = x;
    pos->y = y;
    return true;
}

/**************** pos2D_delete ****************/
/* see pos2D.h for description */
void 
pos2D_delete(pos2D_t* pos)
{
    if (pos == NULL) {
        fprintf(stderr, "pos2D_delete: NULL pos input -- must be non-NULL\n");
        exit (1);
    }

    free(pos);
}


/**************** pos2D_getX ****************/
/* see pos2D.h for description */
int 
pos2D_getX(pos2D_t* pos)
{
    if (pos == NULL) {
        fprintf(stderr, "pos2D_getX: NULL pos input -- must be non-NULL\n");
        return -1;
    }

    return pos->x;
}

/**************** pos2D_getY ****************/
/* see pos2D.h for description */
int 
pos2D_getY(pos2D_t* pos)
{
    if (pos == NULL) {
        fprintf(stderr, "pos2D_getY: NULL pos input -- must be non-NULL\n");
        return -1;
    }

    return pos->y;
}