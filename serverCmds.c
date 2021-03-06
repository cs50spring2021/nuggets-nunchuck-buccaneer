
/* 
 * serverCmds.c - Handles the server-side functions of the game
 *
 * Nunchuck Buccaneers
 * cs50 - Spring 2021
 * 05/31/21
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "pos2D.h"
#include "message.h"
#include "gameInfo.h"
#include "playerInfo.h"
#include "map.h"
#include "serverCmds.h"


static const int maxNameLength = 50;   		// max number of chars in playerName
static const int maxPlayers = 26;      		// maximum number of players
static const int goldTotal = 250;      		// amount of gold in the game
static const int goldMinNumPiles = 10; 		// minimum number of gold piles
static const int goldMaxNumPiles = 30; 		// maximum number of gold piles

static void sendDisplays(gameInfo_t* gameinfo, addr_t Player, int goldCollected);
static bool shortMove(gameInfo_t* gameinfo, addr_t addr, char dir, int* goldCollected);
static pos2D_t* dirToMovement(pos2D_t* start, char dir, pos2D_t* mapWH);

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
	if (mapFile == NULL) {
		fprintf(stderr, "initializeGame: Invalid Args passed");
		return NULL;
	}
	//Generate Random gold pile numbers
	int piles = goldMinNumPiles + (rand() % (goldMaxNumPiles - goldMinNumPiles));
	fprintf(stderr, "PILES: %d\n", piles);

	//Create a gameInfo
	gameInfo_t* gameInfo = mem_assert(gameInfo_newGameInfo(piles, goldTotal, mapFile, maxPlayers),"Server Main: mem gameInfo");

	//Add gold piles to the map
	map_t* map = gameInfo_getMap(gameInfo);
	for (int i = 0 ; i < piles; i++) {
		map_putOneGold(map);
	}
	return gameInfo;
}

/******************* movePlayer *********************
What it does: a player and does a move for that player depending on 
the given command and collects gold for that player, updating displays
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
	if (gameinfo == NULL) {
		fprintf(stderr, "movePlayer: Invalid Args passed");
		return NULL;
	}

	// grab address pointer
    addr_t* addrP = mem_malloc_assert(sizeof(addr_t), "movePlayer: out of mem");
	*(addrP) = addr;

	//Check that it is not a spectator
	playerInfo_t* player = gameInfo_getPlayer(gameinfo, addrP);
	if (player->username == NULL) {
		// exit without doing anything
        free(addrP);
		return false;
	}
	// Create var for goldCollected
	int goldCollected = 0;
	// Check if not sprint
	if (isupper(input) == 0) {
		//Do one short move
		shortMove(gameinfo, addr, input, &goldCollected);
	} else {
		//Loop short moves until unable to
		while (shortMove(gameinfo, addr, tolower(input), &goldCollected)) {
			gameInfo_updateSightGrid(gameinfo, &addr);
		}
	}
	//Send new Game State
	sendDisplays(gameinfo, addr, goldCollected);
	//Check for end
	if (gameInfo_getGoldPiles(gameinfo) == 0) {
		printf("Server Ended: Gold Piles All Gone\n");
		endGame(gameinfo);
        free(addrP);
		return true;
	}
    free(addrP);
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
	if (gameinfo == NULL) {
		fprintf(stderr, "shortMove: Invalid Args passed");
		return false;
	}

	// grab address pointer
    addr_t* addrP = mem_malloc_assert(sizeof(addr_t), "movePlayer: out of mem");
	*(addrP) = addr;

	//Find the pos we need to go to 
	playerInfo_t* player = gameInfo_getPlayer(gameinfo, addrP);
	map_t* map = gameInfo_getMap(gameinfo);
	pos2D_t* mapWH = map_getWidthheight(map);
	pos2D_t* toPos = dirToMovement(player->pos, dir, mapWH);
	//Check in Bounds
	if(toPos == NULL){
        mem_free(addrP);
		return false;
	}
	pos2D_delete(mapWH);
	char current = map_getGamePos(map, toPos);
	//Check if wall or empty space
	if (current == '+' || current == ' ') {
		pos2D_delete(toPos);
        mem_free(addrP);
		return false;
	}
	//Check if gold
	if (current == '*') {
		*goldCollected += gameInfo_pickupGold(gameinfo, addrP);
	}
	int movedPlayer = -1;
	playerInfo_t* displaced = NULL;
	//Check for another Player, Capital letter
	if (isupper(current) != 0) {
		//Find ID
		movedPlayer = current - 65;
		//Set player recorded pos
		displaced = gameInfo_getPlayerFromID(gameinfo, movedPlayer);
		pos2D_set(displaced->pos, pos2D_getX(player->pos), pos2D_getY(player->pos));
	}
	//Move player to spot on map
	map_setPlayerPos(map, toPos, player);
	if (movedPlayer != -1) {
		//Update displaced on the map
		map_setPlayerPos(map, displaced->pos, displaced);
	}
	//Delete to Pos
	pos2D_delete(toPos);
    mem_free(addrP);
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
static pos2D_t* dirToMovement(pos2D_t* start, char dir, pos2D_t* mapWH){
	//Change Args
	if (start == NULL) {
		fprintf(stderr, "dirToMovement: Invalid Args passed");
		return NULL;
	}
	int x = pos2D_getX(start);
	int y = pos2D_getY(start);
	//Get direction and return a new pos that is the appropriate spot to move
	switch(dir) {
		case 'k'  :
			//UP
			y--;
      		break;
		case 'l'  :
			//RIGHT
			x++;
      		break;
		case 'j'  :
			//DOWN
			y++;
      		break;
		case 'h'  :
			//LEFT
			x--;
      		break;
		case 'u'  :
			//UPRIGHT
			y--;
			x++;
      		break;
		case 'n'  :
			//DOWNRIGHT
			y++;
			x++;
      		break;
		case 'y'  :
			//UPLEFT
			y--;
			x--;
      		break;
		case 'b'  :
			//DOWNLEFT
			y++;
			x--;
      		break;
		default : 
			fprintf(stderr, "shortMove: Invalid movement key");
			return NULL;
	}
	if(x < 0 || x >= pos2D_getX(mapWH)){
		return NULL;
	}
	if(y < 0 || y >= pos2D_getY(mapWH)){
		return NULL;
	}
	return mem_assert(pos2D_new(x, y), "dirToMovement: Pos Memory");
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
  // Check arg
  if (gameinfo == NULL) {
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
    mem_free(terminalSize);
    addr_t* playerP = mem_malloc_assert(sizeof(addr_t), "MEM: Join AddressCpy");
    *(playerP) = player;
    message = mem_malloc_assert(message_MaxBytes, "joinUser(): Mem Message");
   
    // ajust the players name if one is provided, check that it is not empty
    //      by empty we mean, doesn't contain graphical charcters
    int isValid = 0; 
    if (playerName != NULL) {
        if (strlen(playerName) > maxNameLength) {
            /* truncates the playername if its longer than 50 characters to exactly
            50 characters long. */
            playerName[50] = '\0';
        }
        
        /* replace any character for which isgraph() and isblank() are both false
        with an underscore */
        /* use this to check for spaces as well */
        for (int i = 0; i < strlen(playerName); i++) {
            if (isgraph(playerName[i]) == 0 && isblank(playerName[i]) != 0) {
                playerName[i] = '_';
            } else if (isspace(playerName[i]) != 0) {
                playerName[i] = '_';
            } else {
                isValid = 1;
            }
        }
    } else {
        isValid = 1;
    }
    
    // if they passed an invalid playername, quit them and return;
    if (isValid == 0) {
        sprintf(message, "QUIT Sorry - you must provide player's name.");
        message_send(player, message);
        mem_free(message);
        mem_free(playerP);
        return;
    }

  /* writes a message that'll be sent to the client to check the dimensions 
  of their window */
  sprintf(message, "GRID %d %d", nrows, ncols);

  // sends the GRID message to the client
  message_send(player, message);

  // if player name is not provided, add the user as a spectator
  if (playerName == NULL) {
		fprintf(stderr,"JOIN SPEC\n");
		gameInfo_addSpectator(gameinfo, playerP);
  } else {
		// add the user as a player
		fprintf(stderr,"JOIN USER: %s\n", playerName);
		// Check that not at max players
		if(gameInfo_getNumPlayers(gameinfo) >= maxPlayers){
			fprintf(stderr, "joinUser: Max Players Hit");
			message_send(player, "QUIT Already at Max Players");
			free(message);
			return;
		}
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
		playerInfo_t* playerinfo = gameInfo_addPlayer(gameinfo, playerP, pos, playerName);
        map_setPlayerPos(map,pos,playerinfo);
		//Create OK message
        free(message);
		message = mem_malloc_assert((sizeof(char) * 20) + 1, "joinUser(): Mem Message");
		 /* writes a message that'll be sent to the client to check the dimensions 
 		 of their window */
  		sprintf(message, "OK %c", playerinfo->playerID + 65);

  		// sends the OK message to the client
 		message_send(player, message);
		free(pos);
  	}
  // send the updated gameinfo to all clients.
  sendDisplays(gameinfo, message_noAddr(), 0);
  mem_free(message);
  #ifdef TESTING
  fprintf(stderr, "PLAYER COUNT - > %d\n", gameInfo_getActivePlayers(gameinfo));
  #endif
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

	// variables
  map_t* map;
  playerInfo_t* playerinfo;
  pos2D_t* pos;
  addr_t* playerP = mem_malloc_assert(sizeof(addr_t), "leaveUser: out of mem");
  *(playerP) = player;

  // get the map
  if ((map = gameInfo_getMap(gameinfo)) == NULL) {
    fprintf(stderr, "error: gameinfo provided is NULL.\n");
		exit(1);
  }	
  // gets the playerinfo struct
  if ((playerinfo = gameInfo_getPlayer(gameinfo, playerP)) == NULL) {
  	fprintf(stderr, "error: couldn't retrieve playerinfo struct.\n");
    exit(2);
  }
  //Check if it is a spectator
  if (playerinfo->username != NULL) {
		// gets the position of the player that will be cleared on the map
		pos = playerinfo->pos;
		// clears the spot on the map
		map_clearSpot(map, pos);

		char msgBuffer[message_MaxBytes];
		sprintf(msgBuffer, "QUIT Thanks for Playing!");
		#ifdef TESTING
		fprintf(stderr, "%s\n", msgBuffer);
		#endif

		message_send(*(playerinfo->address), msgBuffer);
		// player is removed from gameinfo
		gameInfo_removePlayer(gameinfo, playerP);
		// checks to see if the last player has left the server
		if (gameInfo_getActivePlayers(gameinfo) == 0) {
			printf("Server Ended: Active Players All Gone\n");
			endGame(gameinfo);
            mem_free(playerP);
			return true;
		}
  } else {
		char msgBuffer[message_MaxBytes];
		sprintf(msgBuffer, "QUIT Thanks for Watching!");
		#ifdef TESTING
		fprintf(stderr, "%s\n", msgBuffer);
		#endif

		message_send(*(playerinfo->address), msgBuffer);
		// Spectator is removed from gameinfo
		gameInfo_removeSpectator(gameinfo);
  }
  // send the updated display to all players
  sendDisplays(gameinfo, message_noAddr(), 0);

  #ifdef TESTING
  fprintf(stderr, "PLAYER COUNT - > %d\n", gameInfo_getActivePlayers(gameinfo));
  #endif
	mem_free(playerP);
	return false;
}

