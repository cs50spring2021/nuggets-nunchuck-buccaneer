/*
 * network.c - CS50 'network' module
 *
 * see network.c for further information
 *
 * nunchuck_buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "network.h"
#include "mem.h"
#include "pos2D.h"
#include "file.h"
#include "message.h"
#include "serverCmds.h"
#include "clientCmds.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

typedef struct loopArgs {
  gameInfo_t* gameinfo;
  char* playerID;
  addr_t* addy;
} loopArgs_t;

/**************** global types ****************/


/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
bool str2int(const char string[], int* number);
static int numSlots(char** arr);

/**************** startNetworkServer() ****************/
/* see network.h for description */
void
startNetworkServer(gameInfo_t* gameInfo, FILE* errorFile)
{
  int port = 0;
  // initalizes the message server
  if ((port = message_init(errorFile)) == 0) {
    // error occurred while initalizing the server
    fprintf(stderr, "error: issue encountered while initializing the "
                       "server\n");
    exit(1);
  }
  printf("Ready to Play, waiting at port %d\n", port);
  //Create args struct for loop
  loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "startNetworkServer(): Mem Error for args");
  args->gameinfo = gameInfo;
  args->playerID = "$";
  /* responsible for the bulk of server communication, handles input messages,
   looping until an error occurs or is told by the handler to terminate. */
  if (!message_loop(args, 0, NULL, handleInput, handleMessage)) {
    // message_loop is false: a fatal error stopped it from continuing to loop.
    fprintf(stderr, "error: a fatal error occurred while looping.\n");
    exit(2);
  }
  message_done();
  mem_free(args);
}

/**************** startNetworkClient() ****************/
/* see network.h for description */
void
startNetworkClient(char* serverHost, int* port, FILE* errorFile, char* name)
{
  addr_t* serverAddress;
  char* message;            // the initial join message sent to the server

  // allocating memory for the two variables
  serverAddress = mem_malloc_assert(sizeof(addr_t), "startNetworkClient(): Mem Server Address");
  //Create args struct for loop
  loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "startNetworkServer(): Mem Error for args");
  args->playerID = mem_malloc_assert(sizeof(char), "startNetworkServer(): Mem error id");
  args->gameinfo = NULL;
  args->addy = serverAddress;
  *(args->playerID) = '@';

  // allocate memory for the message the client sends to the server
  message = mem_malloc_assert(message_MaxBytes, "startNetworkClient(): Mem Message");

  // if name is NULL, the user joining is a spectator.
  if (name == NULL) {
    // constructs the "SPECTATE" message
    sprintf(message, "SPECTATE");
  } else {
    // constructs the "PLAY" message with the user name included
    sprintf(message, "PLAY %s", name);
  }

  // initalizes the message server
  if (message_init(errorFile) == 0) {
    // error occurred while initalizing the client's connection
    fprintf(stderr, "error: issue encountered while initializing the"
                       " client's connection\n");
    exit(3);
  }

  //Convert port to string
  char portStr[10];
  sprintf(portStr, "%d", *port);
  fprintf(stderr, "Got: %s\n", portStr);

  if (!message_setAddr(serverHost, portStr, serverAddress)) {
    fprintf(stderr, "error: issue encountered likely due to a bad hostname or"
                    " port number\n");
    quitClient("Bad port or Hostname");
    exit(4);
  }
  if(!message_isAddr(*serverAddress)){
    fprintf(stderr, "error: Not an address\n");
    quitClient("Filed to Connect");
    exit(5);
  }
  // user joins the server
  message_send(*serverAddress, message);
  /* responsible for the bulk of server communication, handles input messages,
   looping until an error occurs or is told by the handler to terminate. */
  if (!message_loop(args, 0, NULL, handleInput, 
                    handleMessage)) {
    // message_loop is false: a fatal error stopped it from continuing to loop.
    fprintf(stderr, "error: a fatal error occurred while looping.\n");
    exit(5);
  }

  message_done();
  mem_free(message);
  mem_free(serverAddress);
}

