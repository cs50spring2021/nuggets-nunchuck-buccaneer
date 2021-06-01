# CS50 Nuggets
## Implementation Spec

In this document we reference the REQUIREMENTS.md and DESIGN.md and focus on the implementation-specific decisions.
Here we focus on the following:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

The Nuggets program uses several data structures which are all held on the server side, The central data structure is the gameInfo data structure which holds all the information about an ongoing game such as the amount of gold and gold piles remaining, the map, and information about each of the players.

Information about each of the Players is held within a Transparent PlayerInfo struct that contains the player's sight grid, current position, score, their ID, address and username

The Map is a data structure within itself. It holds two Grid structs. One holds the base state of the game with original characters and the other holds the current gameState of the game that has all players and gold piles on it. Map has functions for handling any needed change to the visual part of the game including placing gold, moving player avatars, and providing a visible map from a sight grid

The Grid is just a 2D array of characters that each can be set and gotten. The entire grid can be gotten as a string as well which would make it immediately printable.

The Map contains two of these grids as already described and each playerInfo also contains a grid which has a series of characters that are 0, 1, and 2 that correspond to positions on the map. 0s represent unseen squares, 1s represent seen squares, and 2s represent squares that are currently being seen.

Throughout the entire project we also frequently use a helper pos2D struct that simply holds two integers to make it easy to transfer width and heights and 2D positions on the map.

## Control flow

The Nuggets Game is implemented in two main files `server.c` and `player.c`. Most of the Game logic is contained within `ServerCmds` and `ClientCmds` The game also uses several helper modules to support it.

## Server
### main
Psuedocode: 
```c
    * Check Num Args
    * Parse Args
    * Do Randomness initialization
    * Create game info and initialize game
    * start network server
    * Delete the gameinfo
```

### parseArgs
Psuedocode: 
```c
    * if seed is provided
        * get seed string
        * Get Seed int from string
            * Exit if not an int
        * Check if seed is negative
    * Get mapfile string
    * Open map file
        * if not openable exit
    * Close map file
    * return
```

### initializeGame
Psuedocode: 
```c
    * Check Args
    * Generate random number of gold piles
    * Create a gameInfo
    * Add gold piles to the map
    * return the gameInfo
```

## ServerCmds

### movePlayer
Psuedocode: 
```c
    * Check Args
    * Check if specator
        * return false if is
    * Create int = 0 for gold collected
    * Check if not sprint
        * Do one Short Move and update gold collected
    * else 
        * Loop Shortmoves until you cant and update gold collected
    * SendDisplays to all players with gold collected for the one player
    * Check for the end of the Game
        * End Game
        * return true
    * return false
```

### shortMove
Psuedocode: 
```c
    * Check args
    * Find the pos we need to go to with dirToMovement
    * Check if the pos is out of bounds
        * return false
    * Check if wall or empty space
        * return false
    * Check if gold
        * Pickup gold from goldInfo and to given goldCollected int the amount
    * Check for another player in spot (Capital letter)
        * Find the ID
        * Set the displaced player to the movers old spot
    * Move the mover to it's new pos on the map
    * if a player was displaced
        * Move it to the movers old pos on the map
    * Delete the position for where to go
```

### dirToMovement
Psuedocode: 
```c
    * Check Args
    * Get a dir and create a new pos to move to
    * return that pos
```

### joinUser
Psuedocode: 
```c
    * Check args
    * initalize variables nrows and ncols (int)
    * Allocate memory to create a message (char*)
    * extract the terminalSize from the ints stored in the pos2D struct and extract them into the nrows and ncols variables
    * construct a "GRID" message
    * send it to the client
    * Check if name is NULL for spectator
        * Call gameInfo add spectator
    * Else
        * call gameInfo_getMap()
        * call map_randomEmptySquare()
        * Call gameinfo add player
    * SendDisplays to all players
```

### leaveUser
Psuedocode: 
```c
    * Check args
    * Get the map from gameinfo
    * Call Map clear Spot
    * Call gameInfo remove Player
    * Send Displays to all players
    * Check if no players are left
        * return true
    * return false
```

### SendDisplays
Psuedocode: 
```c
    * Check args
    * Get gold score remaining
    * For each player
        * Get gold purse of the player
        * If player is the one who collected gold
            * Call displayHeader with their gold info
        * else
            * call displayHeader with gold collected = 0
        * Update sight grid with gameInfo update sight grid
        * use map get visible grid to get the grid in the players sight
        * Call display with their visible grid
```

