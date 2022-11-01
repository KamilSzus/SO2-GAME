//
// Created by kamil on 29.10.2022.
//

#include <string.h>
#include "../headers/client.h"
#include "../headers/server.h"


void connectToServer() {
    sem_t *sem = sem_open("/gameSO2_Join_Signal", 0);
    if (sem == SEM_FAILED) {
        return;
    }
    int fd = shm_open("/gameSO2_Join_SHM", O_RDWR, 0600);
    if (fd == -1) {
        return;
    }
    ftruncate(fd, sizeof(userJoin));
    userJoin *join_shm = (userJoin *) mmap(NULL, sizeof(userJoin), PROT_WRITE | PROT_READ,
                                           MAP_SHARED, fd, 0);

    join_shm->player_pid = getpid();
    printf("Identyfikator sesji: %d; pdata=%p......\n", join_shm->player_pid, join_shm);
    //printf("%s",join_shm->board->map);
    while (1) {
        char msg[1024];
        printf("Podaj tekst: ");
        char *p = fgets(msg, 1024, stdin);
        if (!p) break;
        if (*msg)
            msg[strlen(msg) - 1] = '\x0';

        sem_wait(&join_shm->server_open_request);
        strcpy(join_shm->payload, msg);
        join_shm->player_pid = getpid();
        sem_post(&join_shm->server_open_request);

        sem_post(sem);
    }

    sem_post(&join_shm->server_new_request);
    sem_wait(&join_shm->server_checked_request);

    sem_close(sem);
    munmap(join_shm, sizeof(struct userJoin));
    close(fd);
}






