//
// Created by kamil on 23.10.2022.
//

#ifndef SO2_SERVER_H
#define SO2_SERVER_H

//#include <sys/types.h>
//#include <bits/semaphore.h>
#include <curses.h>
#include <ncurses.h>
#include <malloc.h>

//typedef struct player_info {
//    sem_t received_data;
//    sem_t map_calculated;
//    int is_connected;
//    int round_number;
//    int bush_timer;
//    int ID;
//    pid_t PID;
//    pid_t server_PID;
//    int type;
//    int deaths;
//    int coins_found;
//    int coins_brought;
//    int move;
//} player;

typedef struct infoServer {
    //player* players;
    int campsite_xy;
    //pid_t server_PID;
    int player_count;
    int round_number;
} infoServer;

void serverInit(infoServer *server);

#endif //SO2_SERVER_H
