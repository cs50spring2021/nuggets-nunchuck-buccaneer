/*

    visibility.c - CS 50 'visibility' module
    See visibility.h for more info.

    John "James" Utley
    CS50, 5/1/2021

*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pos2D.h"
#include "grid.h"

//NEED TO DO THE TESTING

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
        //fprintf(stderr, "visibility_getVisibility: Null args passed");
        return false;
    }
    // Get reversed points for X and Y axis
    pos2D_t* startReversed = pos2D_new(pos2D_getY(start), pos2D_getX(start));
    pos2D_t* endReversed = pos2D_new(pos2D_getY(end), pos2D_getX(end));
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
    //fprintf(stderr, "cX: %d cY: %d Slope: %f\n", changeX, changeY, slope);
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
    //fprintf(stderr, "X: %d, Y: %d\n", pos2D_getX(end),pos2D_getY(end));
    //loop towards end x from start x until 1 away from end
    while(abs(xDiff - changeX) >= 1){
        //Find the Y of the line for that x
        double lineY = ((double)pos2D_getY(start)) + ((double)xDiff) * slope;
        //fprintf(stderr, "Y: %lf\n", lineY);
        //Check if hits an exact square
        if(round(lineY) == lineY){
            //Create a new point to hold the pos of intersection
            pos2D_t* point = NULL;
            //Flip the point depending on flip bool
            if(!flipPosXY){
                point = pos2D_new(pos2D_getX(start) + xDiff, (int)lineY);
            } else {
                point = pos2D_new((int)lineY, pos2D_getX(start) + xDiff);
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
                overPoint = pos2D_new(pos2D_getX(start) + xDiff, round(lineY + 0.5f));
                underPoint = pos2D_new(pos2D_getX(start) + xDiff, round(lineY - 0.5f));
            } else {
                overPoint = pos2D_new(round(lineY + 0.5f), pos2D_getX(start) + xDiff);
                underPoint = pos2D_new(round(lineY - 0.5f), pos2D_getX(start) + xDiff);
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