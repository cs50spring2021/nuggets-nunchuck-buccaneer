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
  printf("Network Test\n");
  char** array = tokenizeMessage("GRID 15 23 18");

  printf("\n Words in the array: ");
  // i is 4 because in this particular case we are testing a "GRID" message
  for (int i = 0; i < 4; i++) {
    printf("%s ", array[i]);
  }
  printf("\n");

}


