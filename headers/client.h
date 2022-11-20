//
// Created by kamil on 29.10.2022.
//

#ifndef SO2_CLIENT_H
#define SO2_CLIENT_H

#include "board.h"
#include "player.h"

//                                                      Server's PID: 1370
//█ ███                                                Campsite X/Y: unknown
//█ 3 █                                                Round number: 32234
//███ █
//█T█                                                 Player:
//                                                    Number:     3
//                                                    Type:       HUMAN
//                                                    Curr X/Y    04/03
//                                                    Deaths      0
//
//                                                    Coins found 0
//                                                    Coins brought 0


typedef struct keyThreadInfoPlayer {
    int key;
    pthread_mutex_t mutex;
} keyThreadInfoPlayer;

void connectToServer();

void *keyboardInputFuncPlayer(void *pKey);

int keyFuncPlayer(void);

void clientInfoPrintPlayers(int y, int x, int i, WINDOW *window, player player);

void clientInfoPrintServer(int y, int x, WINDOW *window, player player);


#endif //SO2_CLIENT_H