/**************** tokenizeMessage() ****************/
/* see network.h for description */
char**
tokenizeMessage(char* message)
{
  char** tokens = NULL;      // the array that stores the words
  char* word = message;       // used to split up words (stays at front of word)
  char* rest = message;       // used to split up words (goes to end of word)
  int i = 0;

  // allocates memory for the array (tokens array has a max of 4 indices)
  tokens = mem_malloc_assert(4 * sizeof(char*), "error: issue "
              "encountered while allocating memory for the array.\n");
  for (int i = 0; i < 4; i++) {
    tokens[i] = NULL;
  }
  
  /* this loop is used to read the string and break it into its individual
  words. It separates words by spaces and also looks out for null characters.
  To separate the words from one another, it inserts null characters at the
  end of a word. Borrowed some of the syntax from Alan Moss' Querier */
  while (word[0] != '\0' ) {
    rest = word;
    // brings rest to the same spot as word
    while (!isspace(*rest) && *rest != '\0') {
      rest++;
    }
    if (*(rest) == '\n') {
      // this conditional is specifically for DISPLAY messages.
      *rest = '\0';
      tokens[i] = word;
      i++;
      rest++;
      word = rest;
      tokens[i] = word;
      return tokens;
    }
    if (*rest == '\0') {
      tokens[i] = word;
      break;
    }
    /* sets char to a null character, distinguishing the series of characters 
    before the null char as its own word */
    *rest = '\0';
    // stores the word into the array
    tokens[i] = word;
    word = rest;
    i++;
    // go forward a character
    word++;
    
    /* stops parsing the string if the first word parsed is "QUIT". The rest 
    of the string just goes into the 2nd slot in the array (1st). */
    if ((strcmp(tokens[0], "QUIT")) == 0) {
      tokens[1] = word;
      return tokens;
    }
    /* stops parsing the string if the first word parsed is "PLAY". The rest of 
    the string just goes into the 2nd slot in the array (1st). */
    if ((strcmp(tokens[0], "PLAY")) == 0) {
      tokens[1] = word;
      return tokens;
    }
  }
return tokens;
}

