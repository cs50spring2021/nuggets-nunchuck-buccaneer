/*
 * network.c - CS50 'network' module
 *
 * see network.c for further information
 *
 * nunchuck_buccaneers, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "network.h"
#include "mem.h"
#include "pos2D.h"
#include "ncurses.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/


/**************** global types ****************/


/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
bool str2int(const char string[], int* number);

/**************** startNetworkServer() ****************/
/* see network.h for description */
void
startNetworkServer(gameInfo_t* gameInfo, FILE* errorFile)
{
  int port = 0;
  int timeout = 10;
  // initalizes the message server
  if ((port = message_init(FILE* errorFile)) == 0) {
    // error occurred while initalizing the server
    fprintf(stderr, "error: issue encountered while initializing the "
                       "server\n");
    exit(1);
  }
  /* responsible for the bulk of server communication, handles input messages,
   looping until an error occurs or is told by the handler to terminate. */
  if (!message_loop(gameInfo, timeout, handleTimeout, handleInput, 
                    handleMessage)) {
    // message_loop is false: a fatal error stopped it from continuing to loop.
    fprintf(stderr, "error: a fatal error occurred while looping.\n");
    exit(2);
  }

  message_done();
}

/**************** startNetworkClient() ****************/
/* see network.h for description */
void
startNetworkClient(char* serverHost, char* port, FILE* errorFile, char* name)
{
  addr_t serverAddress;
  int timeout = 10;
  char* message;            // the initial join message sent to the server

  // allocating memory for the two variables
  serverAddress = mem_malloc_assert(sizeof(adrr_t));
  if (serverAddress == NULL) {
    fprintf(stderr, "error: issue encountered while allocating memory for the"
                    " server address.\n");
    exit(1);
  }

  message = mem_malloc_assert((sizeof(char) * (5 + strlen(name))) + 1);
  if (message == NULL) {
    fprintf(stderr, "error: issue encountered while allocating memory for"
    " the message that's sent to the server.\n");
    exit(2);
  }

  sprintf(message, "PLAY %s", name);

  // initalizes the message server
  if ((port = message_init(FILE* errorFile)) == 0) {
    // error occurred while initalizing the client's connection
    fprintf(stderr, "error: issue encountered while initializing the"
                       " client's connection\n");
    exit(3);
  }
  if (!message_setAddr(serverHost, serverPort, &serverAddress)) {
    fprintf(stderr, "error: issue encountered likely due to a bad hostname or"
                    " port number\n");
    exit(4);
  }

  // user joins the server
  message_send(serverAddress, message);
  /* responsible for the bulk of server communication, handles input messages,
   looping until an error occurs or is told by the handler to terminate. */
  if (!message_loop(NULL, timeout, handleTimeout, handleInput, 
                    handleMessage)) {
    // message_loop is false: a fatal error stopped it from continuing to loop.
    fprintf(stderr, "error: a fatal error occurred while looping.\n");
    exit(5);
  }

  message_done();
  mem_free(message);
  mem_free(serverAddress);
}

/**************** numWords() ****************/
/* see network.h for description */
  /* scans and reads the message until a null character is encountered. It
  determines the number of words in the input line so that we can create 
  an array of the appropriate size later. */
int
numWords(char* message) {
  int numWords = 0;
  int i = 0;

  while (message[i] != '\0') {
    if (message[i+1] != '\0') {
      // detects a new line character
      if (message[i] == '\\' && message[i+1] == 'n') {
        i += 2;
        numWords++;
        continue;
      }
    }

    numWords++;
    /* scans through the word until it encounters a space, which signifies 
    the end of the word. Also makes sure it doesn't read a null character */
    while (!isspace(message[i]) && message[i] != '\0') {
      i++;
    }
  }
  return numWords;
}