### EndGame
Psuedocode: 
```c
    * Check args
    * use gameInfo to get the string representation of the scoreboard
    
    * construct a "QUIT" message to send to the user
    * for each player
        * Call quit client on the player, providing scoreboard
```

## Client
### main
Psuedocode: 
```c
    * Check number of args
    * parse args
    * call initscr(), initalizing ncurses screen
    * call startNetworkClient(), to initalize the client connection
```

## ClientCmds

### display
Psuedocode: 
```c
    * Check args
    * reset cursor to top left corner of screen 
    * create x and y int to represent cursor pos
    * Move cursor down a spot to skip header
    * for each row in display
        * for each column in display
            * set char to space in ncurses
    * while each of the new line segments in the string
        * while each of the chars in that segment
            * Take that spot read it from the string
            * Place the char in the nCurses
            * Move Cursor Right a spot
        * Move Cursor down a spot
        * Move cursor to x = 0
    * call refresh()
```

### displayHeader
Psuedocode: 
```c
    * Check args
    * Check if p == -1 for a spectator
        * Construct a header message "Player A has p nuggets (r nuggets unclaimed)."
    * else
        * Construct a header message "Spectator: r nuggets unclaimed."
    * If n is not 0
        * Call display action with "GOLD received: n"
    * reset cursor to top left corner of the screen
    * create x and y int to represent cursor pos
    * For each char in the header message
        * Print char in ncurses 
        * Move a char over to right
```

### displayAction
Psuedocode: 
```c
    * Check args
    * Get string length of the message, subtract 1 from it, and store it in a variable named charsLeft
    * initalize a char variable to store the current char
    * initalize an x and y variable to store the location the cursor is on the window
    * move cursor to top right corner of the screen
    * while charsLeft >= 0
        * index into the string and get the charsLeft character in the string
        * place that character where the cursor is
        * move the cursor over one to the left
        * decrement charsLeft by 1
```

### ensureDimensions
```c
    * initalize 2 pairs of nrows and ncols variables (one for game window size, one for client window size)
    * extract the pos2D struct passed to the function into the server nrows and ncols pair
    * call getmaxyx, providing stdscr as the first parameter and the nrowsClient and nrowsCols variables.
    * while the nrowsGame > nrowsClient or ncolsGame > ncolsClient
                * print message to clients screen: tell them they need to expand their window size (provide them with the necessary window size requirements)
        * call getmaxyx again to reassess the clients window size
```

### quitClient
```c
    * exit curses with endwin()
    * print the explanation passed in the quit message
    * exit main with zero;
```

## Other Modules

### pos2D
Psuedocode: 
```c
pos2D_new:
    * allocate memory to a new pos2D struct
    * set x and y values (if both > 0)
    * return new pos2D object
pos2D_delete:
    * free memory from parameterized pos2D object
pos2D_getX:
    * return stored x value
pos2D_getY:
    * return stored y value
pos2D_set:
    * set x and y values to parameterized x and y values
```
### grid
Psuedocode:
```c
struct grid
    * char* map;
    * int width;
    * int height;
grid_new:
    * check args
    * create a copy of the passed string
    * while not at the end of the map string
        * if the char at this position is '\n'
            * if height = 0
                * the width is the index of this charcter
            * incriment height
        * incriment the index
    * allocate memory for the grid
    * assign map, width, and hight to the grid struct
    * return grid
grid_delete:
    * check args
    * free the map string
    * free the gid
    * return
grid_getPos:
    * check args
    * check that pos holds valid x and y
    * check that the coordinates are inbounds
    * get the character at (width - 1) * y + x
    * if the character is a wall ('+', '-', or '|') make it into '+'
    * return the character
grid_setPos:
    * check args
    * check that the pos holds valid x and y
    * check that the coordinates are inbounds
    * set the character at (width -1) * y + x to the passed character
    * return
grid_toString:
    * check args
    * make a copy of the grids map
    * return the copy of the grids map
grid_getWidthheight:
    * checks args 
    * create a new pos2D
    * set the x coordinate to the width, the y to the height
    * return the pos2D
```
### map
```c
map_new:
    * check args
    * open the map file, reads its contents as a string, close it
    * allocate memory for the map
    * make grids for baseGrid and gameGrid from the the map string
    * return the map
map_getBaseGrid:
    * check args
    * return a pointer to the maps baseGrid
map_getVisibleMap:
    * check args
    * check that the grids in map are the same dimensions as the visibleGrid
    * open three grids as strings: baseString, gameString, visibleString
    * start an outString
    * for each character in sightString at index i
        * if it is a new line'\n', set index i in outString to '\n'
        * else if it is a '0', set index i in outString to a' '
        * else if it is a '1', set index i in outString equal to index i in baseString
        * else if it is a '2', set index i in outString equal to index i in gameString
        * else, print an error message with the unexpected character
    * cap the outString with a terminator character '\0'
    * free open copies of strings
    * return an outGrid made from outString
map_getGamePos:
    * check args
    * get the character at pos from gameGrid 
    * return the character
map_setPlayerPos:
    * check args
    * set the new pos on gameGrid to the playerID
    * save the old playerPos, set the playerPos to the new pos
    * if the old player pos is NULL, do nothing
    * else if the character at the old player pos is the players ID char, set it back to the baseGrid character
    * else if the character at the old player pos is NOT the players ID char, leave it be
    * return;
map_clearSpot:
    * check args
    * get the char at the given pos in baseGrid
    * if the char returned is NULL, return
    * set the pos in gameGrid to the char from baseGrid
    * return;
map_randomEmptySquare:
    * check args
    * take the modulus of a random large integer with the length of the gameString
    * while the character at the random integer in gameGrid is NOT equal to '.'
        * get another random index 
    * turn the index integer into x and y coordinates using the gridWidth of gameGrid
    * return a pos2D with the x and y of the open spot
map_putOneGold:
    * check args
    * call map_randomEmptySquare to get an empty square
    * set the character at the random pos to the gold symbol '*'
    * return
map_getWidthHeight:
    * check args
    * get a pos with width and height from gameGrid
    * return that pos
map_delete:
    * check args
    * call grid_delete on both grids held by map
    * return;
map_clearSpot:
    * get the pos on the baseGrid
    * Set the pos on the gameState Grid to the base Grid
    * return;
```

