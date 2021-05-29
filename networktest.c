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
}



