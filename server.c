/*

    server.c
    runs the server and parses and receives Messages from Clients, sends relevant visible maps for all players.
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


//CONSTANTS

int maxNameLength = 50;   // max number of chars in playerName
int maxPlayers = 26;      // maximum number of players
int goldTotal = 250;      // amount of gold in the game
int goldMinNumPiles = 10; // minimum number of gold piles
int goldMaxNumPiles = 30; // maximum number of gold piles

// LOCAL FUNCTIONS, PROTOTYPES

static void parseArgs(const int argc, char *argv[], char** mapFilePath, int* seed);

// FUNCTIONS

/******************* main *********************
parses args, uses networkServer to start server, initializes Game, start networkServer(MessageLoop)
If no seed is provided it is set to -1 to signify full randomness
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
	printf("Seed: %d\n", seed);
	//Do randomness initialization
	if(seed == -1){
		srand(seed);
	} else {
		srand(getpid());
	}
	//gameInfo_new();
}

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

/******************* initializeGame *********************
Makes the map places gold on the map and initializes a gameInfo struct
Caller Provides:
	A string that is a path that leads to a valid map file
We Return:
	A game info with initialized map, array of players and gold info
Caller is Responsible For:
	Later calling gameInfo_delete
*/
gameInfo_t* initializeGame(char* mapFile){
	return NULL;
}

/******************* movePlayer *********************
a player and does a move for that player depending on the given command and collects gold for that player, updating displays
Caller Provides:
	A gameInfo to update
	A address for a player to move
	A char that is a movement command
We Return:
	True if the last pile of gold is collected by this move
	False if there is still gold after this move
*/
bool movePlayer(gameInfo_t* gameinfo, addr_t* player, char input){
	return false;
}

/******************* shortMove - movePlayer Helper *********************
gets a direction (an adjacent cmd) and a player and does a single
 move for that player and collects gold for that player
Caller Provides:
	A gameInfo to update
	A address for a player to move
	A char that is an adjacent movement command/direction
We Return:
	True if the last pile of gold is collected by this move
	False if there is still gold after this move
*/
	bool shortMove(gameInfo_t* gameinfo, addr_t* player, char dir){
		return false;
	}

/******************* joinUser *********************
Adds a player and places them on the map or adds a spectator
Caller Provides:
	A gameInfo to update
	A address for the player to create
	A player name to assign to that player
	and the terminal size for the player attempting to join
We Do:
	Check if the terminal size is big enough, if not sends a joinFail to the player with correct size
	Otherwise add the player to GameInfo with relevant info and if they are not a spectator
	places them into a random empty spot
	on the map
*/
void joinUser(gameInfo_t* gameinfo, addr_t* player, char* playerName, pos2D_t* terminalSize){
	return;
}

/******************* leaveUser *********************
gets a player and removes them from the game
Caller Provides:
	A gameInfo to update
	A Player to remove
We Do:
	Update the gameInfo to reflect the removal of a player, Update the map and other user displays to reflect the removal of a user
We return:
	True if the last player has left
	False if their are still players in the game
*/
bool leaveUser(gameInfo_t* gameinfo, addr_t* Player){
	return false;
}

/******************* sendDisplays *********************
Send visible map (get it via the maps getVisibleMap) and the relevant information that's displayed at the top of the screen. To all players
Caller Provides:
	A gameInfo to use for the header and to get the map from and to get the playerInfos from
We Do:
	Send each player a line containing their current nuggets and the nuggets left to collect in the game with their visable map contained below 
	it. To get the visible map we use the sightmaps from each playerinfo struct to combine into get VisibleMap
*/
void sendDisplays(gameInfo_t* gameinfo){
	return;
}

/******************* endGame *********************
disconnect everyone, give them Scoreboards
Caller Provides:
	A gameInfo to update
We Do:
	Send a new display to each player that contains the endgame scoreboard constructed from gameinfo
	Send a quit message to all players
*/
void endGame(gameInfo_t* gameinfo){
	return;
}