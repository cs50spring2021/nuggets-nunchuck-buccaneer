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
#include <string.h>
#include "server.c"
#include "network.h"
#include "gameInfo.h"

typedef struct loopArgs {
    gameInfo_t* gameinfo;
    char* playerID;
} loopArgs_t;

static void showMap(gameInfo_t* gameinfo);
const int seed = 12345;

int main(){
	srand(seed);
	gameInfo_t* gameInfo = mem_assert(initializeGame("maps/hole.txt"), "Server Main: initializeGame mem");
	//Create args struct for loop
  	loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "startNetworkServer(): Mem Error for args");
  	args->playerID = mem_malloc_assert(sizeof(char), "startNetworkServer(): Mem error id");
  	args->gameinfo = gameInfo;
  	*(args->playerID) = '@';
	addr_t player1 = message_noAddr();
	message_setAddr("26346", "122345", &player1);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nINVALID MESSAGE\n");
	handleMessage(args, player1, "Message");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN MESSAGE\n");
	handleMessage(args, player1, "PLAY Doodle");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN MESSAGE\n");
	handleMessage(args, player1, "PLAY Beep");

	showMap(gameInfo);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nLEAVE MESSAGE\n");
	if(handleMessage(args, player1, "KEY Q")){
		fprintf(stderr, "END GAME\n");
	} else {
		fprintf(stderr, "CONTINUE GAME\n");
	}

	showMap(gameInfo);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nLEAVE MESSAGE 2ND PLAYER\n");
	if(handleMessage(args, player1, "KEY Q")){
		fprintf(stderr, "END GAME\n");
	} else {
		fprintf(stderr, "CONTINUE GAME\n");
	}
	showMap(gameInfo);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN MESSAGE\n");
	handleMessage(args, player1, "PLAY Ahh");

	showMap(gameInfo);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE MESSAGE\n");
	handleMessage(args, player1, "KEY l");

	showMap(gameInfo);

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE AND COLLECT GOLD\n");
	handleMessage(args, player1, "KEY l");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE AWAY\n");
	handleMessage(args, player1, "KEY l");

	
	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nSPRINT DOWN\n");
	handleMessage(args, player1, "KEY J");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE INTO WALL\n");
	handleMessage(args, player1, "KEY j");
	
	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nSPRINT RIGHT\n");
	handleMessage(args, player1, "KEY L");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE UP X3\n");
	handleMessage(args, player1, "KEY k");
	handleMessage(args, player1, "KEY k");
	handleMessage(args, player1, "KEY k");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE RIGHT X3\n");
	handleMessage(args, player1, "KEY l");
	handleMessage(args, player1, "KEY l");
	handleMessage(args, player1, "KEY l");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nMOVE INTO EMPTY\n");
	handleMessage(args, player1, "KEY j");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN SPECTATOR\n");
	handleMessage(args, player1, "SPECTATE");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN NEW SPECTATOR\n");
	handleMessage(args, player1, "SPECTATE");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nLEAVE MESSAGE 3RD PLAYER\n");
	if(handleMessage(args, player1, "KEY Q")){
		fprintf(stderr, "END GAME\n");
	} else {
		fprintf(stderr, "CONTINUE GAME\n");
	}

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nLEAVE MESSAGE SPECTATOR\n");
	if(handleMessage(args, player1, "KEY Q")){
		fprintf(stderr, "END GAME\n");
	} else {
		fprintf(stderr, "CONTINUE GAME\n");
	}

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN SPECTATOR\n");
	handleMessage(args, player1, "SPECTATE");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN A PLAYER\n");
	handleMessage(args, player1, "PLAY uhoh");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nJOIN ANOTHER PLAYER\n");
	handleMessage(args, player1, "PLAY alright");

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\n COLLIDE THEM\n");
	handleMessage(args, player1, "KEY J");
	handleMessage(args, player1, "KEY H");

	
	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nLEAVE MESSAGE\n");
	if(handleMessage(args, player1, "KEY Q")){
		fprintf(stderr, "END GAME\n");
	} else {
		fprintf(stderr, "CONTINUE GAME\n");
	}

	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\n MOVE TO CHECK PROPER DISPLACEMENT\n");
	handleMessage(args, player1, "KEY U");
	handleMessage(args, player1, "KEY J");


	fprintf(stderr, "\n************************************\n");
	fprintf(stderr, "\nEND GAME\n");
	endGame(gameInfo);

	mem_free(args->playerID);
	mem_free(args);
	gameInfo_delete(gameInfo);
}

static void showMap(gameInfo_t* gameinfo){
	fprintf(stderr, "CURRENT MAP\n");
	map_t* map = gameInfo_getMap(gameinfo);
	pos2D_t* mapWH = map_getWidthheight(map);
	for(int y = 0 ; y < pos2D_getY(mapWH); y++){
		for(int x = 0 ; x < pos2D_getX(mapWH); x++){
			pos2D_t* pos = pos2D_new(x,y);
			fprintf(stderr, "%c", map_getGamePos(map, pos));
			pos2D_delete(pos);
		}
		fprintf(stderr, "\n");
	}
	pos2D_delete(mapWH);
}