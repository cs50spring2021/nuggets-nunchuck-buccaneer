/*
 * servertest.c - test program for server.c
 *
 * Nunchuck-Buccaneers
 * 05/24/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.c"
#include "network.h"
#include "gameInfo.h"


int 
main() {
    // initialize values
    char* mapFile = "../maps/big.txt";
    int seed = getpid();
    srand(seed);
    gameInfo_t* game = intializeGame(mapFile);

    startNetworkServer(game);

    gameInfo_delete(game);
    
    return 0; // succecssful run!
}