/*

    visibility.c - CS 50 'visibility' module
    See visibility.h for more info.

    Nunchuck-Buccaneers
    CS50, 5/22/2021

*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pos2D.h"
#include "grid.h"
#include "mem.h"

/**************** file-local global variables ****************/

/**************** local types ****************/

/**************** global types ****************/

/**************** global functions ****************/

/**************** local functions ****************/

static bool checkLineVerticles(bool flipPosXY, pos2D_t* start, pos2D_t* end, grid_t* baseGrid);

/**************** functions ****************/

/******************* visibility_getVisibility *********************
Module that provides a function to check if one point is visible from another given a base grid.
Caller Provides:
    A start position to check
    An end position to check
    A pointer to a map to use for the sight lines
We Return:
    True if the square is visible:
    False if square is not visible or there is an error
*/
bool visibility_getVisibility(pos2D_t* start, pos2D_t* end, grid_t* baseGrid){
    // Check Args
    if(start == NULL || end == NULL || baseGrid == NULL){
        fprintf(stderr, "visibility_getVisibility: Null args passed\n");
        return false;
    }
    //Check In bounds
    pos2D_t* gridHW = grid_getWidthheight(baseGrid);
    if(pos2D_getX(start) < 0 || pos2D_getX(start) >= pos2D_getX(gridHW)){
        pos2D_delete(gridHW);
        fprintf(stderr, "visibility_getVisibility: Start position out of bounds\n");
        return false;
    }
    if(pos2D_getY(start) < 0 || pos2D_getY(start) >= pos2D_getY(gridHW)){
        pos2D_delete(gridHW);
        fprintf(stderr, "visibility_getVisibility: Start position out of bounds\n");
        return false;
    }
    if(pos2D_getX(end) < 0 || pos2D_getX(end) >= pos2D_getX(gridHW)){
        pos2D_delete(gridHW);
        fprintf(stderr, "visibility_getVisibility: End position out of bounds\n");
        return false;
    }
    if(pos2D_getY(end) < 0 || pos2D_getY(end) >= pos2D_getY(gridHW)){
        pos2D_delete(gridHW);
        fprintf(stderr, "visibility_getVisibility: End position out of bounds\n");
        return false;
    }
    pos2D_delete(gridHW);
    // Get reversed points for X and Y axis
    pos2D_t* startReversed = mem_assert(pos2D_new(pos2D_getY(start), pos2D_getX(start)), "visibility_getVisibility: Mem pos2D");
    pos2D_t* endReversed = mem_assert(pos2D_new(pos2D_getY(end), pos2D_getX(end)), "visibility_getVisibility: Mem pos2D");
    // Check the verticle axis for both the original points and the reversed axis points
    //If both are visible then point is visible
    bool visible = (checkLineVerticles(false, start, end, baseGrid));
    visible = visible && checkLineVerticles(true, startReversed, endReversed, baseGrid);
    // Delete reversed Points
    pos2D_delete(startReversed);
    pos2D_delete(endReversed);
    //Return visibility
    return visible;
}


/******************* checkLineVerticles *********************
Checks line of sight for  the verticle walls between two points it can be flipped to check the horizontal
Caller Provides:
    A start position to check
    An end position to check
    A pointer to a grid to use for the sight lines
    A bool on whether to flip the axises for newly made points
We Return:
    True if the square is visible for those vert lines
    False if square is not visible or there is an error
*/
static bool checkLineVerticles(bool flipPosXY, pos2D_t* start, pos2D_t* end, grid_t* baseGrid){
    // X and y are interchangeable but did everything in terms of unflipped
    // Find the slope of the line
    int changeX = pos2D_getX(end) -  pos2D_getX(start);
    int changeY = pos2D_getY(end) -  pos2D_getY(start);
    double slope = ((double)changeY) / ((double)changeX);
    //Check if directly above
    if(changeX == 0){
        return true;
    }
    //Skip the origin pos
    int xDiff = 0; 
    if(changeX > 0){
        xDiff++;
    } else {
        xDiff--;
    }
/* 
  STYLE: isn't the following expression the same as saying
    while (xDiff != changeX) ?
  that is far more readable.
 */
    //loop towards end x from start x until 1 away from end
    while(abs(xDiff - changeX) >= 1){
        //Find the Y of the line for that x
        double lineY = ((double)pos2D_getY(start)) + ((double)xDiff) * slope;
        //Check if hits an exact square
        if(round(lineY) == lineY){
            //Create a new point to hold the pos of intersection
            pos2D_t* point = NULL;
            //Flip the point depending on flip bool
            if(!flipPosXY){
                point = mem_assert(pos2D_new(pos2D_getX(start) + xDiff, (int)lineY), "visibility_getVisibility: Mem pos2D");
            } else {
                point = mem_assert(pos2D_new((int)lineY, pos2D_getX(start) + xDiff), "visibility_getVisibility: Mem pos2D");
            }
            //Get the char for the point
            char found = grid_getPos(baseGrid, point);
            // Delete the point
            pos2D_delete(point);
            // If found is not room tile block sight
            if(found != '.'){
                return false;
            }
        //Passes between two squares
        } else {
            //Create new pos for the spots above and below the y for that x
            pos2D_t* overPoint = NULL;
            pos2D_t* underPoint = NULL;
            //Flip the points depending on flip bool
            if(!flipPosXY){
                overPoint = mem_assert(pos2D_new(pos2D_getX(start) + xDiff, round(lineY + 0.5f)), "visibility_getVisibility: Mem pos2D");
                underPoint = mem_assert(pos2D_new(pos2D_getX(start) + xDiff, round(lineY - 0.5f)), "visibility_getVisibility: Mem pos2D");
            } else {
                overPoint = mem_assert(pos2D_new(round(lineY + 0.5f), pos2D_getX(start) + xDiff), "visibility_getVisibility: Mem pos2D");
                underPoint = mem_assert(pos2D_new(round(lineY - 0.5f), pos2D_getX(start) + xDiff), "visibility_getVisibility: Mem pos2D");
            }
            //Get characters for the over and under pos
            char over = grid_getPos(baseGrid, overPoint);
            char under = grid_getPos(baseGrid, underPoint);
            //Delete points
            pos2D_delete(overPoint);
            pos2D_delete(underPoint);
            // if both points are blocked block sight
            if(over != '.' && under != '.'){
                return false;
            }
        }
        //Move to next x pos
        if(changeX > 0){
            xDiff++;
        } else {
            xDiff--;
        }
    }
    // Sight is clear!
    return true;
}
