/* 
 * pos2Dtest.c - test program for `pos2D` module
 *
 * Nunchuck-buccaneers, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "pos2D.h"
#include "file.h"
#include "string.h"
#include "mem.h"

/********************* main ****************/
int
main(int argc, char* argv[])
{
    for (int i = 0; i < 100; i++) {
        int x = rand() % 100;
        int y = rand() % 100;
        pos2D_t* pos = pos2D_new(x, y);
        printf("pos2D_%d: %d, %d\n", i, pos2D_getX(pos), pos2D_getY(pos));
        pos2D_delete(pos);
    }
    /*
    printf("\n");
    printf("Interactive terminal mode: \n");
    printf("Use commands <n> and <s> to create new pos2D or set pos2D values\n");
    printf("\n");
    char* line;
    pos2D_t* pos = NULL;
    while ((line = file_readLine(stdin)) != NULL) {
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
        if (strcmp(info[0], "n") == 0) {
            pos = pos2D_new(atoi(info[1]), atoi(info[2]));
        } 
        else if (strcmp(info[0], "s") == 0) {
            if (pos == NULL) {
                pos = pos2D_new(0, 0);
            }
            printf("pos2D before setting: %d, %d\n", pos2D_getX(pos), pos2D_getY(pos));
            pos2D_set(pos, atoi(info[1]), atoi(info[2]));
        }
        printf("pos2D: %d, %d\n", pos2D_getX(pos), pos2D_getY(pos));
        mem_free(line);
    }
    pos2D_delete(pos);
    */
    return 0; // successful run
}