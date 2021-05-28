/*
 * testing file for the network.c module
 *
 * Nunchuck Buccaneers
 * CS50 - Spring 2021
 * 05/28/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "network.h"
#include "../server.h"

int main(){
    srand(getpid());
	gameInfo_t* gameInfo = mem_assert(initializeGame("../maps/hole.txt"), "Server Main: initializeGame mem");
	startNetworkServer(gameInfo, stderr);
	gameInfo_delete(gameInfo);    
}