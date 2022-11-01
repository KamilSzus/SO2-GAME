//
// Created by kamil on 23.10.2022.
//

#ifndef SO2_SERVER_H
#define SO2_SERVER_H

#include <locale.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "board.h"

typedef struct userJoin {
    int player_number;
    sem_t server_open_request;
    sem_t server_new_request;
    sem_t server_checked_request;
    pid_t player_pid;
    bool isBot;
    pid_t server_PID;
    boardData *board;//do testow
    char payload[1024];
    //JoinStatus join_status;
} userJoin;

typedef struct sharedMemoryJoin {
    int fd;
    userJoin *userJoin;
    pthread_t sharedMemoryThread;
    bool isSharedMemoryRunning;

} sharedMemoryJoin;


typedef struct infoServer {
    //player* players;
    int playersNumber;
    pid_t server_PID;//id prorocesu
    boardData *board;
    pthread_mutex_t mutex;
    sharedMemoryJoin sharedMemoryJoin;
    sem_t update;

} infoServer;


infoServer *serverInit();

void serverRun(infoServer *server);

int createFifoFile(char *path);

int createFifoPath(char *dest, int id, char *type);

#endif //SO2_SERVER_H
