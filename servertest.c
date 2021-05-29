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
#include "serverCmds.h"

typedef struct loopArgs {
    gameInfo_t* gameinfo;
    char* playerID;
} loopArgs_t;

int main(){
    srand(getpid());
	gameInfo_t* gameInfo = mem_assert(initializeGame("maps/hole.txt"), "Server Main: initializeGame mem");
	//Create args struct for loop
  	loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "startNetworkServer(): Mem Error for args");
  	args->playerID = mem_malloc_assert(sizeof(char), "startNetworkServer(): Mem error id");
  	args->gameinfo = gameInfo;
  	*(args->playerID) = '@';
	addr_t player1 = message_noAddr();
	fprintf(stderr, "\nINVALID MESSAGE\n");
	handleMessage(args, player1, "Message");

	fprintf(stderr, "\nJOIN MESSAGE\n");
	handleMessage(args, player1, "PLAY Doodle");

	fprintf(stderr, "Reached End\n");
	gameInfo_delete(gameInfo);   
	fprintf(stderr, "Reached End\n");
 
}