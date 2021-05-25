/*
 * playerInfo.h - header file for the playerInfo module
 * fully transparent module
 * 
 * *playerInfo* nested struct stores info about the players
 * 
 * Nunchuck Buccaneers, May 2021
 */

#ifndef __PLAYERINFO_H
#define __PLAYERINFO_H

 /*********************** types **********************/
typedef struct playerInfo {
    grid_t* sightGrid;
    pos2D_t* pos;
    int score;
    int playerID;
    addr_t* address;
    char* username;
} playerInfo_t;

#endif // __PLAYERINFO_H