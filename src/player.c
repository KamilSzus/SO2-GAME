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

    //p.ID = ++id;

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

    return p;
}

void randomPlayerSpawn(player *player, boardData *board) {
    if (!player) {
        return;
    }

    point spawn;
    //sprawdzic czy player pojawi sie na innym
    while (1) {
        spawn.x = rand() % (player->world_size.x - 1) + 1;
        spawn.y = rand() % (player->world_size.y - 1) + 1;
        //*(map->map + i * map->width + j)
        if (*(board->map + spawn.y * board->width + spawn.x) == ' '){
            break;
        }
    }

    player->spawn_location= spawn;

}