//
// Created by kamil on 31.10.2022.
//

#include <unistd.h>
#include <stdlib.h>
#include "../headers/player.h"

player initPlayer(int i, boardData *board, pid_t serverPID) {

    player p = {0};

    sprintf(p.name, "Player%d", i);

    sem_init(&p.received_data, 0, 0);
    sem_init(&p.map_calculated, 0, 0);

    p.ID = i;

    p.world_size.y = board->height;
    p.world_size.x = board->width;
    p.coins_brought = 0;
    p.coins_found = 0;

    point camp;
    camp.x = board->width;
    camp.y = board->height;

    p.campsite_xy = camp;
    p.server_PID = serverPID;
    randomPlayerSpawn(&p, board);
    mapFragment(board, p.spawn_location, &p);

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
            //int src_x = spawn.x + j;
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