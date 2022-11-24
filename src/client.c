//
// Created by kamil on 29.10.2022.
//
#define _GNU_SOURCE

#include <string.h>
#include <signal.h>
#include "../headers/client.h"
#include "../headers/server.h"
#include <pthread.h>


void connectToServer() {


    sem_t *semAuthentication = sem_open("/Authentication", 0);
    if (semAuthentication == SEM_FAILED) {
        return;
    }

    int fdAuthentication = shm_open("/AuthenticationSHM", O_RDWR, 0600);
    if (fdAuthentication == -1) {
        return;
    }

    ftruncate(fdAuthentication, sizeof(authentication));
    authentication *playerAuthentication = (authentication *) mmap(NULL, sizeof(authentication), PROT_WRITE | PROT_READ,
                                                                   MAP_SHARED, fdAuthentication, 0);

    int playerID = playerAuthentication->playerNumber;

    sem_post(&playerAuthentication->authenticationPost);
    sem_post(semAuthentication);

    setlocale(LC_ALL, "");
    start_color();
    WINDOW *okno1;    // Okna programu

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury
    init_colors();

    okno1 = newwin(LINES, COLS, 0, 0);
    box(okno1, 0, 0);            // Standardowe ramki

    pthread_t keyboardInput;
    keyThreadInfoPlayer keyInfo;
    keyInfo.key = 0;
    pthread_create(&keyboardInput, NULL, keyboardInputFuncPlayer, &keyInfo);
    if (pthread_mutex_init(&keyInfo.mutex, NULL) != 0) {
        return;
    }

    switch (playerID) {
        case 1:
            wrefresh(okno1);
            sem_t *sem = sem_open("/msg_signalPlayer1", 0);
            if (sem == SEM_FAILED) {
                return;
            }

            int fd = shm_open("/gameSO2_Join_SHMPlayer1", O_RDWR, 0600);
            if (fd == -1) {
                return;
            }

            ftruncate(fd, sizeof(player));
            player *join_shm = (player *) mmap(NULL, sizeof(player), PROT_WRITE | PROT_READ,
                                               MAP_SHARED, fd, 0);
            join_shm->playerPID = getpid();
            do {
                mapPrintFragment(5, 5, okno1, join_shm->map);
                clientInfoPrintServer(5, 55, okno1, *join_shm);
                printLegend(19, 55, okno1);
                clientInfoPrintPlayers(7, 55, 1, okno1, *join_shm);
                wrefresh(okno1);

                if (join_shm->isPlayerMoved == 0) {
                    pthread_create(&keyboardInput, NULL, keyboardInputFuncPlayer, &keyInfo);
                }

                join_shm->move = keyInfo.key;
                keyInfo.key = 0;
                sem_wait(&join_shm->received_data);
                sem_post(&join_shm->received_data);
                sem_post(sem);
                flushinp();
            } while (join_shm->move != 'q' && join_shm->move != 'Q');

            sem_close(sem);
            munmap(join_shm, sizeof(player));
            shm_unlink("/gameSO2_Join_SHMPlayer1");
            close(fd);
            endwin();// Koniec pracy z CURSES
            delwin(okno1);// Usuniecie okien
            break;

        case 2:
            wrefresh(okno1);

            sem_t *semPlayer2 = sem_open("/msg_signalPlayer2", 0);
            if (semPlayer2 == SEM_FAILED) {
                return;
            }

            int fdPlayer2 = shm_open("/gameSO2_Join_SHMPlayer2", O_RDWR, 0600);
            if (fdPlayer2 == -1) {
                return;
            }

            ftruncate(fdPlayer2, sizeof(player));
            player *join_shmPlayer2 = (player *) mmap(NULL, sizeof(player), PROT_WRITE | PROT_READ,
                                                      MAP_SHARED, fdPlayer2, 0);
            join_shmPlayer2->playerPID = getpid();

            do {
                mapPrintFragment(5, 5, okno1, join_shmPlayer2->map);
                clientInfoPrintServer(5, 55, okno1, *join_shmPlayer2);
                printLegend(19, 55, okno1);
                clientInfoPrintPlayers(7, 55, 1, okno1, *join_shmPlayer2);
                wrefresh(okno1);

                if (join_shmPlayer2->isPlayerMoved == 0) {
                    pthread_create(&keyboardInput, NULL, keyboardInputFuncPlayer, &keyInfo);
                }

                join_shmPlayer2->move = keyInfo.key;
                keyInfo.key = 0;
                sem_wait(&join_shmPlayer2->received_data);
                sem_post(&join_shmPlayer2->received_data);
                sem_post(semPlayer2);
                flushinp();
            } while (join_shmPlayer2->move != 'q' && join_shmPlayer2->move != 'Q');

            sem_close(semPlayer2);
            munmap(join_shmPlayer2, sizeof(player));
            shm_unlink("/gameSO2_Join_SHMPlayer2");
            close(fdPlayer2);
            endwin();// Koniec pracy z CURSES
            delwin(okno1);        // Usuniecie okien
            break;
        default:
            return;
    }

    munmap(playerAuthentication, sizeof(authentication));
    shm_unlink("/AuthenticationSHM");
    close(fdAuthentication);
    sem_close(semAuthentication);

}

int keyFuncPlayer(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void *keyboardInputFuncPlayer(void *pKey) {
    keyThreadInfoPlayer *info = (keyThreadInfoPlayer *) pKey;

    int key = 0;
    if (keyFuncPlayer()) {
        key = getch();
        pthread_mutex_lock(&info->mutex);
        info->key = key;
        pthread_mutex_unlock(&info->mutex);
    }

    return NULL;
}

void clientInfoPrintPlayers(int y, int x, int i, WINDOW *window, player player) {
    mvwprintw(window, y + 2, x + 15 + (i * 25), "%s", player.name);
    mvwprintw(window, y + 3, x + 15 + (i * 25), "HUMAN");
    mvwprintw(window, y + 4, x + 15 + (i * 25), "%d", player.deaths);
    mvwprintw(window, y + 5, x + 15 + (i * 25), "%d/%d", player.pos.x, player.pos.y);
    mvwprintw(window, y + 7, x + 15 + (i * 25), "%d", player.coinsCarried);
    mvwprintw(window, y + 8, x + 15 + (i * 25), "%d", player.coinsInDeposit);
}

void clientInfoPrintServer(int y, int x, WINDOW *window, player player) {
    mvwprintw(window, y++, x, "Server's: %d", player.server_PID);
    mvwprintw(window, y++, x + 1, "Campsite X/Y: unknown");
    mvwprintw(window, y++, x + 1, "Round number: %d", player.roundNumber);
    y++;
    mvwprintw(window, y++, x, "Player:   ");
    mvwprintw(window, y++, x + 1, "Type:       ");
    mvwprintw(window, y++, x + 1, "Deaths:       ");
    mvwprintw(window, y++, x + 1, "Curr X/Y:     ");
    mvwprintw(window, y++, x, "Coins        ");
    mvwprintw(window, y++, x + 1, "carried       ");
    mvwprintw(window, y, x + 1, "brought       ");
}



