//
// Created by kamil on 23.10.2022.
//
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include "../headers/server.h"
#include <errno.h>


infoServer *serverInit() {

    infoServer *server = (infoServer *) calloc(1, sizeof(infoServer));
    if (server == NULL) {
        return NULL;
    }

    server->board = mapLoad("map.txt");
    server->serverPid = getpid();
    server->playersNumber = 0;
    server->beastNumber = 0;
    server->coinNumber = 0;
    server->treasureNumber = 0;
    server->roundNumber = 0;
    server->isPlayerOneConnected = 0;
    server->isPlayerTwoConnected = 0;
    sem_init(&server->update, 0, 1);

    return server;
}

void serverRun(infoServer *server) {
    srand(time(NULL));
    serverAndThread serverAndThread[3];
    player players[3];
    pthread_t player_thr[3];
    for (int i = 1; i <= 2; i++) {
        players[i] = initPlayer(i, server->board, server->serverPid);
        serverAndThread[i].infoServer = server;
        serverAndThread[i].id = i;
        serverAndThread[i].playerInThread = &players[i];
        if (pthread_mutex_init(&serverAndThread[i].mutex, NULL) != 0) {
            return;
        }
        pthread_create(&player_thr[i], NULL, playerConnection, &serverAndThread[i]);
    }

    setlocale(LC_ALL, "");
    WINDOW *okno1;    // Okna programu
    int znak;

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury
    start_color();
    init_colors();

    okno1 = newwin(LINES, COLS, 0, 0);
    box(okno1, 0, 0);
    wrefresh(okno1);

    pthread_t authenticationThread;
    pthread_create(&authenticationThread, NULL, authenticationThreadFunc, &serverAndThread[1]);

    beast beast;
    pthread_t beast_thr;
    int beastHunt = 0;

    pthread_t keyboardInput;
    keyThreadInfo keyInfo;
    keyInfo.key = 0;
    pthread_create(&keyboardInput, NULL, keyboardInputFunc, &keyInfo);

    if (pthread_mutex_init(&keyInfo.mutex, NULL) != 0) {
        return;
    }

    do {
        pthread_mutex_lock(&keyInfo.mutex);
        znak = keyInfo.key;
        keyInfo.key = 0;
        pthread_mutex_unlock(&keyInfo.mutex);

        mapPrint(5, 5, okno1, server->board);
        serverInfoPrintServer(5, 55, okno1, *server);
        printLegend(19, 55, okno1);
        for (int i = 1; i <= 2; i++) {
            serverInfoPrintPlayers(7, 55, i, okno1, *serverAndThread[i].playerInThread);
        }
        for (int i = 1; i <= 2; i++) {
            movePlayer(server->board, serverAndThread[i].playerInThread);
        }
        wrefresh(okno1);
        if (znak == 'c' && server->coinNumber < 50) {
            generateRandomCoin(server->board);
            server->coinNumber++;
        } else if (znak == 't' && server->treasureNumber < 50) {
            generateRandomTreasure(server->board);
            server->treasureNumber++;
        } else if (znak == 'T' && server->treasureNumber < 50) {
            generateRandomLargeTreasure(server->board);
            server->treasureNumber++;
        } else if ((znak == 'B' || znak == 'b') && server->beastNumber < 1) {
            server->beastNumber++;
            beastHunt = 1;
            beast.isBeastHunt = 1;
            beast = initBeast(server->board);
            serverAndThread[0].beastInThread = &beast;
            serverAndThread[0].beastInThread = &beast;
            serverAndThread[0].infoServer = server;
            pthread_create(&beast_thr, NULL, beastConnection, &serverAndThread[0]);
        }

        //nanosleep((const struct timespec[]) {{0, 200000000L}}, NULL);
        sleep(1);
        server->roundNumber++;

        for (int i = 1; i <= 2; i++) {
            if (serverAndThread[i].playerInThread->bushTimer == 0) {
                serverAndThread[i].playerInThread->isPlayerMoved = 0;
            } else {
                serverAndThread[i].playerInThread->bushTimer--;
            }
            serverAndThread[i].playerInThread->roundNumber = server->roundNumber;
        }
        if (beastHunt == 1) {
            mapFragmentBeast(serverAndThread[0].infoServer->board, serverAndThread[0].beastInThread->pos,
                             serverAndThread[0].beastInThread);

            if (serverAndThread[0].beastInThread->bushTimer == 0) {
                serverAndThread[0].beastInThread->isBeastMoved = 0;
            } else {
                serverAndThread[0].beastInThread->bushTimer--;
            }


        }

        killPlayer(serverAndThread[1].playerInThread, serverAndThread[2].playerInThread,
                   serverAndThread[0].beastInThread, serverAndThread[1].infoServer->board);
        //dodac jak oboje sa w camp


        werase(okno1);
        box(okno1, 0, 0);
        flushinp();
    } while (znak != 'q' && znak != 'Q');

    endwin();// Koniec pracy z CURSES
    delwin(okno1);        // Usuniecie okien
    mapDestroy(server->board);
    free(server);
}

