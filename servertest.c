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
	message_setAddr("26346", "122345", &player1);

	fprintf(stderr, "\nINVALID MESSAGE\n");
	handleMessage(args, player1, "Message");

	fprintf(stderr, "\nJOIN MESSAGE\n");
	handleMessage(args, player1, "PLAY Doodle");

	map_t* map = gameInfo_getMap(gameInfo);
	pos2D_t* mapWH = map_getWidthheight(map);
	for(int y = 0 ; y < pos2D_getY(mapWH); y++){
		for(int x = 0 ; x < pos2D_getX(mapWH); x++){
			pos2D_t* pos = pos2D_new(x,y);
			fprintf(stderr, "%c", map_getGamePos(map, pos));
			pos2D_delete(pos);
		}
		fprintf(stderr, "\n");
	}

	// fprintf(stderr, "\MOVE MESSAGE\n");
	// handleMessage(args, player1, "KEY l");

	// fprintf(stderr, "\nLEAVE MESSAGE\n");
	// handleMessage(args, player1, "KEY Q");

	//fprintf(stderr, "\nJOIN SPECTATOR\n");
	//handleMessage(args, player1, "PLAY");

	mem_free(args->playerID);
	mem_free(args);
	gameInfo_delete(gameInfo);    
}