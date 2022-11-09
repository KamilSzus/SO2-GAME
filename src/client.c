//
// Created by kamil on 29.10.2022.
//

#include <string.h>
#include "../headers/client.h"
#include "../headers/server.h"


void connectToServer() {
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
    printf("Identyfikator sesji: %d; pdata=%p......\n", join_shm->playerPID, join_shm);
    while (1) {
        char msg[1024];
        printf("Podaj tekst: ");
        join_shm->move = fgetc(stdin);
        if (!join_shm->move) break;
        if (*msg)
            msg[strlen(msg) - 1] = '\x0';

        sem_wait(&join_shm->received_data);
        //strcpy((char *) join_shm->move, msg);
        join_shm->playerPID = getpid();
        sem_post(&join_shm->received_data);
        sem_post(sem);
    }

    //sem_post(&join_shm->server_new_request);
    //sem_wait(&join_shm->server_checked_request);

    sem_close(sem);
    munmap(join_shm, sizeof(struct player));
    close(fd);
}






