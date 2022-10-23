//
// Created by kamil on 23.10.2022.
//
#include "board.h"

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

    map->map = (char *) calloc((map->width * map->height), sizeof(char));
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

            mvwprintw(window, i + y, j + x, "%c", object);
        }
    }
}

void mapDestroy(boardData *map) {
    if (!map || !map->map || map->height < 0 || map->width < 0 || map->campLocationX < 0 || map->campLocationY < 0) {
        return;
    }
    free(map->map);
    free(map);
}