void serverInfoPrintServer(int y, int x, WINDOW *window, infoServer Server) {
    mvwprintw(window, y++, x, "Server's playerPID: %d", Server.serverPid);
    mvwprintw(window, y++, x + 1, "Campsite X/Y: %d/%d", 21, 10);
    mvwprintw(window, y++, x + 1, "Round number: %d", Server.roundNumber);
    y++;
    mvwprintw(window, y++, x, "Parameter:   ");
    mvwprintw(window, y++, x + 1, "playerPID:          ");
    mvwprintw(window, y++, x + 1, "Type:         ");
    mvwprintw(window, y++, x + 1, "Deaths:       ");
    mvwprintw(window, y++, x + 1, "Curr X/Y:     ");
    mvwprintw(window, y++, x, "Coins        ");
    mvwprintw(window, y++, x + 1, "carried       ");
    mvwprintw(window, y, x + 1, "brought       ");
}

void serverInfoPrintPlayers(int y, int x, int i, WINDOW *window, player player) {
    mvwprintw(window, y + 2, x + 15 + (i * 25), "%s", player.name);
    mvwprintw(window, y + 3, x + 15 + (i * 25), "%s", player.playerPID);
    mvwprintw(window, y + 4, x + 15 + (i * 25), "HUMAN");
    mvwprintw(window, y + 5, x + 15 + (i * 25), "%d", player.deaths);
    mvwprintw(window, y + 6, x + 15 + (i * 25), "%d/%d", player.pos.x, player.pos.y);
    mvwprintw(window, y + 8, x + 15 + (i * 25), "%d", player.coinsCarried);
    mvwprintw(window, y + 9, x + 15 + (i * 25), "%d", player.coinsInDeposit);
}

void printLegend(int y, int x, WINDOW *window) {
    mvwprintw(window, y++, x + 1, "1234 - Players");
    mvwprintw(window, y++, x + 1, "@ - wall");
    mvwprintw(window, y++, x + 1, "# - bushes (slow down)");
    mvwprintw(window, y++, x + 1, "* - wild beast");
    mvwprintw(window, y++, x + 1, "c - one coin");
    mvwprintw(window, y++, x + 1, "t - treasure (10 coins)");
    mvwprintw(window, y - 3, x + 25, "T - large treasure (50 coins)");
    mvwprintw(window, y - 2, x + 25, "C - campsite");
    mvwprintw(window, y - 1, x + 25, "D - dropped treasure ");
}

void *playerConnection(void *playerStruct) {
    serverAndThread *pServerAndThread = (serverAndThread *) playerStruct;
    player *pPlayer = pServerAndThread->playerInThread;

    char semaforName[100] = {0};
    strcat(semaforName, "/msg_signal");
    strcat(semaforName, pPlayer->name);

    sem_t *sem = sem_open(semaforName, O_CREAT, 0600, 0);//semafor tworzy plik
    if (sem == SEM_FAILED) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    char shmName[100] = {0};
    strcat(shmName, "/gameSO2_Join_SHM");
    strcat(shmName, pPlayer->name);

    int fd = shm_open(shmName, O_CREAT | O_RDWR, 0600); //zwraca id shm
    ftruncate(fd, sizeof(player));
    player *SHPlayer = (player *) mmap(NULL, sizeof(player), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fd, 0);
    assert (SHPlayer != MAP_FAILED);

    memcpy(SHPlayer, pPlayer, sizeof(player));

    sem_init(&SHPlayer->receivedData, 1, 1); // shared, signaled

    if (pServerAndThread->id == 1) {
        pServerAndThread->infoServer->isPlayerOneConnected = 1;
    } else if (pServerAndThread->id == 2) {
        pServerAndThread->infoServer->isPlayerTwoConnected = 1;
    }

    while (SHPlayer->move != 'q') {
        sem_wait(sem);
        sem_wait(&SHPlayer->receivedData);
        mapFragment(pServerAndThread->infoServer->board, pPlayer->pos, pPlayer);
        pPlayer->move = SHPlayer->move;
        movePlayer(pServerAndThread->infoServer->board, pPlayer);
        //pPlayer->playerPID = SHPlayer->playerPID; czemu to nie dziala
        memcpy(SHPlayer, pPlayer, sizeof(player));
        sem_post(&SHPlayer->receivedData);
    }

    munmap(SHPlayer, sizeof(player));
    close(fd);
    shm_unlink(shmName);
    sem_destroy(sem);

    pthread_t playerThr;
    pthread_t authenticationThread;


    if (pServerAndThread->id == 1) {
        pServerAndThread->infoServer->isPlayerOneConnected = 0;

        pthread_create(&authenticationThread, NULL, reConnectPlayerOne, NULL);
        pthread_join(authenticationThread, NULL);

        sem_init(&pServerAndThread->playerInThread->receivedData, 0, 0);
        pServerAndThread->playerInThread->move = 0;
        pthread_create(&playerThr, NULL, playerConnection, pServerAndThread);
        pthread_join(playerThr, NULL);


    } else if (pServerAndThread->id == 2) {
        pServerAndThread->infoServer->isPlayerTwoConnected = 0;

        pthread_create(&authenticationThread, NULL, reConnectPlayerTwo, NULL);
        pthread_join(authenticationThread, NULL);
        pServerAndThread->playerInThread->move = 0;

        sem_init(&pServerAndThread->playerInThread->receivedData, 0, 0);
        pthread_create(&playerThr, NULL, playerConnection, pServerAndThread);
        pthread_join(playerThr, NULL);
    }

}