### visibility
Psuedocode: 
```c
visibility_getVisibility:
    * check args
    * Get reversed points for X and Y axis
    * Check the verticle walls for the original points and the reversed axis points
    * if both are visible then point is visible
    * delete reversed points
    * return visibility
checkLineVerticles - helper for visibility checks if intersects verticle walls:
    * Find the slope of the line
    * Check if point is directly above
        * No sight block if it is
    * Skip the origin pos
    * Loop towards endx from start x until 1 away from the end
        * Find the Y of the line for that x
        * Checks if hit an exact square
            * Create a new point to hold pos of intersection
            * Flip the point if flip is on
            * Get the char for that point
            * delete point
            * check if char is room tile
                * if not block sight
        * if passes between positions
        * Create new pos for above and below the y for x
        * Flip the points depending on flip bool
        * Get characters for over and under pos
        * delete points
        * if both points not room tiles block sight
        * move to next x
    * if get here sight is clear

```

### gameInfo
```c
gameInfo_newGameInfo:
    * check args
    * allocate memory for the gameInfo struct
    * allocate memory for MaxPlayers playerInfo's in players array
    * allocate memory for MaxPlayers playerInfo's in removedPlayers array
    * set goldPiles to number of piles passed in
    * set goldScore to the passed in score
    * set numPlayers to 0
    * set inactivePlayers to 0
    * set map to a new map with the passed in mapfile
    * set maxPlayers to MaxPlayers passed in
    
gameInfo_addPlayer:
    * check args
    * allocate memory and set parameterized values to their corresponding attributes
    * if the number of players in the game is less than 25:
        * set the playerID to the first empty spot in the array
    * else: 
        * free player memory
        * return false
    * grab a string of the baseGrid from the map passed in and initialize the sightGrid string in memory
    * step through each char in the mapString:
        * if the spot is a '\n', set that value to '\n' in the sightGrid string
        * if the spot is a '\0', set that value to '\0' (end of the mapString)
        * else set the spot to '0' (cannot see anything)
    * set the players sightGrid, calling grid_new on the string
    * take the sightGrid, looping through each char in the string
        * if visible, set the value to '2'
        * if not visible, leave at '0'
    * add the player to the players list and increment the number of players, returning true
    
gameInfo_addSpectator:
    * check args
    * allocate memory and set parameterized values to their corresponding attributes
    * set the position to NULL, score to -1, ID to 25, and username to NULL
    * grab the mapString from the base grid and allocate memory for a string to hold the basegrid string
    * step through each char in the mapString:
        * if the spot is a '\n', set that value to '\n' in the sightGrid string
        * if the spot is a '\0', set that value to '\0' (end of the mapString)
        * else set the spot to '2' (can see everything)
    * set the players sightGrid, calling grid_new on the string
    * see if there is already a current spectator, removing them if so
    * set the spectator to our new spectator
        
gameInfo_removePlayer:
    * check args
    * move the player from the players array to removedPlayers array
    * note: do NOT change numPlayers, as we only accept *MaxPlayers* total
    * increment number of inactive players in the game
    
gameInfo_deletePlayer:
    * check args
    * grab the player to remove
    * set the players spot in the players array to NULL, decrement the number of players, freeing the player stored in memory along with any other necessary memory
    
gameInfo_removeSpectator:
    * check args
    * free the spectator and all of its attributes from memory
    * set the spectator's value in the players array to NULL

gameInfo_getPlayer:
    * check args
    * go through the number of players in players:
        * if the address of a player matches the address passed in, return that player
    * if no player was found return NULL
    
gameInfo_getSpectator:
    * check args
    * return the last member of the players array (NULL if no spectator)

gameInfo_getPlayerFromID:
    * check args
    * go through the number of players in players:
        * if the ID of the player matches the ID passed in, return that player
    * if no player was found return NULL

gameInfo_pickupGold:
    * check args
    * grab a random amount of gold left, unless 1 pile left (use that pile's amount)
    * grab the player w passed in address, incrementing their score by the amount of gold found
    * decrement gold piles and amt of gold left
    * return the amount of gold picked up

gameInfo_createScoreBoard:
    * add each player to the scoreboard list and sort the list by score (highest to lowest)
    * print each players score out to the scoreboard line char* and return the scoreboard line
    
gameInfo_getScoreRemaining:
    * check args
    * return the score remaining stored in gameInfo

gameInfo_getMap:
    * check args
    * return the map held in gameInfo
    
gameInfo_getNumPlayers:
    * check args
    * return the number of players held in gameInfo

gameInfo_getActivePlayers:
    * Check args
    * return num players - inactive players

gameInfo_updateSightGrid:
    * check args
    * grab the sightGrid string from the player
    * go through from their current position
        * set values to 2 if seeable
        * otherwise set to 1 if previously a 2 and 
        * change nothing otherwise
    * return true if completed, false otherwise

gameInfo_getGoldPiles:
    * check args
    * return goldPiles held in gameInfo

gameInfo_delete:
    * check args
    * delete all of the players in the players array
    * delete the spectator and players array
    * delete all of the players in the removedPlayers array
    * delete the removedPlayers array
    * free the map, and, finally, the gameInfo struct
```


