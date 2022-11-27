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
    sem_t receivedData;
    point worldSize;
    point spawnLocation;
    point pos;
    int isBeastHunt;
    int isBeastMoved;
    char map[25];
} beast;


beast initBeast(boardData *board);

void randomBeastSpawn(beast *pBeast, boardData *board);

void mapFragmentBeast(boardData *board, point point, beast *pBeast);

int beastPull(beast *pBeast, point *newPosition,boardData *map);

void beastMove(beast *beastStruct, point *newPos, boardData *map);


#endif //SO2_BEAST_H
