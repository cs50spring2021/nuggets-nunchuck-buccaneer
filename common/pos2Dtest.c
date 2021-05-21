/* 
 * pos2Dtest.c - test program for `pos2D` module
 *
 * Nunchuck-buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "pos2D.h"
#include "readline.h"

int
main(const int argc, const char* argv[])
{
    // create 100 new pos2D objects and print them out
    for (int i = 0; i < 100; i++) {
        int x, y = rand() % 100;
        pos2D_t* pos = pos2D_new(x, y);
        printf("pos2D_%d: %d, %d\n", i, pos2D_getX(pos), pos2D_getY(pos));
        pos2D_delete(pos);
    }

    printf("\n");
    printf("Interactive terminal mode: \n");
    printf("Use commands <n> and <s> to create new pos2D or set pos2D values\n");
    printf("\n");
    char* line;
    while ((line = readline(stdin)) != NULL) {
        printf("pos2D: ");
        char* info[3];
        char* cmd = strtok(line, " ");
        int i = 0;
        while (cmd != NULL) {
            info[i] = cmd;
            cmd = strtok(NULL, " ");
            i++;
        }

        printf("\n");
        pos2D_t* pos = NULL;
        if (info[0] == "n") {
            pos = pos2D_new(info[1], info[2]);
        } 
        else if (info[0] == "s") {
            if (pos == NULL) {
                pos = pos2D_new(0, 0);
            }
            printf("pos2D before setting: %d, %d\n", pos2D_getX(pos), pos2D_getY(pos));
            pos2D_set(pos, info[1], info[2]);
        }
        printf("pos2D: %d, %d\n", pos2D_getX(pos), pos2D_getY(pos));
    }

    return 0; // successful run
}