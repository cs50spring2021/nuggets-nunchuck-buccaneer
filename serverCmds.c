#include <stdio.h>
#include <unistd.h>
#include "pos2D.h"
#include "message.h"
#include "gameInfo.h"
#include "playerInfo.h"
#include "map.h"
#include <ctype.h>
#include "serverCmds.h"


const int maxNameLength = 50;   // max number of chars in playerName
const int maxPlayers = 26;      // maximum number of players
const int goldTotal = 250;      // amount of gold in the game
const int goldMinNumPiles = 10; // minimum number of gold piles
const int goldMaxNumPiles = 30; // maximum number of gold piles

static void sendDisplays(gameInfo_t* gameinfo, addr_t Player, int goldCollected);
static bool shortMove(gameInfo_t* gameinfo, addr_t addr, char dir, int* goldCollected);
static pos2D_t* dirToMovement(pos2D_t* start, char dir);
static void endGame(gameInfo_t* gameinfo);

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
gameInfo_t* initializeGame(char* mapFile){
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
bool movePlayer(gameInfo_t* gameinfo, addr_t addr, char input){
	//Check Args
	if(gameinfo == NULL){
		fprintf(stderr, "movePlayer: Invalid Args passed");
		return NULL;
	}
	//Check that it is not a spectator
	playerInfo_t* player = gameInfo_getPlayer(gameinfo, addr);
	if(player->username == NULL){
		//Exit without doing anything
		return false;
	}
	//Create var for goldCollected
	int goldCollected = 0;
	//Check if not sprint
	if(isupper(input) == 0){
		//Do one short move
		shortMove(gameinfo, addr, input, &goldCollected);
	} else {
		//Loop short moves until unable to
		while(shortMove(gameinfo, addr, tolower(input), &goldCollected));
	}
	//Send new Game State
	sendDisplays(gameinfo, addr, goldCollected);
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
static bool shortMove(gameInfo_t* gameinfo, addr_t addr, char dir, int* goldCollected){
	//Check args
	if(gameinfo == NULL){
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
		*goldCollected += gameInfo_pickupGold(gameinfo, addr);
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
	return true;
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
adds a player, calls ensureDimensions on player, and places them on the map also
can add a user as a spectator if name is NULL
Caller Provides:
	A gameInfo to update
	A address for the player to create
	A player name to assign to that player, NULL for spectator
We Do:
	Add a player to the game or a spectator, send the required dimensions to the player
	and places the player on the map
*/
void joinUser(gameInfo_t* gameinfo, addr_t player, char* playerName) 
{
  // Check args
  if (gameinfo == NULL || playerName == NULL) {
    fprintf(stderr, "joinUser: Invalid Args passed");
    return;
  }
  char* message;
  int nrows = 0; 
  int ncols = 0;
  pos2D_t* pos;

  // gets the rows by columns from the pos2D struct
  map_t* map = gameInfo_getMap(gameinfo);
  pos2D_t* terminalSize = map_getWidthheight(map);
  nrows = pos2D_getX(terminalSize);
  ncols = pos2D_getY(terminalSize);

  message = mem_malloc_assert((sizeof(char) * 20) + 1, "joinUser(): Mem Message");
  if (message == NULL) {
    fprintf(stderr, "error: issue encountered while allocating memory for"
    " the message that's sent to the server.\n");
    exit(1);
  }

  /* writes a message that'll be sent to the client to check the dimensions 
  of their window */
  sprintf(message, "GRID %d %d", nrows, ncols);
  // sends the GRID message to the client
  message_send(player, message);

  // if player name is not provided, add the user as a spectator
  if (playerName == NULL) {
	gameInfo_addSpectator(gameinfo, player);
  } else {
	// get the map
	if ((map = gameInfo_getMap(gameinfo)) == NULL) {
	  fprintf(stderr, "error: gameinfo provided is NULL.\n");
      free(message);
	  exit(2);
	}
	// generate a random position to place the new user in the map
	if ((pos = map_randomEmptySquare(map)) == NULL) {
	  fprintf(stderr, "error: map provided is NULL.\n");
      free(message);
	  exit(3);
    } 
	// add the new user to the game info
	gameInfo_addPlayer(gameinfo, player, pos, playerName);

	free(pos);
  }
  // send the updated gameinfo to all clients.
  sendDisplays(gameinfo, message_noAddr(), 0);
  free(message);
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
bool leaveUser(gameInfo_t* gameinfo, addr_t player)
{
  // Check args
  if (gameinfo == NULL) {
	fprintf(stderr, "leaveUser: Invalid Args passed.\n");
	return false;
  }
  map_t* map;
  playerInfo_t* playerinfo;
  pos2D_t* pos;

  // get the map
  if ((map = gameInfo_getMap(gameinfo)) == NULL) {
    fprintf(stderr, "error: gameinfo provided is NULL.\n");
	exit(1);
  }	
  // gets the playerinfo struct
  if ((playerinfo = gameInfo_getPlayer(gameinfo, player)) == NULL) {
    fprintf(stderr, "error: couldn't retrieve playerinfo struct.\n");
    exit(2);
  }
  // gets the position of the player that will be cleared on the map
  pos = playerinfo->pos;

  // clears the spot on the map
  map_clearSpot(map, pos);
  // player is removed from gameinfo
  gameInfo_removePlayer(gameinfo, player);
  // send the updated display to all players
  sendDisplays(gameinfo, message_noAddr(), 0);

  // checks to see if the last player has left the server
  if (gameInfo_getNumPlayers(gameinfo) == 0) {
    return true;
  }
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

static void sendDisplays(gameInfo_t* gameinfo, addr_t addr, int goldCollected){
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
				if(message_eqAddr(addr, player->address)){
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
			message_send(player->address, msgBuffer);
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
static void endGame(gameInfo_t* gameinfo)
{
  // Check args
  if (gameinfo == NULL) {
	fprintf(stderr, "endGame: Invalid gameinfo struct passed.\n");
	return;
  }
  char* scoreboard;
  char* message;
  playerInfo_t* player;
  addr_t playerAddress;

  // gets the scoreboard to send to all the players when they are disconnected
  if ((scoreboard = gameInfo_createScoreBoard(gameinfo)) == NULL) {
    fprintf(stderr, "error: scoreboard returned is NULL.\n");
    exit(1);
  }

  // allocates memory for the message that'll be sent to the user
  message = mem_malloc_assert((sizeof(char*) * (5 + strlen(scoreboard))) + 1, "endGame(): Mem message");
  if (message == NULL) {
    fprintf(stderr, "error: issue encountered while allocating memory for"
    " the message.\n");
    exit(2);
  }
  // constructs the quit message sent to the user
  sprintf(message, "QUIT %s", scoreboard);

  // loop over all the playerIDs and disconnect all of the players
  for (int i = 0; i < 26; i++) {
    if ((player = gameInfo_getPlayerFromID(gameinfo, i)) == NULL) {
      // error occurred or a player does not exist at this index
      continue;
    }
    // grab the player address
    playerAddress = player->address;
    // send the quit message to the specific player
    message_send(playerAddress, message);
  }	
}
