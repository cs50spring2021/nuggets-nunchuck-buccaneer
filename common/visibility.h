/*

    visibility.h
    Provides a function to check whether two spots on a map can see each other

	Nunchuck-Buccaneers
    CS50, 5/20/2021

*/
/******************* visibility_getVisibility *********************
Module that provides a function to check if one point is visible from another given a map.
Caller Provides:
    A start position to check
    An end position to check
    A pointer to a map to use for the sight lines
We Return:
    True if the square is visible:
    False if square is not visible or there is an error
*/
bool visibility_getVisibility(pos2D_t* start, pos2D_t* end, map_t* map);
