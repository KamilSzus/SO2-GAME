


#include "server.h"

int main() {
    infoServer* server = serverInit();
    //serverRun(server);


    munmap(server->sharedMemoryJoin.userJoin, sizeof(struct userJoin));
    close(server->sharedMemoryJoin.fd);
    shm_unlink("/gameSO2_Join_SHM");
    //sem_close(sem);
    return 0;
}
