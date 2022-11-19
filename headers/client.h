//
// Created by kamil on 29.10.2022.
//

#ifndef SO2_CLIENT_H
#define SO2_CLIENT_H

#include "board.h"


typedef struct keyThreadInfoPlayer {
    int key;
    pthread_mutex_t mutex;
} keyThreadInfoPlayer;

void connectToServer();

void *keyboardInputFuncPlayer(void *pKey);

int keyFuncPlayer(void);


#endif //SO2_CLIENT_H
