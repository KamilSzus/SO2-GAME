//
// Created by kamil on 31.10.2022.
//

#include <stdlib.h>
#include "../headers/player.h"

player initPlayer(int i, boardData *board, pid_t serverPID) {
    player p = {0};

    sprintf(p.name, "Player%d", i);

    sem_init(&p.received_data, 0, 0);
    sem_init(&p.map_calculated, 0, 0);

    p.ID = i;

    point worldSize;
    worldSize.x = board->width;
    worldSize.y = board->height;

    p.world_size = worldSize;
    p.coinsInDeposit = 0;
    p.coinsCarried = 0;

    p.server_PID = serverPID;
    randomPlayerSpawn(&p, board);
    mapFragment(board, p.spawn_location, &p);
    p.pos = p.spawn_location;
    p.isPlayerMoved = 0;
    p.bushTimer = 0;
    p.wasInBush = 0;

    return p;
}

void randomPlayerSpawn(player *player, boardData *board) {
    if (!player) {
        return;
    }

    point spawn;
    while (1) {
        spawn.x = rand() % (player->world_size.x - 1) + 1;
        spawn.y = rand() % (player->world_size.y - 1) + 1;
        if (*(board->map + spawn.y * board->width + spawn.x) == ' ') {
            *(board->map + spawn.y * board->width + spawn.x) = player->ID + '0';
            break;
        }
    }

    player->spawn_location = spawn;

}

void mapFragment(boardData *src, point spawn, player *player) {
    //.....
    //.....
    //..1..
    //.....
    //.....

    int tempX = spawn.x - 2;
    int tempY = spawn.y - 2;

    for (int i = 0; i < 5; ++i) {
        int src_y = tempY + i;

        for (int j = 0; j < 5; ++j) {
            int src_x = tempX + j;

            if (src_y < 0 || src_x < 0 || src_y >= src->height || src_x >= src->width) {
                player->map[i * 5 + j] = ' ';
                continue;
            }

            char temp = src->map[src_y * src->width + src_x];

            if (temp != '\0') {
                player->map[i * 5 + j] = temp;
            }
        }
    }

}

void movePlayer(boardData *map, player *player) {
    if (!map || !player || player->isPlayerMoved != 0) {
        return;
    }

    point newPosition;

    if (player->move == 68) {//left
        newPosition.y = player->pos.y;
        newPosition.x = player->pos.x - 1;
    } else if (player->move == 67) {//right
        newPosition.y = player->pos.y;
        newPosition.x = player->pos.x + 1;
    } else if (player->move == 65) {//down
        newPosition.y = player->pos.y - 1;
        newPosition.x = player->pos.x;
    } else if (player->move == 66) {//up
        newPosition.y = player->pos.y + 1;
        newPosition.x = player->pos.x;
    }

    //tymczasowe
    if (player->move == 68 || player->move == 67 || player->move == 65 || player->move == 66) {
        if (player->wasInBush == 1) {
            if (map->map[newPosition.y * map->width + newPosition.x] == '@') {
                return;
            }
            map->map[player->pos.y * map->width + player->pos.x] = '#';
            player->wasInBush = 0;
        }
        if (map->map[newPosition.y * map->width + newPosition.x] == '@') {
            return;
        } else if (map->map[newPosition.y * map->width + newPosition.x] == 'c') {
            addOneCoin(player);
        } else if (map->map[newPosition.y * map->width + newPosition.x] == 't') {
            addSmallTreasure(player);
        } else if (map->map[newPosition.y * map->width + newPosition.x] == 'T') {
            addLargeTreasure(player);
        } else if (map->map[newPosition.y * map->width + newPosition.x] == 'C') {
            depositGold(player);

            map->map[player->pos.y * map->width + player->pos.x] = ' ';
            player->pos = newPosition;
            player->move = 0;
            player->isPlayerMoved = 1;

            return;
        } else if (map->map[newPosition.y * map->width + newPosition.x] == '#') {
            player->bushTimer = 2;
            player->wasInBush = 1;
        } else if (map->map[newPosition.y * map->width + newPosition.x] == 'D') {
            for (int i = 0; i < map->lastIndexArray; i++) {
                if (isEqual(newPosition, map->droppedCoins[i].collisionLocalization) == 1) {
                    player->coinsCarried += map->droppedCoins[0].coins;
                }
            }
        }
        map->map[newPosition.y * map->width + newPosition.x] = player->ID + '0';
        if (map->map[player->pos.y * map->width + player->pos.x] != 'C' &&
            map->map[player->pos.y * map->width + player->pos.x] != '#') {
            map->map[player->pos.y * map->width + player->pos.x] = ' ';
        }

        player->pos = newPosition;
        player->move = 0;
        player->isPlayerMoved = 1;
    }
}

void addOneCoin(player *player) {
    player->coinsCarried++;
}

void addSmallTreasure(player *player) {
    player->coinsCarried += 5;
}

void addLargeTreasure(player *player) {
    player->coinsCarried += 10;
}

void depositGold(player *player) {
    player->coinsInDeposit = player->coinsCarried;
    player->coinsCarried = 0;
}

void dropGoldAfterDeath(player *player, boardData *map) {
    if (player->isDeath == 1) {
        map->map[player->pos.y * map->width + player->pos.x] = 'D';
        randomPlayerSpawn(player, map);
    }
}

void killPlayer(player *playerOne, player *playerTwo, boardData *map) {
    if (playerOne->pos.x == playerTwo->pos.x && playerOne->pos.y == playerTwo->pos.y) {
        point col = playerOne->pos;
        playerOne->pos = playerOne->spawn_location;
        playerTwo->pos = playerTwo->spawn_location;

        mapFragment(map, playerOne->spawn_location, playerOne);
        *(map->map + playerOne->spawn_location.y * map->width + playerOne->spawn_location.x) = playerOne->ID + '0';
        mapFragment(map, playerTwo->spawn_location, playerTwo);
        *(map->map + playerTwo->spawn_location.y * map->width + playerTwo->spawn_location.x) = playerTwo->ID + '0';

        int possibleValue = playerOne->coinsCarried + playerTwo->coinsCarried;

        playerOne->coinsCarried = 0;
        playerTwo->coinsCarried = 0;

        playerOne->deaths++;
        playerTwo->deaths++;

        if (possibleValue > 0) {
            map->map[col.y * map->width + col.x] = 'D';
            droppedCoins dc = {0};
            dc.collisionLocalization = col;
            dc.coins = possibleValue;
            map->droppedCoins[map->lastIndexArray] = dc;
            map->lastIndexArray++;
        }

    }
}