//
// Created by kamil on 29.10.2022.
//
#define _GNU_SOURCE

#include <string.h>
#include <bits/types/wint_t.h>
#include <wchar.h>
#include <signal.h>
#include "../headers/client.h"
#include "../headers/server.h"
#include <pthread.h>


void connectToServer() {

    setlocale(LC_ALL, "");
    start_color();
    WINDOW *okno1;    // Okna programu

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury
    init_colors();

    okno1 = newwin(LINES, COLS, 0, 0);
    box(okno1, 0, 0);            // Standardowe ramki

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

    mvwprintw(okno1, 5, 5, "Wait for player number: %d", playerID);
    wrefresh(okno1);

    sem_post(&playerAuthentication->authenticationPost);
    sem_post(semAuthentication);
    sem_wait(&playerAuthentication->authenticationStartGame);
    werase(okno1);

    munmap(playerAuthentication, sizeof(authentication));
    shm_unlink("/AuthenticationSHM");
    close(fdAuthentication);
    sem_close(semAuthentication);

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
                wrefresh(okno1);

                // if (pthread_tryjoin_np(keyboardInput, (void *) &keyInfo) == 0) {
                mvwprintw(okno1, 5, 5, "Wait for player number: %d", keyInfo.key);
                wrefresh(okno1);
                pthread_create(&keyboardInput, NULL, keyboardInputFuncPlayer, &keyInfo);
                // }
                join_shm->move = keyInfo.key;
                sem_wait(&join_shm->received_data);
                sem_post(&join_shm->received_data);
                sem_post(sem);
                // flushinp();
            } while (join_shm->move != 'q' && join_shm->move != 'Q');

            sem_close(sem);
            munmap(join_shm, sizeof(struct player));
            shm_unlink("/gameSO2_Join_SHMPlayer1");
            close(fd);
            endwin();// Koniec pracy z CURSES
            delwin(okno1);        // Usuniecie okien
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

            while (1) {
                mapPrintFragment(5, 5, okno1, join_shmPlayer2->map);
                wrefresh(okno1);
                join_shmPlayer2->move = wgetch(okno1);


                if (join_shmPlayer2->move == 'q') {
                    break;
                }

                sem_wait(&join_shmPlayer2->received_data);
                sem_post(&join_shmPlayer2->received_data);
                sem_post(semPlayer2);
                flushinp();
            }

            sem_close(semPlayer2);
            munmap(join_shmPlayer2, sizeof(struct player));
            shm_unlink("/gameSO2_Join_SHMPlayer2");
            close(fdPlayer2);
            endwin();// Koniec pracy z CURSES
            delwin(okno1);        // Usuniecie okien
            break;
        default:
            return;
    }
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
    if (keyFunc()) {
        key = getch();
        pthread_mutex_lock(&info->mutex);
        info->key = key;
        pthread_mutex_unlock(&info->mutex);
    }

    return NULL;
}





