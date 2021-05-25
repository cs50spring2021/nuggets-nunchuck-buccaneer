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

int main()
{

    grid_t* grid1 = NULL;
    char* small = 
        "+----------+\n"
        "|..........|\n"
        "|..........|\n"
        "|..........|\n"
        "+----------+\n"
        "            \n";
    
    printf("_______ grid_new() _______\n");
    printf("Null input: \n");
    grid1 = grid_new(NULL);
    if (grid1 != NULL) {
        printf("test fail <<<<<<<\n");
    }
    
    printf("\nProper input 'small.txt': \n");
    grid1 = grid_new(small);
    if (grid1 != NULL) {
        printf("test passed\n");
    }

    printf("\n_______ grid_toString() _______\n");
    printf("Null input: \n");
    char* map = grid_toString(NULL);
    if (map != NULL) {
        printf("test failed <<<<<<<\n");
    }

    printf("\nProper function: \n");
    map = grid_toString(grid1);
    if (strcmp(map, small) == 0) {
        printf("test passed\n");
            free(map);
    } else {
        printf("test failed <<<<<<<\n");
        printf("%s\n", map);
        printf("\n%s\n", small);
    }

    printf("\n_______ grid_getWidthheight() _______\n");
    printf("Null input: \n");
    pos2D_t* pos = grid_getWidthheight(NULL);
    if (pos != NULL) {
        printf("test failed <<<<<<<\n");
        pos2D_delete(pos);
    }
    
    printf("\nProper function: \n");
    pos = grid_getWidthheight(grid1);
    if (pos2D_getX(pos) == 12 && pos2D_getY(pos) == 6) {
        printf("test passed\n");
        pos2D_delete(pos);
    } else {
        printf("test failed <<<<<<<\n");
    }
    
    printf("\n_______ grid_getPos() _______\n");
    pos = pos2D_new(0,0);
    printf("Null grid input:\n");
    char c = grid_getPos(NULL, pos);
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\nNull pos input:\n");
    c = grid_getPos(grid1, NULL);
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }

    printf("\npos out of scope in x:\n");
    pos = pos2D_new(12, 1);
    c = grid_getPos(grid1, pos);
    if (c != '\0') {
        printf("test failed\n");
    }
    pos2D_delete(pos);

    printf("\npos out of scope in y:\n");
    pos = pos2D_new(4, 6);
    c = grid_getPos(grid1, pos);
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\ncall on wall corner:\n");
    pos = pos2D_new(0, 0);
    c = grid_getPos(grid1, pos);
    if (c == '+') {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\ncall on verticle wall:\n");
    pos = pos2D_new(0, 1);
    c = grid_getPos(grid1, pos);
    if (c == '+') {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\ncall on wall corner:\n");
    pos = pos2D_new(1, 0);
    c = grid_getPos(grid1, pos);
    if (c == '+') {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\ncall on open room slot:\n");
    pos = pos2D_new(1, 1);
    c = grid_getPos(grid1, pos);
    if (c == '.') {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\ncall on empty slot:\n");
    pos = pos2D_new(1, 5);
    c = grid_getPos(grid1, pos);
    if (c == ' ') {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);


    printf("\n_______ grid_setPos() _______\n");
    pos = pos2D_new(0,0);
    printf("Null grid input:\n");
    char s = 's';
    grid_setPos(NULL, pos, s);
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }

    printf("\nNull pos input:\n");
    grid_setPos(grid1, NULL, s);
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }

    printf("\nNull '\\0' charicter input:\n");
    grid_setPos(grid1, pos, '\0');
    if (c != '\0') {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);

    printf("\npos out of scope in x:\n");
    pos = pos2D_new(12, 1);
    grid_setPos(grid1, pos, s);
    pos2D_delete(pos);

    printf("\npos out of scope in y:\n");
    pos = pos2D_new(4, 6);
    grid_setPos(grid1, pos, s);
    pos2D_delete(pos);

    printf("\nproperly functioning:\n");
    pos = pos2D_new(0, 0);
    grid_setPos(grid1, pos, s);
    c = grid_getPos(grid1, pos);
    if (c == s) {
        printf("test passed\n");
    } else {
        printf("test failed <<<<<<<\n");
    }
    pos2D_delete(pos);
    
    printf("_______ grid_delete() _______\n");
    printf("NULL grid passed:\n");
    grid_delete(NULL);

    printf("\ndelete full grid:\n");
    grid_delete(grid1);

}
