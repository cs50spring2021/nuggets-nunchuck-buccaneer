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
*server* runs the server and parses and receives Messages from Clients, sends relevant visible maps for all players. Handles all the GameLogic and initializes the map.
1. *main*, parses args, Seeds randomness initializes Game, start networkServer(MessageLoop)
2. *initializeGame*, placeGold, make Map
3. *movePlayer*, gets a direction and a player and does a move for that player and collects gold, true on all gold collected
4. *joinUser*, adds a player, calls ensureDimensions on player, and places them on the map also can add a user as a spectator if name is NULL
6. *leaveUser*, gets a player and removes them from the game
7. *sendDisplays*, send visible map (get it via getVisibleMap) and the relevant information that's displayed at the top of the screen. To all players.
8. *endGame*, disconnect everyone, give them Scoreboards

#### PLAYER MODULE
*player* sends keystrokes and join msg; Displays the visible map for players and the end game screen
1. *Main* Parses args, connects to the server, Start N Curses, start NetworkPlayer(MessageLoop)
2. *Display* Display what the server sends to the client
3. *DisplayHeader* Displays the string on the top of the screen, which provides relevant game information to the client.
4. *DisplayAction* A static function that writes an action message on the viewer given a char*
5. *EnsureDimensions* Called by join server and loops on player until they have the right dimensions
6. *QuitClient* Called if server finishes or player quits, called from server, returns true. Upon receiving a QUIT message the client shall exit curses, print the explanation followed by a newline, and exit.


And some helper modules that provide data structures:

1. *gameInfo* Struct holding number of gold piles left, amount of gold left, and a PlayerInfo array also holds the map struct
2. *grid* Struct to hold a map of characters that can be interacted with 2d coordinates to get and set. Has toString and parseFromString
3. *map* Struct to hold BaseGrid and GameState Grid, functions to send display to players, setPlayerPos, Place Gold, and a function to return a visible map from a sight grid
4. *playerInfo* Struct to hold player-specific info such as location (x and y coordinates), gold collected, playerID, and address.
5. *pos2D* Struct wrapper for x and y coordinates, has getX and getY also has Set function
6. *Visibility* Module that provides a function to check if one point is visible from another given a map.
7. *Network* Runs message.h and handles messages and timeouts, parses them and translates them to functions which must be called on the player

## Pseudo code for logic/algorithmic flow

### The server will run as follows:
```
parse the command line, validate parameters, initialize other modules
create gameInfo struct
initalizes the game
start network server
    get messages - parse them
    call appropriate functions on server
```

MovePlayer

```
getplayerpos and cmd/direction
if not a sprint
    use one shortMove
if sprint,
    loop through shortMove until shortMove false
    send new game state
if no more gold
    endgame
    return true
```

Helper ShortMove:
```
take move dir
find adjacent pos
if wall
    return false
if gold
	add Score to player
    use TakeOutGold to remove it
if another player
	save playerID
setPlayerPos to the pos (map SetPlayerPos)
if replaced another player
	setPlayerPos of other player to old pos (map SetPlayerPos)
return true
```

InitializeGame
```
read the map file
create two grid for that map
create the map struct
place gold in map
```
JoinPlayer
```
if player terminal size is not big enough,
    call JoinFail on client with size needed
    return early
if there is space for another player,
    Create a new playerInfo struct to insert into the array of players (stored in gameInfo struct)
    Assign the new player a location on the map and place them there
    Send all players the updated map
else,
    Send the player a rejection message
```

JoinSpectator
```	
if there is a current spectator,
    Kick them off the server
let the new spectator connect
send the player a join message
messageLoop
    receives display input from the server
    if spectator presses “q”,
        send a quit message to the server and disconnect spectator.
```

LeavePlayer
```
take Player off the map  (GameInfo removePlayer)
delete corresponding playerInfo
send new Game State (Map SendNewGameState)
if no more players,
    endGame
    return true
```

EndGame
```
construct a scoreboard (GameInfo)
send a scoreboard of all the gold collected and other relevant information to the players 
send a quit message to all of the players
close down the server
```

### The player will run as follows:
```
parse the command line args, validate parameters
connect to the server and send it an initialization message
start nCurses
starts NetworkPlayer (MessageLoop)
    display what the server sends the client
    send keystrokes to the server through the handleInput function in MessageLoop
try to call JoinPlayer on server
quit when the user presses “q” or when a quit message is received from the server
```

Display
```
take a string
print it with nCurses use addch(c) maybe wprintf() and move cursor
```
JoinFail
```
loop till terminal resized to size
try to join again
```	
## Major data structures

Helper modules provide all the data structures we need:

- *gameInfo* holds all the major data structures needed to run the game
- *array* playerIDs are the indices, the array stores playerInfo structs
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

### Division of work

In the interest of efficiency, it makes sense to have each person work on modules of the project that are co-dependent. This allows each individual team member to build knowlege on one section of the project, without needing to know the nitty gritty detailed of every module. It also streamlines the workflow, as each team member is responsible for writing the code that their future code is dependent on, ensuring that we spend little to no time waiting on others to finish modules.

When writing the modules, if functionality from one module is requisite for functionality in another, team members are to assume that the functions being borrowed from other modules do exactly as described without fail. This allows for individuals to complete their code in the absence of worrying about functionality on the part of the others, thus enhancing efficiency.

General Responsibilities
James: Visibility Module & Testing, Server Main, Move, displayHeader, leaveUser, & Server Testing
Will: Grid Module & Testing, Map Module & Testing, Various Functions in Player.c, Player EnsureDimensions
Spencer: GameInfo Module & Testing, Pos2D module, server endGame, Player Testing
Alan: Network Module & Testing, Server join User, display, display action