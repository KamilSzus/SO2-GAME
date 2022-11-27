//
// Created by kamil on 12.11.2022.
//

#include <stdlib.h>
#include "../headers/beast.h"

beast initBeast(boardData *board) {
    beast beast = {0};

    sem_init(&beast.receivedData, 0, 0);

    beast.worldSize.y = board->height;
    beast.worldSize.x = board->width;

    point camp;
    camp.x = board->width;
    camp.y = board->height;
    beast.isBeastHunt = 0;
    randomBeastSpawn(&beast, board);
    mapFragmentBeast(board, beast.spawnLocation, &beast);
    beast.pos = beast.spawnLocation;
    beast.isBeastMoved = 0;
    beast.wasInBush = 0;
    beast.isBestOnCoin = 0;
    beast.isBestOnSmallTreasure = 0;
    beast.isBestOnLargeTreasure = 0;
    beast.isBestOnCampsite = 0;
    beast.isBestOnDrop = 0;
    beast.bushTimer = 0;

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
        spawn.x = rand() % (pBeast->worldSize.x - 1) + 1;
        spawn.y = rand() % (pBeast->worldSize.y - 1) + 1;
        if (*(board->map + spawn.y * board->width + spawn.x) == ' ') {
            *(board->map + spawn.y * board->width + spawn.x) = '*';
            break;
        }
    }

    pBeast->spawnLocation = spawn;
}

int beastPull(beast *pBeast, point *newPosition, boardData *map) {
    //..1..
    //.....
    //..*..
    //.....
    //.....
    //bestia porusza sie o 2
    if (!pBeast) {
        return 2;
    }
    //up
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y - i) * map->width + pBeast->pos.x];
        //map->map[newPosition.y * map->width + newPosition.x]

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y + i) * map->width + pBeast->pos.x];

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[pBeast->pos.y * map->width + pBeast->pos.x - i];

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[pBeast->pos.y * map->width + pBeast->pos.x + i];
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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y - i) * map->width + pBeast->pos.x + i];

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y - i) * map->width + pBeast->pos.x - i];

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y - i) * map->width + pBeast->pos.x + i];

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
    for (int i = 1; i <= 2; i++) {
        char temp = map->map[(pBeast->pos.y + i) * map->width + pBeast->pos.x - i];

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
    if (beastStruct->isBeastMoved == 0) {

        if (beastStruct->isBestOnCoin == 1) {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = 'c';
            beastStruct->isBestOnCoin = 0;
        } else if (beastStruct->isBestOnSmallTreasure == 1) {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = 't';
            beastStruct->isBestOnSmallTreasure = 0;
        } else if (beastStruct->isBestOnLargeTreasure == 1) {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = 'T';
            beastStruct->isBestOnLargeTreasure = 0;
        } else if (beastStruct->isBestOnCampsite == 1) {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = 'C';
            beastStruct->isBestOnCampsite = 0;
        } else if (beastStruct->isBestOnDrop == 1) {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = 'D';
            beastStruct->isBestOnDrop = 0;
        }

        if (beastStruct->wasInBush == 1) {
            if (map->map[newPos->y * map->width + newPos->x] == '@') {
                return;
            }
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = '#';
            beastStruct->wasInBush = 0;
        }

        if (map->map[newPos->y * map->width + newPos->x] == '@') {
            return;
        } else if (map->map[newPos->y * map->width + newPos->x] == 'c') {
            beastStruct->isBestOnCoin = 1;
        } else if (map->map[newPos->y * map->width + newPos->x] == 't') {
            beastStruct->isBestOnSmallTreasure = 1;
        } else if (map->map[newPos->y * map->width + newPos->x] == 'T') {
            beastStruct->isBestOnLargeTreasure = 1;
        } else if (map->map[newPos->y * map->width + newPos->x] == 'C') {
            beastStruct->isBestOnCampsite = 1;
        } else if (map->map[newPos->y * map->width + newPos->x] == '#') {
            beastStruct->bushTimer = 2;
            beastStruct->wasInBush = 1;
        } else if (map->map[newPos->y * map->width + newPos->x] == 'D') {
            beastStruct->isBestOnDrop = 1;
        }

        map->map[newPos->y * map->width + newPos->x] = '*';
        if (map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] != 'C' &&
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] != '#') {
            map->map[beastStruct->pos.y * map->width + beastStruct->pos.x] = ' ';
        }

        beastStruct->pos = *newPos;
        beastStruct->isBeastMoved = 1;

    }
}

void beastRandomMove(beast *pBeast, boardData *board) {
    int directory = rand() % (4) + 1;
    point newPos = pBeast->pos;

    switch (directory) {
        case 1:
            newPos.x = newPos.x - 1;
            break;
        case 2:
            newPos.y = newPos.y - 1;
            break;
        case 3:
            newPos.x = newPos.x + 1;
            break;
        case 4:
            newPos.y = newPos.y + 1;
            break;
        default:
            return;
    }

    beastMove(pBeast, &newPos, board);
}