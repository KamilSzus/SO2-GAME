//
// Created by kamil on 12.11.2022.
//

#ifndef SO2_BEAST_H
#define SO2_BEAST_H

#include <semaphore.h>
#include "point.h"
#include "board.h"
#include <stdio.h>

typedef struct beast {
    sem_t received_data;
    sem_t map_calculated;
    point world_size;
    point spawn_location;
    point pos;
    int is_connected;
    int round_number;
    int bush_timer;
    int ID;
    pid_t server_PID;
    int type;
    int move;
    int isBeastMoved;
    char map[25];
} beast;


beast initBeast(boardData *board, pid_t serverPID);

void randomBeastSpawn(beast *pBeast, boardData *board);

void mapFragmentBeast(boardData *board, point point, beast *pBeast);



#endif //SO2_BEAST_H
