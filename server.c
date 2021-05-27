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
#include "playerInfo.h"
#include <ctype.h>

//CONSTANTS

const int maxNameLength = 50;   // max number of chars in playerName
const int maxPlayers = 26;      // maximum number of players
const int goldTotal = 250;      // amount of gold in the game
const int goldMinNumPiles = 10; // minimum number of gold piles
const int goldMaxNumPiles = 30; // maximum number of gold piles

// LOCAL FUNCTIONS, PROTOTYPES

static void parseArgs(const int argc, char *argv[], char** mapFilePath, int* seed);
static gameInfo_t* initializeGame(char* mapFile);
static bool shortMove(gameInfo_t* gameinfo, addr_t* addr, char dir);
static pos2D_t* dirToMovement(pos2D_t* start, char dir);
static void sendDisplays(gameInfo_t* gameinfo);
static void endGame(gameInfo_t* gameinfo);

// FUNCTIONS

bool movePlayer(gameInfo_t* gameinfo, addr_t* player, char input);

#ifndef SERVERTEST
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
	startNetworkServer(gameInfo);
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

/******************* initializeGame *********************
Makes the map places gold on the map and initializes a gameInfo struct
Caller Provides:
	A string that is a path that leads to a valid map file
We Return:
	A game info with initialized map, array of players and gold info
	Null on an error
Caller is Responsible For:
	Later calling gameInfo_delete
*/
static gameInfo_t* initializeGame(char* mapFile){
	//Check Args
	if(mapFile == NULL){
		fprintf(stderr, "initializeGame: Invalid Args passed");
		return NULL;
	}
	//Generate Random gold pile numbers
	int piles = goldMinNumPiles + (rand() / (goldMaxNumPiles - goldMinNumPiles));
	printf("PILES: %d\n", piles);
	//Create a gameInfo
	gameInfo_t* gameInfo = mem_assert(gameInfo_newGameInfo(piles, goldTotal, mapFile),"Server Main: mem gameInfo");
	//Add gold piles to the map
	map_t* map = gameInfo_getMap(gameInfo);
	for(int i = 0 ; i < piles; i++){
		map_putOneGold(map);
	}
	return gameInfo;
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
bool movePlayer(gameInfo_t* gameinfo, addr_t* addr, char input){
	//Check Args
	if(gameinfo == NULL || addr == NULL){
		fprintf(stderr, "movePlayer: Invalid Args passed");
		return NULL;
	}
	//Check that it is not a spectator
	playerInfo_t* player = gameInfo_getPlayer(gameinfo, addr);
	if(player->username == NULL){
		//Exit without doing anything
		return false;
	}
	//Check if not sprint
	if(isupper(input) == 0){
		//Do one short move
		shortMove(gameinfo, addr, input);
	} else {
		//Loop short moves until you are unable to
		while(shortMove(gameinfo, addr, tolower(input)));
	}
	//Send new Game State
	sendDisplays(gameinfo);
	//Check for end
	if(gameInfo_getGoldPiles(gameinfo) == 0){
		endGame(gameinfo);
		return true;
	}
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
	True if move was possible
	False if move was impossible
	Otherwise on error returns false
*/
static bool shortMove(gameInfo_t* gameinfo, addr_t* addr, char dir){
	//Check args
	if(gameinfo == NULL || addr == NULL){
		fprintf(stderr, "shortMove: Invalid Args passed");
		return false;
	}
	//Find the pos we need to go to 
	playerInfo_t* player = gameInfo_getPlayer(gameinfo, addr);
	pos2D_t* toPos = dirToMovement(player->pos, dir);
	map_t* map = gameInfo_getMap(gameinfo);
	char current = map_getGamePos(map, toPos);
	//Check if out of bounds
	if(current == '\0'){
		return false;
	}
	//Check if wall or empty space
	if(current == '+' || current == ' '){
		return false;
	}
	//Check if gold
	if(current == '*'){
		gameInfo_pickupGold(gameinfo, addr);
	}
	int movedPlayer = -1;
	playerInfo_t* displaced = NULL;
	//Check for another Player, Capital letter
	if(isupper(current) == 1){
		//Find ID
		movedPlayer = current - 65;
		//Set player recorded pos
		displaced = gameInfo_getPlayerFromID(gameinfo, movedPlayer);
		pos2D_set(displaced->pos, pos2D_getX(player->pos), pos2D_getY(player->pos));
	}
	//Move player to spot on map
	pos2D_set(player->pos, pos2D_getX(toPos), pos2D_getY(toPos));
	map_setPlayerPos(map, player->pos, player);
	if(movedPlayer != -1){
		//Update displaced on the map
		map_setPlayerPos(map, displaced->pos, displaced);
	}
	//Delete to Pos
	pos2D_delete(toPos);
}

/******************* dirToMovement *********************
Gets a position from a direction and start pos
Caller Provides:
	A dir to go in, lowercase adjacent
	A starting pos
We Return:
	NULL on any error
	Otherwise we return a pos that is the pos from the start pos in a certain direction
Caller is Responsible For:
	Later freeing the returned pos
*/
static pos2D_t* dirToMovement(pos2D_t* start, char dir){
	//Change Args
	if(start == NULL){
		fprintf(stderr, "dirToMovement: Invalid Args passed");
		return NULL;
	}
	//Get direction and return a new pos that is the appropriate spot to move
	switch(dir) {
		case 'k'  :
			//UP
			return mem_assert(pos2D_new(pos2D_getX(start), pos2D_getY(start) - 1), "dirToMovement: Pos Memory");
      		break;
		case 'l'  :
			//RIGHT
			return mem_assert(pos2D_new(pos2D_getX(start) + 1, pos2D_getY(start)), "dirToMovement: Pos Memory");
      		break;
		case 'j'  :
			//DOWN
			return mem_assert(pos2D_new(pos2D_getX(start), pos2D_getY(start) + 1), "dirToMovement: Pos Memory");
      		break;
		case 'h'  :
			//LEFT
			return mem_assert(pos2D_new(pos2D_getX(start) - 1, pos2D_getY(start)), "dirToMovement: Pos Memory");
      		break;
		case 'u'  :
			//UPRIGHT
			return mem_assert(pos2D_new(pos2D_getX(start) + 1, pos2D_getY(start) - 1), "dirToMovement: Pos Memory");
      		break;
		case 'n'  :
			//DOWNRIGHT
			return mem_assert(pos2D_new(pos2D_getX(start) + 1, pos2D_getY(start) + 1), "dirToMovement: Pos Memory");
      		break;
		case 'y'  :
			//UPLEFT
			return mem_assert(pos2D_new(pos2D_getX(start) - 1, pos2D_getY(start) - 1), "dirToMovement: Pos Memory");
      		break;
		case 'b'  :
			//DOWNLEFT
			return mem_assert(pos2D_new(pos2D_getX(start) - 1, pos2D_getY(start) + 1), "dirToMovement: Pos Memory");
      		break;
		default : 
			fprintf(stderr, "shortMove: Invalid movement key");
			return NULL;
	}
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
static void sendDisplays(gameInfo_t* gameinfo, addr_t* addr, int goldCollected){
	// Check Args
	if(gameinfo == NULL){
		fprintf(stderr, "sendDisplays: Provided gameInfo is NULL");
		return; 
	}
	// Get Gold Score Remaining
	int scoreLeft = gameInfo_getScoreRemaining(gameinfo);
	//Loop through IDs for players
	for(int i = 0; i < maxPlayers; i++){
		playerInfo_t* player = NULL; 
		if((player = gameInfo_getPlayerFromID(gameinfo, i)) != NULL){
			//Make space for the message
			char msgBuffer[41];
			//Check if Spectator
			if((player->username) == NULL){
				//Create the header message for spectator
				sprintf(msgBuffer, "GOLD 0 -1 %d", scoreLeft);
			} else {
				//Check if it was this player that collected gold
				if(message_eqAddr(*addr, *(player->address))){
					// Create the header message for collecting player
					sprintf(msgBuffer, "GOLD %d %d %d", goldCollected, (player->score), scoreLeft);
				} else {
					// Create the header message for non-collecting player
					sprintf(msgBuffer, "GOLD 0 %d %d", (player->score), scoreLeft);
				}
				if(!gameInfo_updateSightGrid(gameinfo, (player->address))){
					fprintf(stderr, "sendDisplays: SightGrid update failed");
				}
			}
			//Send the displayHeader message
			message_send(*(player->address), msgBuffer);
			// Get visible map from player
			grid_t* seen = map_getVisibleMap(gameInfo_getMap(gameinfo), player->sightGrid);
			// Create Display message
			char* stringOfSeen = grid_toString(seen);
			char* displayMsg = mem_malloc_assert(sizeof(char) * (strlen(stringOfSeen) + strlen("DISPLAY\n") + 1), "sendDisplays: mem for display msg failed");
			sprintf(displayMsg, "DISPLAY\n%s", stringOfSeen);
			//Clean up
			mem_free(displayMsg);
			grid_delete(seen);
		}
	}
}

/******************* endGame *********************
disconnect everyone, give them Scoreboards
Caller Provides:
	A gameInfo to update
We Do:
	Send a new display to each player that contains the endgame scoreboard constructed from gameinfo
	Send a quit message to all players
*/
static void endGame(gameInfo_t* gameinfo){
	return;
}