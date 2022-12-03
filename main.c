


#include "headers/server.h"

int main() {
    infoServer *server = serverInit();
    serverRun(server);

    return 0;
}
