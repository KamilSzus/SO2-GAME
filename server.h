//
// Created by kamil on 23.10.2022.
//

#ifndef SO2_SERVER_H
#define SO2_SERVER_H

#include <sys/types.h>
#include <bits/semaphore.h>
#include <curses.h>
#include <ncurses.h>
#include <malloc.h>
#include "board.h"

typedef struct sharedMemoryJoin {
    int fd;
    pid_t server_PID;
    pid_t player_pid;
    bool isBot;
    //JoinStatus join_status;
    int player_number;
    sem_t server_open_request;
    sem_t server_new_request;
    sem_t server_checked_request;
    pthread_t sharedMemoryThread;
    bool isSharedMemoryRunning;


} sharedMemoryJoin;

typedef struct infoServer {
    //player* players;
    pid_t server_PID;//id prorocesu
    boardData *board;
    pthread_mutex_t mutex;
    sharedMemoryJoin *sharedMemoryJoin;


} infoServer;


infoServer *serverInit();

void serverRun(infoServer *server);

int createFifoFile(char *path);

int createFifoPath(char *dest, int id, char *type);

#endif //SO2_SERVER_H
