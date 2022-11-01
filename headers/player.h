//
// Created by kamil on 31.10.2022.
//

#ifndef SO2_PLAYER_H
#define SO2_PLAYER_H

#include <semaphore.h>
#include "point.h"
#include "board.h"
#include <stdio.h>

typedef struct player {
    sem_t received_data;
    sem_t map_calculated;
    char map[100][100];
    char name[8];
    point world_size;
    point spawn_location;
    point pos;
    point campsite_xy;
    int is_connected;
    int round_number;
    int bush_timer;
    int ID;
    pid_t PID;
    pid_t server_PID;
    int type;
    int deaths;
    int coins_found;
    int coins_brought;
    int move;
} player;

player initPlayer(int i, boardData *board, pid_t serverPID);

void randomPlayerSpawn(player *player, boardData *board);

#endif //SO2_PLAYER_H
