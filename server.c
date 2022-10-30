//
// Created by kamil on 23.10.2022.
//

#include <strings.h>
#include "server.h"

void serverJoinPlayer(infoServer *pServer, userJoin *pJoin);

infoServer *serverInit() {

    infoServer *server = (infoServer *) calloc(1, sizeof(infoServer));
    if (server == NULL) {
        return NULL;
    }

    sem_t* sem = sem_open("/gameSO2_Join_Signal", O_CREAT, 0600, 0);
    if(sem==SEM_FAILED){
        return NULL;
    }

    server->board = mapLoad("map.txt");
    server->server_PID = getpid();
    //sem_init(&server->update, 0, 1);

    server->sharedMemoryJoin.fd = shm_open("/gameSO2_Join_SHM", O_CREAT | O_RDWR, 0600); //zwraca id shm
    ftruncate(server->sharedMemoryJoin.fd, sizeof(userJoin));
    server->sharedMemoryJoin.userJoin = (struct userJoin *) mmap(NULL, sizeof(struct userJoin), PROT_READ | PROT_WRITE,
                                                                 MAP_SHARED, server->sharedMemoryJoin.fd, 0);

    userJoin *request = (struct userJoin *) server->sharedMemoryJoin.userJoin;
    request->board = server->board;
    sem_init(&request->server_open_request, 1, 1);

    int terminate = 0, counter = 0;
    while (!terminate) {
        sem_wait(sem);
        sem_wait(&request->server_open_request);
        printf("[%03d:%03d]: %s\n", server->server_PID, request->player_pid, request->payload);
        terminate = strcasecmp(request->payload, "quit") == 0;
        sem_post(&request->server_open_request);
    }

    //serverJoinPlayer(server, request);

    //dodac timer
    return server;
}

void serverJoinPlayer(infoServer *pServer, userJoin *pJoin) {

    //while (pJoin->isSharedMemoryRunning)
    //{
    //    //request->join_status = JOIN_UNKNOWN;
//
    //    sem_post(&pJoin->server_open_request);
    //    sem_wait(&pJoin->server_new_request);
//
    //    if (pJoin->isSharedMemoryRunning == false)
    //        break;
//
    //    pthread_mutex_lock(&pServer->mutex);
//
    //    //int test = server_entity_add(server, request);
    //    //if (test)
    //    //    request->join_status = JOIN_REJECTED;
    //    //else
    //    //    request->join_status = JOIN_ACCEPTED;
//
    //    sem_post(&pJoin->server_checked_request);
    //    sem_wait(&pJoin->server_new_request);
//
    //    pthread_mutex_unlock(&pServer->mutex);
    //}
//
    //pJoin->isSharedMemoryRunning = false;

}

void serverRun(infoServer *server) {
    setlocale(LC_ALL, "");
    WINDOW *okno1, *okno2;    // Okna programu
    int znak;

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury

    okno1 = newwin(LINES, COLS - 20, 0, 0);
    box(okno1, 0, 0);            // Standardowe ramki
    mvwprintw(okno1, 1, 1, "%s", "Okno nr 1");
    mvwprintw(okno1, LINES - 2, 1, "%s", "Spacja konczy program");
    wrefresh(okno1);

    okno2 = newwin(LINES, 20, 0, COLS - 20);
    box(okno2, 0, 0);
    mvwprintw(okno2, 1, 1, "%s", "Okno nr 2");
    wrefresh(okno2);

    do {
        mapPrint(5, 5, okno1, server->board);
        serverInfoPrint(5, 55, okno1);
        wrefresh(okno1);

        znak = wgetch(okno1);        // Oczekiwanie na klawisz
        mvwprintw(okno2, 2, 1, "Nacisnale: %c", znak);
        wrefresh(okno2);
    } while (znak == 'q' || znak == 'Q');


    //while (1) {
    //    //sem_wait(&server->update);
    //
    //    // werase(map);
    //    // maintain_map_print(0, 0, map, server->map);
    //    // maintain_map_print(0, 0, map, server->map_entities);
    //    // wrefresh(map);
    //    //
    //    // werase(info);
    //    // maintain_server_info_print(0, 0, info, server);
    //    // wrefresh(info);
    //
    //
    //
    //    //server_entity_thread_function(server);
    //
    //    if (pthread_tryjoin_np(stdinController, (void *) &pressed_key) == 0) {
    //        char *decoded = (char *) pressed_key;
    //        mvwprintw(okno2, 0, 0, "Button pressed: %02x %02x %02x %02x", decoded[0], decoded[1], decoded[2],
    //                  decoded[3]);
    //        wrefresh(okno2);
    //
    //        if (*pressed_key == 'q' || *pressed_key == 'Q') {
    //            free(pressed_key);
    //            break;
    //        } else if (*pressed_key == 'b' || *pressed_key == 'B') {
    //            if (server->beasts_count >= MAX_BEASTS - 1) {
    //                mvwprintw(okno2, 1, 0, "Cannot add beasts");
    //                wrefresh(okno2);
    //            } else {
    //                for (int i = 0; i < MAX_BEASTS; ++i) {
    //                    if (beast[i] == NULL) {
    //                        beast[i] = entity_beast_add();
    //                        break;
    //                    }
    //
    //                }
    //            }
    //        } else {
    //            maintain_map_add_symbol(server->map, *pressed_key);
    //        }
    //
    //
    //        free(pressed_key);
    //
    //       // pthread_create(&stdinController, NULL, maintain_stdin_controller, NULL);
    //    }
    //
    //}


    endwin();// Koniec pracy z CURSES
    delwin(okno1);        // Usuniecie okien
    delwin(okno2);
    mapDestroy(server->board);
}

//void *server_join_requests_thread_function(void *arg)
//{
//    ServerInfo *server = (ServerInfo *) arg;
//    SHMController *controller = &server->join_request;
//    JoinRequest *request = (JoinRequest *) controller->memory_map;
//
//    while (controller->is_thread_running)
//    {
//        request->join_status = JOIN_UNKNOWN;
//
//        sem_post(&request->server_open_request);
//        sem_wait(&request->server_new_request);
//
//        if (controller->is_thread_running == false)
//            break;
//
//        pthread_mutex_lock(&server->mutex);
//
//        int test = server_entity_add(server, request);
//        if (test)
//            request->join_status = JOIN_REJECTED;
//        else
//            request->join_status = JOIN_ACCEPTED;
//
//        sem_post(&request->server_checked_request);
//        sem_wait(&request->server_new_request);
//
//        pthread_mutex_unlock(&server->mutex);
//    }
//
//    controller->is_thread_running = false;
//
//    return NULL;
//}