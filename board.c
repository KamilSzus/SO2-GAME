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

void serverInfoPrint(int y, int x, WINDOW *window)
{
    mvwprintw(window, y++, x, "Server's PID: %d", 666);
    mvwprintw(window, y++, x + 1, "Campsite X/Y: %d/%d", 0, 0);
    mvwprintw(window, y++, x + 1, "Round number: %d", 0);

    mvwprintw(window, y++, x, "Parameter:   ");
    mvwprintw(window, y++, x + 1, "PID:          ");
    mvwprintw(window, y++, x + 1, "Type:         ");
    mvwprintw(window, y++, x + 1, "Curr X/Y:     ");
    mvwprintw(window, y++, x + 1, "Deaths:       ");
    y++;
    mvwprintw(window, y++, x, "Coins        ");
    mvwprintw(window, y++, x + 1, "carried       ");
    mvwprintw(window, y++, x + 1, "brought       ");
    y -= 9;
}