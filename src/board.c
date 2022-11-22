//
// Created by kamil on 23.10.2022.
//
#include <stdlib.h>
#include "../headers/board.h"

boardData *mapLoad(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    boardData *map = (boardData *) calloc(1, sizeof(boardData));
    if (!map) {
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d %d\n", &map->width, &map->height);

    map->map = (char *) calloc(((map->width + 1) * (map->height + 1)), sizeof(char));
    if (!map->map) {
        mapDestroy(map);
        fclose(file);
        return NULL;
    }

    int i = 0;
    while (!feof(file)) {
        char temp = (char) getc(file);
        if (temp == '\n' || temp == '\r') {
            continue;
        }
        map->map[i++] = temp;
    }
    map->lastIndexArray = 0;

    fclose(file);
    return map;
}

void mapPrint(int y, int x, WINDOW *window, boardData *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            char object = *(map->map + i * map->width + j);
            if (object == '\0') {
                continue;
            }
            attron(COLOR_PAIR(object));
            mvwprintw(window, i + y, j + x, "%c", object);
            attroff(COLOR_PAIR(object));
        }
    }
    if (map->lastIndexArray != 0) {
        mvwprintw(window, 25, 25, "%d", map->droppedCoins[0].coins);
        mvwprintw(window, 26, 26, "%d %d", map->droppedCoins[0].collisionLocalization.y,
                  map->droppedCoins[0].collisionLocalization.x);
    }
}

void mapDestroy(boardData *map) {
    if (!map || !map->map || map->height < 0 || map->width < 0 || map->campLocationX < 0 || map->campLocationY < 0) {
        return;
    }
    free(map->map);
    free(map);
}

void generateRandomCoin(boardData *map) {
    if (!map) {
        return;
    }

    point spawn;
    while (1) {
        spawn.x = rand() % (map->width - 1) + 1;
        spawn.y = rand() % (map->height - 1) + 1;
        if (*(map->map + spawn.y * map->width + spawn.x) == ' ') {
            *(map->map + spawn.y * map->width + spawn.x) = 'c';
            break;
        }
    }

}

void generateRandomTreasure(boardData *map) {
    if (!map) {
        return;
    }

    point spawn;
    while (1) {
        spawn.x = rand() % (map->width - 1) + 1;
        spawn.y = rand() % (map->height - 1) + 1;
        if (*(map->map + spawn.y * map->width + spawn.x) == ' ') {
            *(map->map + spawn.y * map->width + spawn.x) = 't';
            break;
        }
    }

}

void generateRandomLargeTreasure(boardData *map) {
    if (!map) {
        return;
    }

    point spawn;
    while (1) {
        spawn.x = rand() % (map->width - 1) + 1;
        spawn.y = rand() % (map->height - 1) + 1;
        if (*(map->map + spawn.y * map->width + spawn.x) == ' ') {
            *(map->map + spawn.y * map->width + spawn.x) = 'T';
            break;
        }
    }
}

void init_colors() {
    init_pair(' ', COLOR_WHITE, COLOR_BLACK);
    init_pair('@', COLOR_BLACK, COLOR_WHITE);
    init_pair('1', COLOR_WHITE, COLOR_BLUE);
    init_pair('*', COLOR_RED, COLOR_BLACK);
    init_pair('c', COLOR_BLACK, COLOR_YELLOW);
    init_pair('C', COLOR_YELLOW, COLOR_RED);
    init_pair('#', COLOR_GREEN, COLOR_BLACK);
}

void mapPrintFragment(int y, int x, WINDOW *window, const char map[]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            char object = (map[i * 5 + j]);
            if (object == '\0') {
                continue;
            }
            attron(COLOR_PAIR(object));
            mvwprintw(window, i + y, j + x, "%c", object);
            attroff(COLOR_PAIR(object));
        }
    }
}