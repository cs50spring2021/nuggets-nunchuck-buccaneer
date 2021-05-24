/*
 * testing file for the map.c module
 *
 * William French
 * Nunchuck Buccaneers
 * CS50 - Spring 2021
 * 05/24/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "map.h"
#include "grid.h"

/* ******* helper_fucntion *****/

void print_gameGrid(map_t* map)
{
    char* gameSightStr =
    "22222222222222\n"
    "22222222222222\n"
    "22222222222222\n"
    "22222222222222\n"
    "22222222222222\n";


    grid_t* gameSight = grid_new(gameSightStr);
    grid_t* gameGrid = map_getVisibleMap(map, gameSight);
    char* gameString = grid_toString(gameGrid);      
    printf("%s\n", gameString);
    grid_delete(gameSight);
    grid_delete(gameGrid);
    free(gameString);
}


int main()
{
    map_t* map1 = NULL;
    printf("_________ map_new() __________\n");
    printf("NULL input: \n");
    map1 = map_new(NULL);
    if (map1 != NULL) {
        printf("test fail <<<<<<<<<<<\n");
    }
    
    printf("\ninvalid mapFile path: \n");
    char* path = "not_a_file_path";
    map1 = map_new(path);
    if (map1 != NULL) {
        printf("test fail <<<<<<<<<<<\n");
    }

    printf("\nproperly functioning: \n");
    path = "../maps/small.txt";
    map1 = map_new(path);
    if (map1 == NULL) {
        printf("test fail <<<<<<<<<<<< \n");
    } else {
        printf("test passed\n");
    }
    
    printf("\n__________ map_getBaseGrid() __________\n");
    printf("NULL input:\n");
    grid_t* baseGrid = map_getBaseGrid(NULL);
    if (baseGrid != NULL) {
        printf("test failed <<<<<<<<<<<\n");
    } 

    printf("\nproperly functioning: \n");
    baseGrid = map_getBaseGrid(map1);
    if (baseGrid == NULL) {
        printf("test failed <<<<<<<<<<<\n");
    } else {
        char* map = grid_toString(baseGrid);
        printf("%s\n", map);
        free(map);
    }


    printf("\n__________ map_setPlayerPos() __________\n");
    printf("NULL 'map' input:\n");
    pos2D_t* pos0 = pos2D_new(5,0);
    pos2D_t* pos1 = pos2D_new(5,1);
    pos2D_t* pos2 = pos2D_new(5,2);
    pos2D_t* pos3 = pos2D_new(5,3);
    playerInfo_t* player = mem_malloc_assert(sizeof(playerInfo_t), "maptest");
    player->pos = pos0;
    player->playerID = 0;

    map_setPlayerPos(NULL, pos1, player);
    if (pos2D_getY(player->pos) == 1) {
        printf("test failed <<<<<<<<<<<\n");
    } 
    printf("\nNULL 'pos' input:\n");
    map_setPlayerPos(map1, NULL, player);
    if (pos2D_getY(player->pos) == 1) {
        printf("test failed <<<<<<<<<<<\n");
    } 
    printf("\nNULL 'player' input:\n");
    map_setPlayerPos(map1, pos1, NULL);
    if (pos2D_getY(player->pos) == 1) {
        printf("test failed <<<<<<<<<<<\n");
    }
    printf("\nNULL player-ID input:\n");
    map_setPlayerPos(map1, pos1, player);
    if (pos2D_getY(player->pos) == 1) {
        printf("test failed <<<<<<<<<<<\n");
    } 
    player->playerID = 'A';

    printf("\nStandard functioning:\n");
    printf("No player:\n");
    print_gameGrid(map1);
    map_setPlayerPos(map1, pos1, player);
    printf("Player at 5,1:\n");
    print_gameGrid(map1);
    map_setPlayerPos(map1, pos2, player);
    printf("Player at 5,2:\n");
    print_gameGrid(map1);

    printf("Test that it doesn't delete other players when swapping:\n");
    player->playerID = 'B';
    map_setPlayerPos(map1, pos3, player);
    printf("Player A at 5,2. Player B at 5,3:\n");
    print_gameGrid(map1);
    pos2D_delete(pos3);
    free(player);


    printf("\n__________ map_randomEmptySquare() __________\n");
    printf("Null pointer passed:\n");
    pos2D_t* pos4 = map_randomEmptySquare(NULL);
    if (pos4 != NULL) {
        printf("test failed <<<<<<<<<<<<<<<<<\n");
    }
    printf("Properly Functioning:\n");
    printf("These should look like random coordiantes with "
           "2 < x < 14, 0 < y <  4:\nNote: a 'test failed' message will "
           "apear if a coordiante set doesn't meet criteria\n");
    int i = 20;
    srand(getpid());
    int testExit = 0;
    while (i != 0) {
        pos4 = map_randomEmptySquare(map1);
        int x = pos2D_getX(pos4);    
        int y = pos2D_getY(pos4);
        printf("%d, %d\n", x, y);
        if (x >= 14 || x<=2 || y<=0 || y>=4) {
            printf("test failed <<<<<<<<<\n");
            testExit = 1;
        }
        pos2D_delete(pos4);
        i--;
    }
    if (testExit == 0) {
        printf("test passed\n");
    }


    printf("\n__________ map_putOneGold() __________\n");
    printf("Null pointer passed:\n");
    map_putOneGold(NULL);
    printf("\nProperly Functioning:\n");
    printf("No gold:\n");    
    print_gameGrid(map1);
    printf("one gold:\n"); 
    map_putOneGold(map1);
    print_gameGrid(map1);
    printf("two gold:\n"); 
    map_putOneGold(map1);
    print_gameGrid(map1);
    printf("three gold:\n"); 
    map_putOneGold(map1);
    print_gameGrid(map1);

    printf("\n__________ map_getWidthheight() __________\n");
    printf("Null pointer passed:\n");
    pos2D_t* pos5 = map_getWidthheight(NULL);

    printf("\nProperly Functioning:\n");
    pos5 = map_getWidthheight(map1);
    if (pos2D_getX(pos5) == 14 || pos2D_getY(pos5) == 5) {
        printf("test passed\n");
    } else {
        printf("\ntest failed <<<<<<<<<<<<<\n");
    }
    pos2D_delete(pos5);

    printf("\n__________ map_getGamePos() __________\n");
    printf("NULL'map'  pointer passed:\n");
    pos2D_t* pos6 = pos2D_new(3,0);
    map_getGamePos(NULL, pos6);
    printf("\nNULL 'pos' pointer passed:\n");
    map_getGamePos(map1, NULL);
    
    printf("\nProperly functioning:\n");
    char spot = map_getGamePos(map1, pos6);
    if (spot == '+') {
        printf("test 1 passed\n");
    } else {
        printf("test 1 failed <<<<<<<<<<<<<<\n");
    }

    pos2D_set(pos6, 4, 0);
    spot = map_getGamePos(map1, pos6);
    if (spot == '+') {
        printf("test 2 passed\n");
    } else {
        printf("test 2 failed <<<<<<<<<<<<<<\n");
    }

    pos2D_set(pos6, 5, 2);
    spot = map_getGamePos(map1, pos6);
    if (spot == 'A') {
        printf("test 3 passed\n");
    } else {
        printf("test 4 failed <<<<<<<<<<<<<<\n");
    }

    pos2D_set(pos6, 10, 2);
    spot = map_getGamePos(map1, pos6);
    if (spot == '.' || spot == '*') {
        printf("test 3 passed\n");
    } else {
        printf("test 4 failed <<<<<<<<<<<<<<\n");
    }
    pos2D_delete(pos6);

    printf("\n__________ map_getVisibleMap() __________\n");
    printf("NULL 'map' input:\n");

    char* baseSightStr =
    "11111111111111\n"
    "11111111111111\n"
    "11111111111111\n"
    "11111111111111\n"
    "11111111111111\n";
    
    grid_t* baseSGrid = grid_new(baseSightStr);
    baseGrid = map_getVisibleMap(NULL, baseSGrid);
    if (baseGrid != NULL) {
        printf("test failed <<<<<<<<<<<\n");
    }

    printf("\nNULL 'sightGrid' input:\n");
    baseGrid = map_getVisibleMap(map1, NULL);
    if (baseGrid != NULL) {
        printf("test failed <<<<<<<<<<<\n");
    }

    printf("\neach map displayed previously in the test has used " 
            "map_getVisibleMap(). \nWe just want to test getting the baseGrid, and "
            "getting a mix of grids\n");
    printf("\nget the basegrid (all 1s):\n");
    

    baseGrid = map_getVisibleMap(map1, baseSGrid);
    char* baseString = grid_toString(baseGrid);
    printf("%s", baseString);
    free(baseString);
    grid_delete(baseGrid);
    grid_delete(baseSGrid);
    
    printf("\nget a mixegrid (mix of 0,1,2):\n");
    char* mixedSightStr =
    "20211100100000\n"
    "10011100210000\n"
    "20222200102000\n"
    "11022200010000\n"
    "00011100100000\n";
    grid_t* mixedSightGrid = grid_new(mixedSightStr);
    grid_t* mixedGrid = map_getVisibleMap(map1, mixedSightGrid);
    char* mixedString = grid_toString(mixedGrid);
    printf("%s", mixedString);
    free(mixedString);
    grid_delete(mixedGrid);
    grid_delete(mixedSightGrid);

    printf("\n__________ map_delete() __________\n");
    printf("NULL 'map' input:\n");
    map_delete(NULL);
    printf("\nSee valgrind to see if delete is succesful:\n\n");
    map_delete(map1);




}
    