/******************* sendDisplays *********************
Send visible map (get it via the maps getVisibleMap) and the relevant 
information that's displayed at the top of the screen. To all players
Caller Provides:
	A gameInfo to use for the header and to get the map from and to get the 
	playerInfos from
We Do:
	Send each player a line containing their current nuggets and the nuggets 
	left to collect in the game with their visable map contained below it. To get
	the visible map we use the sightmaps from each playerinfo struct to combine
	into get VisibleMap
*/

static void sendDisplays(gameInfo_t* gameinfo, addr_t addr, int goldCollected)
{
	// Check Args
	if (gameinfo == NULL) {
		fprintf(stderr, "sendDisplays: Provided gameInfo is NULL");
		return; 
	}

	//Make space for the message
	char msgBuffer[message_MaxBytes];

	// Get Gold Score Remaining
	int scoreLeft = gameInfo_getScoreRemaining(gameinfo);
	//Loop through IDs for players
	for (int i = 0; i < maxPlayers + 1; i++) {
		playerInfo_t* player = NULL; 
		if ((player = gameInfo_getPlayerFromID(gameinfo, i)) != NULL) {
			//Check if Spectator
			if ((player->username) == NULL) {
				//Create the header message for spectator
				sprintf(msgBuffer, "GOLD 0 -1 %d", scoreLeft);
			} else {
				//Check if it was this player that collected gold
				#ifndef TESTING
				if (message_eqAddr(addr, *(player->address))) {
					#endif
					// Create the header message for collecting player
					sprintf(msgBuffer, "GOLD %d %d %d", goldCollected, (player->score), scoreLeft);
					#ifndef TESTING
				} else {
					// Create the header message for non-collecting player
					sprintf(msgBuffer, "GOLD 0 %d %d", (player->score), scoreLeft);
				}
				#endif
				if (!gameInfo_updateSightGrid(gameinfo, (player->address))) {
					fprintf(stderr, "sendDisplays: SightGrid update failed");
				}
			}

			//Send the displayHeader message
			#ifdef TESTING
			fprintf(stderr, "%s\n", msgBuffer);
			#endif

			message_send(*(player->address), msgBuffer);
			// Get visible map from player
			grid_t* seen = map_getVisibleMap(gameInfo_getMap(gameinfo), player->sightGrid);

			// Create Display message
			char* stringOfSeen = grid_toString(seen);
			char* displayMsg = mem_malloc_assert(sizeof(char) * (strlen(stringOfSeen) + strlen("DISPLAY\n") + 1), "sendDisplays: mem for display msg failed");
			sprintf(displayMsg, "DISPLAY\n%s", stringOfSeen);
			#ifdef TESTING
			fprintf(stderr, "%s\n", displayMsg);
			#endif

			message_send(*(player->address), displayMsg);
			mem_free(stringOfSeen);

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
void endGame(gameInfo_t* gameinfo)
{
  fprintf(stderr, "END GAME CALLED\n");
  // Check args
  if (gameinfo == NULL) {
		fprintf(stderr, "endGame: Invalid gameinfo struct passed.\n");
		return;
  }
	
	// variables
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
  message = mem_malloc_assert(message_MaxBytes, "endGame(): Mem message");

  // constructs the quit message sent to the user
  sprintf(message, "QUIT %s", scoreboard);
  mem_free(scoreboard);

  // loop over all the playerIDs and disconnect all of the players
  for (int i = 0; i < maxPlayers + 1; i++) {
    if ((player = gameInfo_getPlayerFromID(gameinfo, i)) == NULL) {
      // error occurred or a player does not exist at this index
      continue;
    }
    // grab the player address
		if (player->address == NULL) {
			fprintf(stderr, "Address is NULL");
			continue;
		}
    playerAddress = *(player->address);
    // send the quit message to the specific player
		#ifdef TESTING
		fprintf(stderr, "%s\n", message);
		#endif
    message_send(playerAddress, message);
  }
  mem_free(message);
}
