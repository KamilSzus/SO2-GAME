//
// Created by kamil on 31.10.2022.
//

#ifndef SO2_PLAYER_H
#define SO2_PLAYER_H

#include <semaphore.h>
#include "point.h"
#include "board.h"
#include "beast.h"
#include <stdio.h>

typedef struct player {
    sem_t receivedData;
    char name[8];
    point worldSize;
    point spawnLocation;
    point pos;
    int roundNumber;
    int bushTimer;
    int ID;
    pid_t playerPID;
    pid_t serverPid;
    int deaths;
    int coinsCarried;
    int coinsInDeposit;
    int move;
    int isPlayerMoved;
    char map[25];
    int isDeath;
    int wasInBush;
} player;

player initPlayer(int i, boardData *board, pid_t serverPID);

void randomPlayerSpawn(player *player, boardData *board);

void mapFragment(boardData *src, point spawn, player *player);

void movePlayer(boardData *map, player *player);

void addOneCoin(player *player);

void addSmallTreasure(player *player);

void addLargeTreasure(player *player);

void depositGold(player *player);

void killPlayer(player *playerOne, player *playerTwo, beast *pBeast, boardData *map);

#endif //SO2_PLAYER_H
