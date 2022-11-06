//
// Created by kamil on 20.10.2022.
// szerokosc - 52 wysokosc - 25 liczac od 1
//███████████████████████████████████████████████████
//█c  █*      █   #####       █         █ccccccc█   █
//█ █ ███ ███ ███████████ ███ █ ███████ ███ █████ t █
//█ █ 3 █ █ █           █ █ █   █     █     █   █   █
//█ ███ █ █ ███###█████ █ █ █████ █████████████ ███ █
//█ █T█   █           █ █ █ ##  █      c█       █ █ █
//█ █ █████ ███ ███████ █ █ █ ███ ███ ███ ███ █ █ █ █
//█ █   *     █ █       █ █ █     █   █   █ █ █   █ █
//█ █ ███████ ███ ███████ █████ ███ ███ ███ █ ███ █ █
//█ █T█     █   █   █     █   █   █    *    █ █ █ █ █
//█ ███ ███ ███ ███ ███ ███ █ ███ ███████████ █ █ █ █
//█ █   █       █ █  c█  A  █ █   █ █  D    █ █   █ █
//█ █ ██████#██ █ ███ ███ ███ ███ █ █ █████ █ █ ███ █
//█ █    #█   █ █   █   █   █   █   █ █     █ █ █   █
//█ █ █ ##█ ███ ███ ███ ███████ ███ █ ███ ███ █ █ ███
//█ █ █## █    #  █   █ █  ###  █   █  1█     █ █ █ █
//█ █ █#  ███████ █ █ █ █ ██#████ █████ ███████ █ █ █
//█ █ █#      █   █ █ █   █     █   █ █       ##█   █
//█ █████████ █ ███ ███████ ███████ █ █████ █ ##███ █
//█ █#      █ █     █     █       █   █   █ █ ##  █ █
//█ █ █████ █ ███████ █ ███ █████ ███ █ █ ███#█████ █
//█###█     █     2   █  ###█## █     █ █   █###### █
//█c███ █████████████████████#█ ███████ ███ █#    # █
//█ccc█      *          ######█##         █    ##   █
//███████████████████████████████████████████████████
//
#ifndef SO2_BOARD_H
#define SO2_BOARD_H


#include <curses.h>
#include <ncurses.h>
#include <malloc.h>
#include <sys/types.h>
#include <semaphore.h>

typedef struct {
    char *map;
    int campLocationX;
    int campLocationY;
    int width;
    int height;
} boardData;


boardData *mapLoad(char *filename);

void mapDestroy(boardData *map);

void mapPrint(int y, int x, WINDOW *window, boardData *map);

void generateRandomCoin(boardData *map);

void generateRandomTreasure(boardData *map);

void generateRandomLargeTreasure(boardData *map);

void init_colors();

#endif //SO2_BOARD_H