void *beastConnection(void *beastStruct) {
    serverAndThread *pServerAndThread = (serverAndThread *) beastStruct;
    beast *pBeast = (beast *) pServerAndThread->beastInThread;

    point newPos;
    while (1) {
        if (pServerAndThread->beastInThread->isBeastMoved == 0) {
            if (beastPull(pBeast, &newPos, pServerAndThread->infoServer->board) == 0) {
                beastMove(pBeast, &newPos, pServerAndThread->infoServer->board);
            } else {
                beastRandomMove(pBeast, pServerAndThread->infoServer->board);
            }
        }
    }

}

int keyFunc(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void *keyboardInputFunc(void *pKey) {
    keyThreadInfo *info = (keyThreadInfo *) pKey;
    int key;

    while (info->key != 'q' && info->key != 'Q') {
        if (keyFunc()) {
            key = getch();
            pthread_mutex_lock(&info->mutex);
            info->key = key;
            pthread_mutex_unlock(&info->mutex);
        }
    }

    return NULL;
}

void *authenticationThreadFunc(void *pServer) {

    sem_t *sem = sem_open("Authentication", O_CREAT, 0600, 0);//semafor tworzy plik
    if (sem == SEM_FAILED) {
        printf("%s\n", strerror(errno));
        return NULL;
    }
    int fd = shm_open("AuthenticationSHM", O_CREAT | O_RDWR, 0600);
    ftruncate(fd, sizeof(authentication));
    authentication *playersAuthentication = (authentication *) mmap(NULL, sizeof(authentication),
                                                                    PROT_READ | PROT_WRITE,
                                                                    MAP_SHARED, fd, 0);
    assert (playersAuthentication != MAP_FAILED);

    playersAuthentication->playerNumber = 1;
    sem_init(&playersAuthentication->authenticationPost, 1, 1);

    while (playersAuthentication->playerNumber != 3) {
        sem_wait(sem);
        sem_wait(&playersAuthentication->authenticationPost);

        playersAuthentication->playerNumber++;

        sem_post(&playersAuthentication->authenticationPost);
    }

    sem_close(sem);
    close(fd);
    shm_unlink("AuthenticationSHM");
    munmap(playersAuthentication, sizeof(authentication));
}

void *reConnectPlayerOne(void *pServer) {

    sem_t *sem = sem_open("Authentication", O_CREAT, 0600, 0);
    if (sem == SEM_FAILED) {
        printf("%s\n", strerror(errno));
        return NULL;
    }
    int fd = shm_open("AuthenticationSHM", O_CREAT | O_RDWR, 0600); //zwraca id shm
    ftruncate(fd, sizeof(authentication));
    authentication *playersAuthentication = (authentication *) mmap(NULL, sizeof(authentication),
                                                                    PROT_READ | PROT_WRITE,
                                                                    MAP_SHARED, fd, 0);
    assert (playersAuthentication != MAP_FAILED);

    playersAuthentication->playerNumber = 1;
    sem_init(&playersAuthentication->authenticationPost, 1, 1);

    sem_wait(sem);
    sem_wait(&playersAuthentication->authenticationPost);

    sem_post(&playersAuthentication->authenticationPost);

    sem_close(sem);
    close(fd);
    shm_unlink("AuthenticationSHM");
    munmap(playersAuthentication, sizeof(authentication));
}

void *reConnectPlayerTwo(void *pServer) {

    sem_t *sem = sem_open("Authentication", O_CREAT, 0600, 0);//semafor tworzy plik
    if (sem == SEM_FAILED) {
        printf("%s\n", strerror(errno));
        return NULL;
    }
    int fd = shm_open("AuthenticationSHM", O_CREAT | O_RDWR, 0600); //zwraca id shm
    ftruncate(fd, sizeof(authentication));
    authentication *playersAuthentication = (authentication *) mmap(NULL, sizeof(authentication),
                                                                    PROT_READ | PROT_WRITE,
                                                                    MAP_SHARED, fd, 0);
    assert (playersAuthentication != MAP_FAILED);

    playersAuthentication->playerNumber = 2;
    sem_init(&playersAuthentication->authenticationPost, 1, 1);

    sem_wait(sem);
    sem_wait(&playersAuthentication->authenticationPost);

    sem_post(&playersAuthentication->authenticationPost);

    sem_close(sem);
    close(fd);
    shm_unlink("AuthenticationSHM");
    munmap(playersAuthentication, sizeof(authentication));
}