### network
```c
startNetworkServer:
    * call message_init, which returns the port
    * call message_loop, starting the loop which checks for messages from the client
    * call message_done
startNetworkClient:
    * allocate memory for a addr_t struct (serverAddress) and char* (message)
    * create the initial play message thatll be sent to the server
    * call message_init, which returns the port
    * call message_setAddr, which sets the address of the server that the client will send messages to
    * call message_send, sending the initial join message to the server
    * call message_loop, starting the loop which checks for messages from the server
    * call message_done;
    * free the message and server address from memory
tokenizeMessage:
    * initalize variables needed (specifically set word and rest to the first character in the message)
    * allocate memory for an array that will hold the tokens
    * while loop that runs until a null (terminating) character is encountered
       * set rest equal to word
       * while rest is not a space and is not a null character
            * if the char (rest + 1) is a null char
                * if the current and next char combine to make a new line char (\n)
                    * move rest over 2 characters
                    * set word equal to rest
                    * set slot i in the tokens array equal to word
                    * return tokens
            * increment rest by 1
        * set the current rest char to a null char
        * set word equal to rest 
        * increment i by 1
        * move word to the next char over
        * if the first word (slot 0) in the array is "QUIT"
            * set slot 1 in the array equal to the rest of the string
            * return the array
        * if the first word (slot 0) in the array 
            * set slot 1 (not slot 0) in the array equal to the rest of the string
            * return the array
    * return the tokens array
handleMessage:
    * call numWords() to count the number of words in the message
    * call tokenizeMessage to break apart the message into its individual parts, store the returned array in tokens
    * if the 0th index of the array is the word "PLAY"
        * call joinUser()
        * return false
    * if the 0th index of the array is the word "SPECTATE"
        * call joinUser(), providing NULL to the player name argument
        * return false
    * if the 0th index of the array is the word "GRID"
        * initalize a nrows, ncols, and pos2D struct
        * convert the string stored in the 1st (nrows) and 2nd (ncols) index of the array into integers
        * call pos2D_new(), creating a new pos2D struct and providing the int nrows and ncols to the function
        * call ensureDimensions(), checking that the dimensions of the user's window are valid
        * return false
    * if the 0th index of the array is the word "QUIT"
        * call quitClient()
        * return false
    * if the 0th index of the array is the word "OK"
        * return false
    * if the 0th index of the array is the word "KEY"
        * call movePlayer(), providing the char stored in the 1st index of the array for the key stroke argument
        * return false
    * if the 0th index of the array is the word "DISPLAY"
        * call display(), providing the 1st index of the array as the argument that provides the display.
        * return false
    * if the 0th index of the array is the word "GOLD"
        * initalize variables for different information about the gold
        * convert the strings stored in the 1st, 2nd, and 3rd indices in the array into integers
        * call displayHeader()
        * return false
    * print an error message indicating that the message was malformatted
    * return false
handleInput:
    * initalize a character array with all the possible valid keystroke input (18 characters)
    * initalize the other variables
    * allocate memory for a message (string)
    * use scanf to read in the users keystroke input from stdin and store it in a char variable
    * for each valid keystrokes in the array
        * if the key inputted from the user is equal to the keystroke in the array (if its valid input)
            * create a "KEY" message
            * call displayAction on Client with "Key c" 
            * call message_send() to send the key input message to the server
            * return false
    * call displayAction on client with "Unknown Keystroke"
    * return false
```

