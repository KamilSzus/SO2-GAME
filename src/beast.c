//
// Created by kamil on 12.11.2022.
//

#include <stdlib.h>
#include "../headers/beast.h"

beast initBeast(boardData *board) {

    beast beast = {0};

    sem_init(&beast.received_data, 0, 0);
    sem_init(&beast.map_calculated, 0, 0);


    beast.world_size.y = board->height;
    beast.world_size.x = board->width;

    point camp;
    camp.x = board->width;
    camp.y = board->height;
    beast.isBeastHunt = 0;
    randomBeastSpawn(&beast, board);
    mapFragmentBeast(board, beast.spawn_location, &beast);
    beast.pos = beast.spawn_location;

    return beast;
}

void mapFragmentBeast(boardData *board, point point, beast *pBeast) {
    int tempX = point.x - 2;
    int tempY = point.y - 2;

    for (int i = 0; i < 5; ++i) {
        int src_y = tempY + i;

        for (int j = 0; j < 5; ++j) {
            int src_x = tempX + j;

            if (src_y < 0 || src_x < 0 || src_y >= board->height || src_x >= board->width) {
                pBeast->map[i * 5 + j] = ' ';
                continue;
            }

            char temp = board->map[src_y * board->width + src_x];

            if (temp != '\0') {
                pBeast->map[i * 5 + j] = temp;
            }
        }
    }

}

void randomBeastSpawn(beast *pBeast, boardData *board) {
    if (!pBeast) {
        return;
    }

    point spawn;
    while (1) {
        spawn.x = rand() % (pBeast->world_size.x - 1) + 1;
        spawn.y = rand() % (pBeast->world_size.y - 1) + 1;
        if (*(board->map + spawn.y * board->width + spawn.x) == ' ') {
            *(board->map + spawn.y * board->width + spawn.x) = '*';
            break;
        }
    }

    pBeast->spawn_location = spawn;
}

int beastPull(beast *pBeast, point *newPosition) {
    //..1..
    //.....
    //..*..
    //.....
    //.....
    if (!pBeast) {
        return 2;
    }
    //up
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y - i) * 5 + pBeast->pos.x];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x;
            newPosition->y = pBeast->pos.y - i;
            return 0;
        }
    }

    //down
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y + i) * 5 + pBeast->pos.x];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x;
            newPosition->y = pBeast->pos.y + i;
            return 0;
        }
    }

    //left
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[pBeast->pos.y * 5 + pBeast->pos.x - i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x - i;
            newPosition->y = pBeast->pos.y;
            return 0;
        }
    }

    //right
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[pBeast->pos.y * 5 + pBeast->pos.x + i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x + i;
            newPosition->y = pBeast->pos.y;
            return 0;
        }
    }

    //rightUp
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y - i) * 5 + pBeast->pos.x + i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x + i;
            newPosition->y = pBeast->pos.y - i;
            return 0;
        }
    }

    //leftUp
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y - i) * 5 + pBeast->pos.x - i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x - i;
            newPosition->y = pBeast->pos.y - i;
            return 0;
        }
    }

    //rightDown
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y - i) * 5 + pBeast->pos.x + i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x + i;
            newPosition->y = pBeast->pos.y - i;
            return 0;
        }
    }

    //leftDown
    for (int i = 1; i <= 2; ++i) {
        char temp = pBeast->map[(pBeast->pos.y + i) * 5 + pBeast->pos.x - i];

        if (temp == '@') {
            break;
        }
        if (temp == '1' || temp == '2') {
            newPosition->x = pBeast->pos.x - i;
            newPosition->y = pBeast->pos.y + i;
            return 0;
        }
    }

    return 1;
}

void beastMove(beast *beastStruct, point *newPos, boardData *map) {

    map->map[newPos->y * map->width + newPos->x] = '*';
    map->map[newPos->y * map->width + newPos->x] = ' ';
    beastStruct->pos = *newPos;

}