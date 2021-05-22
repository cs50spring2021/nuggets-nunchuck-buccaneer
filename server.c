/*

    server.c
    runs the server and parses and receives Messages from Clients, sends relevant visible maps for all players.
	Handles all the GameLogic and initializes the map.

	Nunchuck-Buccaneers
    CS50, 5/12/2021

*/

#include <stdio.h>
#include "pos2D.h"
#include "message.h"
#include "gameinfo.h"

static void parseArgs();


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
	if(argc != 3){
		fprintf(stderr, "Usage: ./server map.txt [seed]")
		exit(1);
	}
	// Parse the args
	parseArgs();
}

static void parseArgs(){
	
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
gameInfo_t* initializeGame(char* mapFile);

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
bool movePlayer(gameInfo_t* gameinfo, addr_t* player, char input);

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
	bool shortMove(gameInfo_t* gameinfo, addr_t* player, char dir);

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
void joinUser(gameInfo_t* gameinfo, addr_t* player, char* playerName, pos2D_t terminalSize);

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
bool leaveUser(gameInfo_t* gameinfo, addr_t* Player);

/******************* sendDisplays *********************
Send visible map (get it via the maps getVisibleMap) and the relevant information that's displayed at the top of the screen. To all players
Caller Provides:
	A gameInfo to use for the header and to get the map from and to get the playerInfos from
We Do:
	Send each player a line containing their current nuggets and the nuggets left to collect in the game with their visable map contained below 
	it. To get the visible map we use the sightmaps from each playerinfo struct to combine into get VisibleMap
*/
void sendDisplays(gameInfo_t* gameinfo);

/******************* endGame *********************
disconnect everyone, give them Scoreboards
Caller Provides:
	A gameInfo to update
We Do:
	Send a new display to each player that contains the endgame scoreboard constructed from gameinfo
	Send a quit message to all players
*/
void endGame(gameInfo_t* gameinfo);