## Function prototypes

### Server
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `server.c` and is not repeated here.

```c
int main(const int argc, char *argv[]);
static void parseArgs(const int argc, char *argv[], char** mapFilePath, int* seed);
static gameInfo_t* initializeGame(char* mapFile);
```

### ServerCmds
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's prototype in `serverCmds.h` and is not repeated here.

```c
bool movePlayer(gameInfo_t* gameinfo, addr_t* player, char input);
static bool shortMove(gameInfo_t* gameinfo, addr_t* player, char* dir, int* goldCollected);
static pos2D_t* dirToMovement(pos2D_t* start, char dir){
void joinUser(gameInfo_t* gameinfo, addr_t* player, char* playerName);
bool leaveUser(gameInfo_t* gameinfo, addr_t* player);
void sendDisplays(gameInfo_t* gameinfo, addr_t* Player, int goldCollected);
void endGame(gameInfo_t* gameinfo);
```

### Client
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `client.c` and is not repeated here.
```c
int main(const int argc, char* argv[]);
```

### ClientCmds
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's prototype in `clientCmds.h` and is not repeated here.

```c
void display(const char* grid);
void displayHeader(int goldCollected, int goldInPurse, int goldRemaining);
void displayAction(char* action);
void ensureDimensions(pos2D_t* display_hW);
void quitClient(char* message);
```

## Helper Modules

### Network
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `network.h` and is not repeated here.
```c
void startNetworkServer(gameInfo_t* gameInfo, FILE* errorFile);
startNetworkClient(char* serverHost, char* port, FILE* errorFile, char* name);
char** tokenizeMessage(const char* message);
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);
bool str2int(const char string[], int* number);
```

### Visibility
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `visibility.h` and is not repeated here.
```c
bool visibility_getVisibility(pos2D_t* start, pos2D_t* end, grid_t* baseGrid);
```

### Pos2D
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `pos2D.h` and is not repeated here.
```c
pos2D_t* pos2D_new(int x, int y);
void pos2D_delete(pos2D_t* pos);
int pos2D_getX(pos2D_t* pos);
int pos2D_getY(pos2D_t* pos);
bool pos2D_set(pos2D_t* pos, int x, int y);
```

##### Struct pos2D:
- int x
- int y

### grid
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `grid.h` and is not repeated here.
```c
grid_t* grid_new(char* readString);
void grid_delete(grid_t* grid);
char grid_getPos(grid_t* grid, pos2D_t* pos);
void grid_setPos(grid_t* grid, pos2D_t* pos, char value);
char* grid_toString(grid_t* grid);
pos2D_t* grid_getWidthheight(grid_t* grid);
```
##### struct grid:
- char* mapFile
- int width
- int height

