//
// Created by kamil on 23.10.2022.
//
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include "../headers/server.h"
#include "../headers/beast.h"

infoServer *serverInit() {

    infoServer *server = (infoServer *) calloc(1, sizeof(infoServer));
    if (server == NULL) {
        return NULL;
    }

    server->board = mapLoad("map.txt");
    server->server_PID = getpid();
    server->playersNumber = 0;
    server->beastNumber = 0;
    server->coinNumber = 0;
    server->treasureNumber = 0;
    sem_init(&server->update, 0, 1);

    return server;
}

void serverRun(infoServer *server) {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    WINDOW *okno1;    // Okna programu
    int znak;

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury
    start_color();
    init_colors();

    okno1 = newwin(LINES, COLS, 0, 0);
    box(okno1, 0, 0);            // Standardowe ramki
    wrefresh(okno1);

    player players[3];
    pthread_t player_thr[3];
    for (int i = 1; i <= 2; i++) {
        players[i] = initPlayer(i, server->board, server->server_PID);
        pthread_create(&player_thr[i], NULL, player_connection, &players[i]);
    }

    sem_t *sem = sem_open("Authentication", O_CREAT, 0600, 0);//semafor tworzy plik
    if (sem == SEM_FAILED) {
        return;
    }

    int fd = shm_open("AuthenticationSHM", O_CREAT | O_RDWR, 0600); //zwraca id shm
    ftruncate(fd, sizeof(authentication));
    authentication *playersAuthentication = (authentication *) mmap(NULL, sizeof(authentication),
                                                                    PROT_READ | PROT_WRITE,
                                                                    MAP_SHARED, fd, 0);
    assert (playersAuthentication != MAP_FAILED);

    playersAuthentication->playerNumber = 1;
    sem_init(&playersAuthentication->authenticationPost, 1, 1);
    sem_init(&playersAuthentication->authenticationStartGame, 1, 0);

    while (playersAuthentication->playerNumber != 3) {
        mvwprintw(okno1, 5, 5, "Wait for player number: %d", playersAuthentication->playerNumber);
        wrefresh(okno1);
        sem_wait(sem);
        sem_wait(&playersAuthentication->authenticationPost);

        playersAuthentication->playerNumber++;

        sem_post(&playersAuthentication->authenticationPost);
    }

    for (int i = 0; i < 2; i++) {
        sem_post(&playersAuthentication->authenticationStartGame);
    }

    sem_close(sem);
    close(fd);
    shm_unlink("AuthenticationSHM");
    munmap(playersAuthentication, sizeof(authentication));

    beast beast;
    pthread_t beast_thr;

    do {
        mapPrint(5, 5, okno1, server->board);
        //mapPrintFragment(5, 5, okno1, players[1].map);
        serverInfoPrintServer(5, 55, okno1, *server);
        serverInfoPrintPlayers(7, 55, okno1, players);
        wrefresh(okno1);
        printLegend(19, 55, okno1);

        znak = wgetch(okno1);// Oczekiwanie na klawisz
        if (znak == 'w') {
            players[1].move = 66;
        } else if (znak == 'a') {
            players[1].move = 68;
        } else if (znak == 'd') {
            players[1].move = 67;
        } else if (znak == 's') {
            players[1].move = 65;
        }

        movePlayer(server->board, &players[1]);
        if (znak == 'c' && server->coinNumber < 10) {
            generateRandomCoin(server->board);
            server->coinNumber++;
        } else if (znak == 't' && server->treasureNumber < 5) {
            generateRandomTreasure(server->board);
            server->treasureNumber++;
        } else if (znak == 'T' && server->treasureNumber < 5) {
            generateRandomLargeTreasure(server->board);
            server->treasureNumber++;
        } else if ((znak == 'B' || znak == 'b') && server->beastNumber < 1) {
            server->beastNumber++;
            beast = initBeast(server->board, server->server_PID);
            pthread_create(&beast_thr, NULL, beastConnection, &beast);
        }

        mapPrint(5, 5, okno1, server->board);
        werase(okno1);
        box(okno1, 0, 0);            // Standardowe ramki
        wrefresh(okno1);

    } while (znak != 'q' && znak != 'Q');

    endwin();// Koniec pracy z CURSES
    delwin(okno1);        // Usuniecie okien
    mapDestroy(server->board);
    free(server);
}

void serverInfoPrintServer(int y, int x, WINDOW *window, infoServer Server) {
    mvwprintw(window, y++, x, "Server's playerPID: %d", Server.server_PID);
    mvwprintw(window, y++, x + 1, "Campsite X/Y: %d/%d", Server.board->campLocationX, Server.board->campLocationY);
    mvwprintw(window, y++, x + 1, "Round number: %d", 0);
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

void serverInfoPrintPlayers(int y, int x, WINDOW *window, player player[]) {
    for (int i = 1; i <= 2; i++) {
        mvwprintw(window, y + 2, x + 15 + (i * 25), "%s", player[i].name);
        mvwprintw(window, y + 3, x + 15 + (i * 25), "%s", player[i].playerPID);
        mvwprintw(window, y + 4, x + 15 + (i * 25), "%d", player[i].round_number);
        mvwprintw(window, y + 5, x + 15 + (i * 25), "%d", player[i].deaths);
        mvwprintw(window, y + 6, x + 15 + (i * 25), "%d/%d", player[i].pos.x, player[i].pos.y);
        mvwprintw(window, y + 8, x + 15 + (i * 25), "%d", player[i].coinsCarried);
        mvwprintw(window, y + 9, x + 15 + (i * 25), "%d", player[i].coinsInDeposit);
    }
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

void *player_connection(void *playerStruct) {
    player *pPlayer = (player *) playerStruct;

    char semaforName[100] = {0};
    strcat(semaforName, "/msg_signal");
    strcat(semaforName, pPlayer->name);

    sem_t *sem = sem_open(semaforName, O_CREAT, 0600, 0);//semafor tworzy plik
    if (sem == SEM_FAILED) {
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

    sem_init(&SHPlayer->received_data, 1, 1); // shared, signaled

    while (SHPlayer->move != 'q') {
        sem_wait(sem);
        sem_wait(&SHPlayer->received_data);

        memcpy(pPlayer, SHPlayer, sizeof(player));
        sem_post(&SHPlayer->received_data);
    }

    munmap(SHPlayer, sizeof(player));
    close(fd);
    shm_unlink(shmName);
    sem_close(sem);
}

void *beastConnection(void *beastStruct) {
    beast *pBeast = (beast *) beastStruct;
    point newPos;
    while (1) {
        if (beastPull(beastStruct, &newPos) == 0) {
            beastMove(beastStruct, &newPos);
            break;
        }
    }

}