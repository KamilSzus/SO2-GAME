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
#include "beast.h"

typedef struct infoServer {
    int playersNumber;
    int coinNumber;
    int treasureNumber;
    int beastNumber;
    int roundNumber;
    pid_t server_PID;//id prorocesu
    boardData *board;
    pthread_mutex_t mutex;
    sem_t update;

} infoServer;

typedef struct authentication {
    int playerNumber;
    sem_t authenticationPost;
    sem_t authenticationStartGame;
} authentication;

typedef struct keyThreadInfo {
    int key;
    pthread_mutex_t mutex;
} keyThreadInfo;

typedef struct serverAndThread {
    int id;
    infoServer *infoServer;
    player *playerInThread;
    beast *beastInThread;
} serverAndThread;

typedef struct droppedCoins {
    point collisionLocalization;
    int coins;
} droppedCoins;

infoServer *serverInit();

void serverRun(infoServer *server);

void serverInfoPrintServer(int y, int x, WINDOW *window, infoServer server);

void serverInfoPrintPlayers(int y, int x, int i, WINDOW *window, player player);

void printLegend(int y, int x, WINDOW *window);

void *player_connection(void *player_struct);

void *beastConnection(void *beastStruct);

int keyFunc(void);

void *keyboardInputFunc(void *pkey);

void *maintainServer(void *pServer);

#endif //SO2_SERVER_H
