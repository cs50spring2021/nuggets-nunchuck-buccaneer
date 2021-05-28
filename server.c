/*
	server.c - runs the server and parses and receives Messages from Clients, sends relevant visible maps for all players.
	Handles all the GameLogic and initializes the map.

	Nunchuck-Buccaneers
	CS50, 5/12/2021

*/

// INCLUSIONS

#include <stdio.h>
#include <unistd.h>
#include "pos2D.h"
#include "message.h"
#include "gameInfo.h"
#include "playerInfo.h"
#include "map.h"
#include <ctype.h>
#include "serverCmds.h"

// LOCAL FUNCTIONS, PROTOTYPES

static void parseArgs(const int argc, char *argv[], char** mapFilePath, int* seed);

// FUNCTIONS

#ifndef TESTING
/******************* main *********************
parses args, uses networkServer to start server, initializes Game, start networkServer(MessageLoop)
Caller Provides:
	Two arguments a file that stores a map and a seed that is randomness
We Do:
	exit 1 on an unrecoverable error and print a message informing about error,
	exit 0 when we leave cleanly, Runs the server for Nuggets and waits for input
*/
int main(const int argc, char *argv[]){
	// Check Num Args
	if(argc != 3 && argc != 2){
		fprintf(stderr, "Usage: ./server map.txt [seed]\n");
		exit(1);
	}
	// Parse the args
	char* mapFilePath = NULL;
	int seed = -1;
	parseArgs(argc, argv, &mapFilePath, &seed);
	//Do randomness initialization
	if(seed == -1){
		srand(seed);
	} else {
		srand(getpid());
	}
	//Create game info
	gameInfo_t* gameInfo = mem_assert(initializeGame(mapFilePath), "Server Main: initializeGame mem");
	//Start the network server
	startNetworkServer(gameInfo, stderr);
	//Deletes the gameinfo
	gameInfo_delete(gameInfo);
}
#endif

/******************* parseArgs - helper for main *********************
parses args and finds a seed and a mapFilePath, if no seed doesn't set it
Caller Provides:
	An argument count
	An argument array
	A pointer to a mapFile string to fill
	A pointer to a seed integer to fill
We Do:
	exit 1 on an unrecoverable error and print a message informing about error,
	Otherwise we fillout the mapFile string and seed int with the provided
	values, leaving seed alone if no seed is provided
*/
static void parseArgs(const int argc, char *argv[], char** mapFilePath, int* seed){
	// Parse seed if provided
	if(argc == 3){
		char* seedString = argv[2];
		char excess;
		//Get Seed
    	if (sscanf(seedString, "%d%c", seed, &excess) != 1) {
			fprintf(stderr, "Seed is not a valid integer\n");
			exit(1);
		}
		//Check if seed is negative
		if(*seed < 0){
			fprintf(stderr, "Seed must be positive\n");
			exit(1);
		}
	}
	//Check Map
	*mapFilePath = argv[1];
    FILE *fp;
	//Open Map file
  	if ((fp = fopen(*mapFilePath, "r")) == NULL) {
    	fprintf(stderr, "Map is not readable\n");
    	exit(1);
  	}
	//Close map file
  	fclose(fp);
	return;
}