### map
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `map.h` and is not repeated here.
```c
map_t* map_new(char* mapFile);
grid_t* map_getBaseGrid(map_t* map);
grid_t* map_getVisibleMap(map_t* map, grid_t* sightGrid);
char map_getGamePos(map_t* map, pos2D_t* pos);
void map_setPlayerPos(map_t* map, pos2D_t* pos, playerInfo_t* player);
void map_clearSpot(map_t* map, pos2D_t* pos);
pos2D_t* map_randomEmptySquare(map_t* map);
void map_putOneGold(map_t* map);
pos2D_t* map_getWidthHeight(map_t* map);
void map_delete(map_t* map);
void map_clearSpot(map_t* map, pos2D_t* pos);
```
##### Struct map:
- grid_t* baseGrid
- grid_t* gameState

### gameInfo
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `gameInfo.h` and is not repeated here.
```c
gameInfo_t* gameInfo_newGameInfo(int piles, int score, char* mapFile, int maxUsers);
playerInfo_t* gameInfo_addPlayer(gameInfo_t* info, addr_t* address, pos2D_t* pos, char* username);
void gameInfo_addSpectator(gameInfo_t* info, addr_t* address);
void gameInfo_removePlayer(gameInfo_t* info, addr_t* address);
void gameInfo_deletePlayer(gameInfo_t* info, addr_t* address);
void gameInfo_removeSpectator(gameInfo_t* info);
playerInfo_t* gameInfo_getPlayer(gameInfo_t* info, addr_t* address);
playerInfo_t* gameInfo_getSpectator(gameInfo_t* info);
playerInfo_t* gameInfo_getPlayerFromID(gameInfo_t* info, int playerID);
int gameInfo_pickupGold(gameInfo_t* info, addr_t* address);
char* gameInfo_createScoreBoard(gameInfo_t* info);
int gameInfo_getScoreRemaining(gameInfo_t* info);
map_t* gameInfo_getMap(gameInfo_t* info);
int gameInfo_getNumPlayers(gameInfo_t* info);
int gameInfo_getActivePlayers(gameInfo_t* info);
bool gameInfo_updateSightGrid(gameInfo_t* info, addr_t* address);
int gameInfo_getGoldPiles(gameInfo_t* info);
void gameInfo_delete(gameInfo_t* info);
```

##### Struct gameInfo:
- playerInfo_t** players;
- playerInfo_t** removedPlayers;
- int goldPiles;
- int goldScore;
- int numPlayers;
- int inactivePlayers;
- map_t* map;
- int maxPlayers;
 
##### Struct playerInfo:
- grid_t* sightGrid;
- pos2D_t* pos;
- int score;
- int playerID;
- addr_t* address;
- char* username;


The player will run as follows


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally some will go all way back to main and cause an exit
Whereas others arent fatal and the program will attempt to keep running

Most errors involving parsing cmd line args will exit the program as will any problems connecting the server and the clients. Most other network errors are just ignored and the program keeps running with a printed error message. Any other issues are often caught by the individual modules and return to their callers error signifiying values liek NULL or -1

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

For each of the above-described modules, there will be individual testing files written. Some of the testing will also be conducted through the interaction of different modules (i.e., making sure the networking allows functionality for the gamestate, visibility, etc.).

Each testing file will be a .c driven test that will go through each of that modules functions and tests them for both edge cases and acutally doing their jobs, additionally each testing file will be run on Valgrind and that will allow us to ensure that there are no memeory leaks within a module. There will be a .c driven test files for Grid, Pos2D, Map, and GameInfo.

### Integration/system testing

We write programs `*module*test.c` that provide testing for each of the modules, if provided significant enough functionality from each. For example, playerInfo won't be tested with a test file, as its functionality will be clearly seen through its use in other modules.
Other example:
The `networktest.c` program checks the interactivity between clients and servers based on keystrokes sent through the terminal.

The unit testing will be completed with valgrind testing to ensure no memory leaks or errors are present in the modules.

In addition to testing all the Units individually we also want to test the callable functions on both server and client, functions like display/quit on client and functions like join user, leave user and move user on the server. To do this we have two additional testing drivers clienttest and servertest which call these functions on the server and client simply through their mains allowing testing without the hassle of having multiple terminals running. On the server it will basically be able to run a game without the help of the client and client will effectively be able to interact with a server without a server. These can also have valgrind ran on them to ensure no memory leaks. We can switch mains from client and server to their tests with simple ifdef macros.

After all the rest of the testing is complete we will do the networktest in order to make sure communication between server and client is actually working. This will also be run on valgrind to ensure no memory leaks on that module as well.

After running each of these individual tests we will also just screw around with the game to find missed errors or failures in game logic all around. Hopefully finding anything that remains after this extensive testing.