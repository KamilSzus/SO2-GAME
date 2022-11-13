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
#include "player.h"

typedef struct infoServer {
    //player* players;
    int playersNumber;
    int coinNumber;
    int treasureNumber;
    int beastNumber;
    pid_t server_PID;//id prorocesu
    boardData *board;
    pthread_mutex_t mutex;
    // sharedMemoryJoin sharedMemoryJoin;
    sem_t update;

} infoServer;

typedef struct authentication {
    int playerNumber;
    sem_t authenticationPost;
    sem_t authenticationStartGame;
} authentication;

//typedef struct serverAndThread {
//    int id;
//    infoServer *infoServer;
//    void * structInThread;
//} serverAndThread;


infoServer *serverInit();

void serverRun(infoServer *server);

void serverInfoPrintServer(int y, int x, WINDOW *window, infoServer server);

void serverInfoPrintPlayers(int y, int x, WINDOW *window, player player[]);

void printLegend(int y, int x, WINDOW *window);

void *player_connection(void *player_struct);

void *beastConnection(void *beastStruct);

#endif //SO2_SERVER_H
