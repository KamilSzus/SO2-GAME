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
    pid_t serverPid;//id prorocesu
    boardData *board;
    int isPlayerOneConnected;
    int isPlayerTwoConnected;

} infoServer;

typedef struct authentication {
    int playerNumber;
    sem_t authenticationPost;
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
    pthread_mutex_t mutex;
} serverAndThread;

infoServer *serverInit();

void serverRun(infoServer *server);

void serverInfoPrintServer(int y, int x, WINDOW *window, infoServer server);

void serverInfoPrintPlayers(int y, int x, int i, WINDOW *window, player player);

void printLegend(int y, int x, WINDOW *window);

void *playerConnection(void *player_struct);

void *beastConnection(void *beastStruct);

int keyFunc(void);

void *keyboardInputFunc(void *pkey);

void *maintainServer(void *pServer);

void *authenticationThreadFunc(void *pServer);

void *reConnectPlayerOne(void *pServer);

void *reConnectPlayerTwo(void *pServer);

#endif //SO2_SERVER_H