/**************** handleMessage() ****************/
/* see network.h for description */
bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  gameInfo_t* gameinfo;

  char** tokens;
  loopArgs_t* argumentStruct = arg;
  gameinfo = argumentStruct->gameinfo;
  char playerID = *(argumentStruct->playerID);

  // breaks a part the message into its individual parts
  char* copiedMessage = mem_malloc_assert(message_MaxBytes, "handleMessage(): Mem message Copy\n");
  strcpy(copiedMessage, message);
  tokens = tokenizeMessage(copiedMessage);
  // look at the first (0th) slot in each array to see what the command is
  if ((strcmp(tokens[0], "PLAY")) == 0) {
    // if the command is "PLAY", send a message to server with username
    if (tokens[1] == NULL) {
      fprintf(stderr, "ERROR: player name must be provided\n");
      //Check if player is not a server
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }

    // join the user if a name has been sent
    joinUser(gameinfo, from, tokens[1]);
    mem_free(copiedMessage);
    mem_free(tokens);
    return false;
  }

  if ((strcmp(tokens[0], "SPECTATE")) == 0) {
    // if the command is "SPECTATE", send a join spectate message to the server
    joinUser(gameinfo, from, NULL);
    mem_free(copiedMessage);
    mem_free(tokens);
    return false;
  }

  if ((strcmp(tokens[0], "GRID")) == 0) {
    // server ensures the dimensions by sending its dimensions to the client.
    int nrows = 0; 
    int ncols = 0; 
    pos2D_t* pos2D = NULL;

   /*
    * check to see if tokens has 3 total args
    * make sure that int chars* are passed, not alphabetical chars*
    */
    if (numSlots(tokens) != 3) {
      fprintf(stderr, "ERROR: invalid number of args passed in\n");
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }

    // converts the string dimensions of the window into integers
    if (!str2int(tokens[1], &nrows) || !str2int(tokens[2], &ncols)) {
      fprintf(stderr, "ERROR: non-integer value passed for\n");
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }
    // str2int(tokens[1], &nrows);
    // str2int(tokens[2], &ncols);

    // dimension checking
    if (nrows < 0 || ncols < 0) {
      fprintf(stderr, "ERROR: negative value(s) passed for dimensions\n");
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }

    pos2D = pos2D_new(nrows, ncols);
    ensureDimensions(pos2D);
    mem_free(copiedMessage);
    mem_free(tokens);
    pos2D_delete(pos2D);
    return false;
  }

  if ((strcmp(tokens[0], "QUIT")) == 0) {
    // the server disconnects the client from the game.
    quitClient(tokens[1]);
    mem_free(copiedMessage);
    mem_free(tokens);
    mem_free(argumentStruct->playerID);
    mem_free(argumentStruct);
    return true;
  }

  if ((strcmp(tokens[0], "OK")) == 0) {
    // the server was successfully added to the game, do nothing
    *(argumentStruct->playerID) = *(tokens[1]);
    mem_free(copiedMessage);
    mem_free(tokens);
    return false;
  }

  if ((strcmp(tokens[0], "KEY")) == 0) {
    // check to ensure only two args
    if (numSlots(tokens) != 2) {
      fprintf(stderr, "ERROR: invalid number of arguments passed\n");
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }
    // check for quit message
    if ((strcmp(tokens[1], "Q")) == 0) {
      mem_free(copiedMessage);
      mem_free(tokens);
     return leaveUser(gameinfo, from);
    } else {
      char validKeystrokes[] = {'h', 'l', 'k', 'j', 'y', 'u', 'b', 'n', 'H', 'L', 'K',
                    'J', 'Y', 'U', 'B', 'N', 'Q'};
      // error checking: see if keystroke is valid (in above array of 17 chars)
      bool valid = false;
      for (int i = 0; i < 17; i++) {
        if (*(tokens[1]) == validKeystrokes[i]) valid = true;
      }

      if (!valid) {
        fprintf(stderr, "ERROR: invalid keystroke passed\n");
        if(playerID != '$'){
          displayAction("Malformed Message");
        }
        mem_free(copiedMessage);
        mem_free(tokens);
        return false;
      }
      bool out = movePlayer(gameinfo, from, *(tokens[1]));
      // sends a single-character keystroke typed by the user to the server.
      mem_free(copiedMessage);
      mem_free(tokens);
      return out;
    }
  }

  if ((strcmp(tokens[0], "DISPLAY")) == 0) {
    /* server sends the display of the textual representation of the grid to
    the clients */
    display(tokens[1]);
    mem_free(copiedMessage);
    mem_free(tokens);
    return false;
  }

  if ((strcmp(tokens[0], "GOLD")) == 0) {
    int n = 0;
    int p = 0;
    int r = 0;

    // check number of args
    if (numSlots(tokens) != 4) {
      fprintf(stderr, "ERROR: invalid number of arguments passed\n");
      if(playerID != '$'){
        displayAction("Malformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }

    // check str2int on all 3 ints sent in
    if (!str2int(tokens[1], &n) || !str2int(tokens[2], &p) || 
                                      !str2int(tokens[3], &r)) {
      fprintf(stderr, "ERROR: non-Integer values input\n");
      if(playerID != '$'){
        displayAction("alformed Message");
      }
      mem_free(copiedMessage);
      mem_free(tokens);
      return false;
    }
    // str2int(tokens[1], &n);
    // str2int(tokens[2], &p);
    // str2int(tokens[3], &r);

    displayHeader(n, p, r, *(argumentStruct->playerID));
    mem_free(copiedMessage);
    mem_free(tokens);
    return false;
  }
  // the message received was malformatted
  fprintf(stderr, "ERROR: msg received was malformatted, ignoring the msg.\n");
  if(playerID != '$'){
    displayAction("Malformed Message");
  }
  mem_free(copiedMessage);
  mem_free(tokens);
  return false;
}

/**************** handleInput() ****************/
/* see network.h for description */
bool
handleInput(void* arg) {
  if (arg != NULL) {

    // character array for valid keystroke input.
    char array[] = {'h', 'l', 'k', 'j', 'y', 'u', 'b', 'n', 'H', 'L', 'K',
                    'J', 'Y', 'U', 'B', 'N', 'Q'};
    int arrayItems = 17;              // number of items in the above array
    int maxLengthMsg = 6;        // the max length of a key press msg
    char* message;
    loopArgs_t* args = arg;
    addr_t* address = args->addy;
    char* userID = args->playerID;

    message = mem_malloc_assert(maxLengthMsg + 1, "handleInput(): mem message");
    if (message == NULL) {
      fprintf(stderr, "error: issue encountered while allocating memory for"
      " the message that's sent to the server.\n");
      return true;
    }

    char key = '\0';
    key = fgetc(stdin);
    fprintf(stderr, "%c\n", key);
    if (feof(stdin)) {
        key = 'Q';
    }
    fprintf(stderr, "USERID: %c\n", *userID);
    if (*userID != '@') {
      int knownKey = 0;     // tracks if we have found the key in array
      // loops over all of the valid keystrokes that can be inputted
      for (int i = 0; i < arrayItems; i++) {
        if (key == array[i]) {
          sprintf(message, "KEY %c", key);
          message_send(*address, message);
          knownKey = 1;
        }
      }
      // if the key is ot in the known keys array
      if (knownKey == 0) {
        // tell the user that the keystroke was unknown
        displayAction("unknown keystroke");
      }
    } else {
      /* userID is 25, which means the user is a spectator. The spectator is
      only allowed to input "Q"). */
      if (key == 'Q') {
        sprintf(message, "KEY %c", key);
        message_send(*address, message);
      }
    }
    free(message);  
  }
  // if a message wasn't sent, the key inputted was not a valid keystroke
  return false;
}

/* ***************** str2int ********************** */
/*
 * Convert a string to an integer, returning that integer.
 * Returns true if successful, or false if any error. 
 * It is an error if there is any additional character beyond the integer.
 * Assumes number is a valid pointer.
 * Borrowed from CS 50 Lecture Notes.
 */
bool
str2int(const char string[], int* number)
{
  char nextchar;
  return (sscanf(string, "%d%c", number, &nextchar) == 1);
}


/* ***************** numSlots ********************** */
/*
 * Checks the number of non-NULL slots in a char* array.
 * No need to do anything (no memory allocation)
 */
 static int
 numSlots(char** arr) 
 {
   int i = 0;
   for (int j = 0; j < 4; j++) {
    if (arr[i] != NULL) i++;
   }
   return i;
 }
