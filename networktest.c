/*
 * networktest.c - test program for network.c
 *
 * Nunchuck-Buccaneers
 * 05/24/21
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "message.h"

/******************** local type *******************/
/* from network.c and for testing */
typedef struct loopArgs {
    gameInfo_t* gameinfo;
    char* playerID;
    addr_t* addy;
} loopArgs_t;

int main(){
  printf("Parse Test\n");
  char* message = mem_malloc_assert(100 * sizeof(char), "error: issue "
              "encountered while allocating memory for the message.\n");
  strcpy(message, "GRID 15 23 18");
  char** array = tokenizeMessage(message);

  printf("\nWords in the array:\n");
  // i is 4 because in this particular case we are testing a "GRID" message
  for (int i = 0; i < 4; i++) {
    printf("%s", array[i]);
    printf("\n");
  }
  free(array);

  printf("Parse Test 2\n");
  strcpy(message, "PLAY Alan and Friends");
  array = tokenizeMessage(message);

  printf("\nWords in the array:\n");
  // i is 4 because in this particular case we are testing a "GRID" message
  for (int i = 0; i < 2; i++) {
    printf("%s", array[i]);
    printf("\n");
  }
  printf("\n");
  free(array);

  printf("Parse Test 3\n");
  strcpy(message, "QUIT Game is full: no more players can join.");
  array = tokenizeMessage(message);

  printf("\nWords in the array:\n");
  // i is 4 because in this particular case we are testing a "GRID" message
  for (int i = 0; i < 2; i++) {
    printf("%s", array[i]);
    printf("\n");
  }
  printf("\n");
  free(array);

  printf("Parse Test 4\n");
  strcpy(message, "DISPLAY\nthisisthefirst line of the grid\nthis would be the second line");
  array = tokenizeMessage(message);

  printf("\nWords in the array:\n");
  // i is 4 because in this particular case we are testing a "GRID" message
  for (int i = 0; i < 2; i++) {
    printf("%s", array[i]);
    printf("\n");
  }
  printf("\n");
  free(array);
  free(message);

  // malformed message test
  printf("/***************** MALFORMED MESSAGE/INVALID ARGUMENTS TEST *****************/\n");
  loopArgs_t* args = mem_malloc_assert(sizeof(loopArgs_t), "malformed test: allocation error for loop args\n");
  args->gameinfo = gameInfo_newGameInfo(25, 500, "maps/hole.txt", 26);
  args->playerID = "@";

  char* message1 = "HANDLER spencer";
  char* message2 = "PLAY";
  char* message3 = "GRID 10";
  char* message4 = "GRID 10 A";
  char* message5 = "GRID A 10";
  char* message6 = "KEY A B";
  char* message7 = "KEY AB";
  char* message8 = "GOLD 1 4";
  char* message9 = "GOLD a 5 7";
  char* message10 = "GOLD 6 8 N";
  char* message11 = "GRID -1 10";

  addr_t tester = message_noAddr();
  message_setAddr("localhost", "9875", &tester);

  printf("TEST 1: UNKNOWN CMD\n");
  handleMessage(args, tester, message1);

  printf("TEST 2: NO NAME\n");
  handleMessage(args, tester, message2);

  printf("TEST 3: INVALID GRID ARGS\n");
  handleMessage(args, tester, message3);

  printf("TEST 4: INVALID GRID 1\n");
  handleMessage(args, tester, message4);

  printf("TEST 5: INVALID GRID 2\n");
  handleMessage(args, tester, message5);

  printf("TEST 6: INVALID KEY ARGS\n");
  handleMessage(args, tester, message6);

  printf("TEST 7: UNKNOWN KEY ARGS\n");
  handleMessage(args, tester, message7);

  printf("TEST 8: INVALID GOLD ARGS 1\n");
  handleMessage(args, tester, message8);

  printf("TEST 9: INVALID GOLD ARGS 2\n");
  handleMessage(args, tester, message9);

  printf("TEST 10: INVALID GOLD ARGS 3\n");
  handleMessage(args, tester, message10);

  printf("TEST 11: NEGATIVE GRID ARGS\n");
  handleMessage(args, tester, message11);

  printf("TESTS PASSED\n");

  // valid arguments test
  printf("/***************** VALID ARGUMENTS TEST *****************/\n");
  char* valid1 = "PLAY nunchuck buccaneers";
  char* valid2 = "SPECTATE";
  // char* valid3 = "GRID 10 10";
  char* valid4 = "QUIT";
  char* valid5 = "KEY h";
  char* valid6 = "KEY Q";
  char* valid7 = "DISPLAY thisisthemappassedin";
  char* valid8 = "GOLD 10 10 100";
  
  printf("TEST 1: VALID PLAY TEST\n");
  handleMessage(args, tester, valid1);

  printf("\nTEST 2: VALID SPECTATE TEST\n");
  handleMessage(args, tester, valid2);

  /*
  printf("\nTEST 3: VALID GRID TEST\n");
  handleMessage(args, tester, valid3);
  */

  printf("\nTEST 4: VALID QUIT TEST\n");
  handleMessage(args, tester, valid4);

  printf("\nTEST 5: VALID KEY TEST 1\n");
  handleMessage(args, tester, valid5);

  printf("\nTEST 6: VALID KEY TEST 2\n");
  handleMessage(args, tester, valid6);

  printf("\nTEST 7: VALID DISPLAY TEST\n");
  handleMessage(args, tester, valid7);
  printf("%s\n", valid7);

  printf("\nTEST 8: VALID GOLD TEST\n");
  handleMessage(args, tester, valid8);
  
  printf("TESTS PASSED\n");
  gameInfo_delete(args->gameinfo);
  mem_free(args);
  return 0;
}