/**************** tokenizeMessage() ****************/
/* see network.h for description */
char**
tokenizeMessage(const char* message, int numWords)
{
  char** tokens = NULL;      // the array that stores the words
  char* word = message;       // used to split up words (stays at front of word)
  char* rest = message;       // used to split up words (goes to end of word)
  int i = 0;

  // allocates space in memory for the array
  tokens = mem_malloc_assert(numWords * sizeof(char*), "error: issue "
              "encountered while allocating memory for the array.\n");
  

  /* this loop is used to read the string and break it into its individual
  words. It separates words by spaces and also looks out for null characters.
  To separate the words from one another, it inserts null characters at the
  end of a word. Borrowed this from Alan Moss' Querier */
  while (i < numWords) {
    // brings rest to the same spot as word
    rest = word;
    while (!isspace(*rest) && *rest != '\0') {
      if (*(rest+1) != '\0') {
        if (*rest == '\\' && *(rest+1) == 'n') {
          // this conditional is specifically for DISPLAY messages.
          rest += 2;
          word = rest;
          tokens[i] = word;
          return tokens;
        }
      }
      rest++;
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
    
    /* stops parsing the string after we parse the first word. The rest of the
    string just goes into the 2nd slot in the array (1st). */
    if ((strcmp(tokens[0], "QUIT")) == 0) {
      tokens[1] = word;
      return tokens;
    }

    /* stops parsing the string after we parse the first word. The rest of the
    string just goes into the 2nd slot in the array (1st). */
    if ((strcmp(tokens[0], "PLAY"))) == 0) {
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
  int numWords = 0;
  char** tokens;

  gameinfo = arg;

  numWords = numWords(message);
  // breaks a part the message into its individual parts
  tokens = tokenizeMessage(message, numWords);

  // look at the first (0th) slot in each array to see what the command is
  if ((strcmp(tokens[0], "PLAY")) == 0) {
    // if the command is "PLAY", send a message to server with username
    joinUser(gameinfo, from, tokens[1]);
    return false;
  }

  if ((strcmp(tokens[0], "SPECTATE")) == 0) {
    // if the command is "SPECTATE", send a join spectate message to the server
    joinUser(gameinfo, from, NULL);
    return false;
  }

  if ((strcmp(tokens[0], "GRID")) == 0) {
    // server ensures the dimensions by sending its dimensions to the client.
    int nrows = 0; 
    int ncols = 0; 
    pos2D_t* pos2D = NULL;

    // converts the string dimensions of the window into integers
    str2int(tokens[1], &nrows);
    str2int(tokens[2], &ncols);

    pos2D = pos2D_new(nrows, ncols);
    ensureDimensions(pos2D);
    return false;
  }

  if ((strcmp(tokens[0], "QUIT")) == 0) {
    // the server disconnects the client from the game.
    quitClient(tokens[1]);
    return false;
  }

  if ((strcmp(tokens[0], "OK")) == 0) {
    // the server was successfully added to the game, do nothing
    return false;
  }

  if ((strcmp(tokens[0], "KEY")) == 0) {
    if ((strcmp(tokens[1], "Q")) == 0) {
     return leaveUser(gameinfo, from);
    } else {
      // sends a single-character keystroke typed by the user to the server.
      movePlayer(gameinfo, from, tokens[1]);
      return false;
    }
  }

  if ((strcmp(tokens[0], "DISPLAY")) == 0) {
    /* server sends the display of the textual representation of the grid to
    the clients */
    display(tokens[1]);
    return false;
  }

  if ((strcmp(tokens[0], "GOLD")) == 0) {
    int n = 0;
    int p = 0;
    int r = 0;

    str2int(tokens[1], &n);
    str2int(tokens[2], &p);
    str2int(tokens[3], &r);

    displayHeader(n, p, r);
    return false;
  }

  // the message received was malformatted
  fprintf(stderr, "error: msg received was malformatted, ignoring the msg.")
  return false;
}

/**************** handleTimeout() ****************/
/* see network.h for description */
bool
handleTimeout(void* arg)
{
  if (arg != NULL) {
    quitClient("");
    return true;
  }

  // keep looping in message_loop
  return false;
}

/**************** handleInput() ****************/
/* see network.h for description */
bool
handleInput(void* arg);
{
  if (arg != NULL) {

    // character array for valid keystroke input.
    char array[] = {'h', 'l', 'k', 'j', 'y', 'u', 'b', 'n', 'H', 'L', 'K',
                    'J', 'Y', 'U', 'B', 'N', 'Q'};
    int arrayItems = 17;              // number of items in the above array
    char* message;
    addr_t* address = arg;

    message = mem_malloc_assert((sizeof(char) * 6) + 1);
    if (message == NULL) {
      fprintf(stderr, "error: issue encountered while allocating memory for"
      " the message that's sent to the server.\n");
      return true;
    }

    char key = '\0';
    getch(key);

    // loops over all of the valid keystrokes that can be inputted
    for (int i = 0; i < arrayItems; i++) {
      if (key == array[i]) {
        sprintf(message, "KEY %c", key);
        message_send(address, message);
        return false;
      }
    }
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

