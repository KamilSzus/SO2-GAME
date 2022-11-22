//
// Created by kamil on 20.10.2022.
#ifndef SO2_BOARD_H
#define SO2_BOARD_H

#include <curses.h>
#include <ncurses.h>
#include <malloc.h>
#include <sys/types.h>
#include <semaphore.h>
#include "point.h"

typedef struct droppedCoins {
    point collisionLocalization;
    int coins;
} droppedCoins;

typedef struct {
    char *map;
    int campLocationX;
    int campLocationY;
    int width;
    int height;
    droppedCoins droppedCoins[200];
    int lastIndexArray;
} boardData;

boardData *mapLoad(char *filename);

void mapDestroy(boardData *map);

void mapPrint(int y, int x, WINDOW *window, boardData *map);

void generateRandomCoin(boardData *map);

void generateRandomTreasure(boardData *map);

void generateRandomLargeTreasure(boardData *map);

void init_colors();

void mapPrintFragment(int y, int x, WINDOW *window, const char map[]);

#endif //SO2_BOARD_H
