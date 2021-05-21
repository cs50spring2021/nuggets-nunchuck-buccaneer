# CS50 Nuggets
## Design Spec

In this document we reference the REQUIREMENTS.md and focus on the implementation-independent design decisions.
Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

As described in the [Requirements Spec](REQUIREMENTS.md), the nugget’s interface with the user is initially through the command line when the user creates the server or connects to the server. Once the game starts, the user interacts with the program with their keyboard.

```server
$ ./server map.txt seed
```

```client
$ ./client hostname port [playername]
```

The user also interacts with the game through the client side through stdin. Allowing the player to move about and quit the game. The server has no more user interaction after being called.

Check the [Requirements Spec](REQUIREMENTS.md) to see the specific keystrokes available to the client


## Inputs and outputs

*Input:* when creating the server, the user must provide the server the map text file. Once the game has started, the user provides input to the program exclusively through keystrokes from the keyboard to stdin. 

*Output:* the output is the map display updated to player movements and game events updated by the server and pushed out to all of the clients.

## Functional decomposition into modules

We anticipate the following modules or functions:

#### SERVER MODULE
  1. *server*, runs the server and parses and receives Messages from Clients, sends relevant visible maps for all players. Handles all the GameLogic and initializes the map.
  2. *Main*, Parses args, uses networkServer to start server, Initializes Game, start NetworkServer(MessageLoop)
  3. *InitializeGame*, placeGold, make Map
  4. *MovePlayer*, Gets a direction and a player and does a move for that player, true on gold collected
  5. *JoinPlayer*, Adds a player and places them on the map
  6. *JoinSpectator*, Adds a player as a spectator
  7. *LeavePlayer*, Gets a player and removes them from the game
  8. *EndGame*, Disconnect Everyone, Give them Scoreboards, Stop ServerNetwork

#### PLAYER MODULE
  1. *player*, sends keystrokes and join msg; Displays the visible map for players and the end game screen
  2. *Main*, Parses args, connects to the server, Send Join message, Start N Curses, start NetworkPlayer(MessageLoop), 
  3. *Display*, Display what the server sends to the client
  4. *JoinFail*, gets if a join failed and sends join message after resizing


And some helper modules that provide data structures:
#### HELPER MODULES
  1. *gameInfo*, Struct holding amount of gold, Gold hashtable, and PlayerInfo hashtables
  2. *grid*, Struct to hold a map of characters that can be interacted with 2d coordinates to get and set. Has toString and parseFromString
  3. *map*, Struct to hold BaseGrid and GameState Grid, functions to send display to players, setPlayerPos, Place Gold, and a function to return a visible map from a sight grid
  4. *playerInfo*, Struct to hold player-specific info such as location (x and y coordinates), gold collected, playerID, and address.
  5. *2dPoint*, Struct wrapper for x and y coordinates, has getX and getY also has Set function
  6. *Visibility*, Module that provides a function to check if one point is visible from another given a map.
  7. *networkClient*, Runs message.h and handles messages and timeouts, parses them and translates them to functions which must be called on the player
  8. *networkServer*, Runs message.h and handles messages and timeouts, parses them and translates them to functions which must be called on the server

## Pseudo code for logic/algorithmic flow

### The server will run as follows:
```
parse the command line, validate parameters, initialize other modules
Call InitializeGame()
Start Network Server
    Get Messages - Parse Them
    Call Appropriate Functions on server
```

MovePlayer

```
getplayerpos and cmd/direction
if not a sprint
    Use one shortMove
If sprint,
    Loop Through ShortMove until shortMove false
    Send new game state
If no more gold
    Endgame
    Return true
```

Helper ShortMove:
```
Take move dir
Find adjacent pos
If wall
    Return false
If gold
	Add Score to player
    use TakeOutGold to remove it
If another player
	Save playerID
SetPlayerPos to the pos (map SetPlayerPos)
If replaced another player
	SetPlayerPos of other player to old pos (map SetPlayerPos)
Return true
```

InitializeGame
```
Read the map file
Create two grid for that map
Create the Map struct
Place Gold in Map
```

JoinPlayer
```
if player terminal size is not big enough,
    call JoinFail on client with size needed
	return early
if there is space for another player,
	Create a new playerInfo struct in the hashtable of players (GameInfo addPlayer)
	Assign the new player a location on the map and place them there
	Send all players the updated map
else,
	Send the player a rejection message
```

JoinSpectator
```	
if there is a current spectator,
	Kick them off the server
Let the new spectator connect
Send the player a join message
MessageLoop
    receives display input from the server
    if spectator presses “q”
        send a quit message to the server and disconnect spectator.
```

LeavePlayer
```
Take Player off the map  (GameInfo removePlayer)
Delete corresponding playerInfo
Send new Game State (Map SendNewGameState)
If no more players,
	EndGame
	Return true
```

EndGame
```
Construct a scoreboard (GameInfo)
Send a scoreboard of all the gold collected and other relevant information to the players 
Send a quit message to all of the players
Close down the server
```

### The player will run as follows:
```
Parse the command line args, validate parameters
Connect to the server and send it an initialization message
Start nCurses
Starts NetworkPlayer (MessageLoop)
    Display what the server sends the client
    Send keystrokes to the server through the handleInput function in MessageLoop
Try to call JoinPlayer on server
Quit when the user presses “q” or when a quit message is received from the server
```

Display
```
Take a string
Print it with nCurses use addch(c) maybe wprintf() and move cursor
```
JoinFail
```
Loop till terminal resized to size
Try to join again
```	
## Major data structures

Helper modules provide all the data structures we need:

- *gameInfo* holds all the major data structures needed to run the game
- *hashtable* gold: mapping coordinates of gold piles to the amount of gold they hold
- *hashtable* players: maps player addresses to playerInfo structs
- *map* holds the base grid and gamestate grid along with various methods
- *grid* base grid: holds the original base map.txt 
- *grid* gamestate grid: hold the current state of the game 
- *grid* sight grids: holds what a particular player can see
- *playerInfo* struct to hold the information of a single player


## Testing plan

For testing, we plan to run the server from a local host as well as remotely, connecting multiple clients to test connection, as well as running the program.

A sampling of tests that should be run:
- Using the provided “bot mode” to test multiple players at once while running the server
- Test erroneous cases - such as using bot mode to connect a number of players over the player count and joining as a spectator once a spectator is already connected.
- Test player collisions during gameplay to ensure proper switching
- Test tunnels to make sure players can only see one space in front of them
- Test connecting from multiple clients remotely to make sure network connections are running as needed
- Check error logs to make sure all tests are run correctly → print stderr and stdout to testing.out while running the program
- Test multiple map sizes to make sure the program reads in map text files and assembles grid layouts correctly