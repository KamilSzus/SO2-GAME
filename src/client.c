//
// Created by kamil on 29.10.2022.
//

#include <string.h>
#include "../headers/client.h"
#include "../headers/server.h"


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
    //printf("Identyfikator sesji: %d; pdata=%p......\n", join_shm->playerPID, join_shm);
    join_shm->playerPID = getpid();

    while (1) {
        mapPrintFragment(5, 5, okno1, join_shm->map);
        wrefresh(okno1);
        join_shm->move = wgetch(okno1);


        if (join_shm->move == 'q') {
            break;
        }

        sem_wait(&join_shm->received_data);
        sem_post(&join_shm->received_data);
        sem_post(sem);
    }

    //sem_post(&join_shm->server_new_request);
    //sem_wait(&join_shm->server_checked_request);

    sem_close(sem);
    munmap(join_shm, sizeof(struct player));
    close(fd);
    endwin();// Koniec pracy z CURSES
    delwin(okno1);        // Usuniecie okien
}






