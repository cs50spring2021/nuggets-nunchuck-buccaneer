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

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/


/**************** global types ****************/


/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */


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
  serverAddress = mem_malloc_assert(sizeof(serverAddress, "error: issue " 
  "encountered while allocating memory for the server address.\n"));
  message = mem_malloc_assert(sizeof(char*) * strlen(name)) + 1, "error: issue" 
  " encountered while allocating memory for the message that's sent to"
  " the server.\n");

  sprintf(message, "PLAY %s", name);

  // initalizes the message server
  if ((port = message_init(FILE* errorFile)) == 0) {
    // error occurred while initalizing the client's connection
    fprintf(stderr, "error: issue encountered while initializing the"
                       " client's connection\n");
    exit(1);
  }
  if (!message_setAddr(serverHost, serverPort, &serverAddress)) {
    fprintf(stderr, "error: issue encountered likely due to a bad hostname or"
                    " port number\n");
    exit(2);
  }

  message_send(serverAddress, message);
  /* responsible for the bulk of server communication, handles input messages,
   looping until an error occurs or is told by the handler to terminate. */
  if (!message_loop(NULL, timeout, handleTimeout, handleInput, 
                    handleMessage)) {
    // message_loop is false: a fatal error stopped it from continuing to loop.
    fprintf(stderr, "error: a fatal error occurred while looping.\n");
    exit(3);
  }

  message_done();
  mem_free(message);
  mem_free(serverAddress);
}

/**************** tokenizeMessage() ****************/
/* see network.h for description */
char**
tokenizeMessage(const char* message)
{
  char** tokens = NULL;      // the array that stores the words
  char* word = message;       // used to split up words (stays at front of word)
  char* rest = message;       // used to split up words (goes to end of word)
  int numWords = 0;
  int i = 0;

  /* scans and reads the message until a null character is encountered. It
  determines the number of words in the input line so that we can create 
  an array of the appropriate size later. */
  while (message[i] != '\0') {
    // if the character is a letter (first letter of the word)
    if (isalpha(message[i])) {
      numWords++;
      /* scans through the word until it encounters a space, which signifies 
      the end of the word. Also makes sure it doesn't read a null character */
      while (!isspace(message[i]) && message[i] != '\0') {
        i++;
      }
    } else {
      i++;
    }
  }
  // allocates space in memory for the array
  tokens = mem_malloc_assert(numWords * sizeof(char*), "error: issue "
              "encountered while allocating memory for the array.\n");
  

  /* this loop is used to read the string and break it into its individual
  words. It separates words by spaces and also looks out for null characters.
  To separate the words from one another, it inserts null characters at the
  end of a word. */
  i = 0;
  while (i < numWords) {
    // steps through the characters in the string until it finds a letter
    if (isalpha(*word)) {
      // brings rest to the same spot as word
      rest = word;
      while (!isspace(*rest) && *rest != '\0') {
        rest++;
      }
      /* sets char to a null character, distinguishing the series of characters 
      before the null char as its own word */
      *rest = '\0';
      // stores the word into the array
      tokens[i] = word;
      word = rest;
      i++;
    }
    // go forward a character
    word++;
  }
  return tokens;
}

/**************** handleMessage() ****************/
/* see network.h for description */
bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  
}

/**************** handleTimeout() ****************/
/* see network.h for description */
bool
handleTimeout(void* arg)
{
  
}

/**************** handleInput() ****************/
/* see network.h for description */
bool
handleInput(void* arg);
{
  
}

