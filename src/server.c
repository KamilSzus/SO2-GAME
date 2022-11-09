//
// Created by kamil on 23.10.2022.
//


#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include "../headers/server.h"
#include "../headers/player.h"

infoServer *serverInit() {

    infoServer *server = (infoServer *) calloc(1, sizeof(infoServer));
    if (server == NULL) {
        return NULL;
    }

    // sem_t *sem = sem_open("/gameSO2_Join_Signal", O_CREAT, 0600, 0);
    // if (sem == SEM_FAILED) {
    //     return NULL;
    // }

    server->board = mapLoad("map.txt");
    server->server_PID = getpid();
    server->playersNumber = 0;
    server->beastNumber = 0;
    server->coinNumber = 0;
    server->treasureNumber = 0;
    sem_init(&server->update, 0, 1);

    //server->sharedMemoryJoin.fd = shm_open("/gameSO2_Join_SHM", O_CREAT | O_RDWR, 0600); //zwraca id shm
    //ftruncate(server->sharedMemoryJoin.fd, sizeof(player));
    //server->sharedMemoryJoin.userJoin = (struct player *) mmap(NULL, sizeof(struct player), PROT_READ | PROT_WRITE,
    //                                                            MAP_SHARED, server->sharedMemoryJoin.fd, 0);

    //userJoin *request = (struct userJoin *) server->sharedMemoryJoin.userJoin;
    //sem_init(&request->server_open_request, 1, 1);

    //player players[4];
    //pthread_t player_thr[4];
//
    //for (int i = 0; i < 4; i++) {
    //    players[i] = initPlayer(i, server->board, server->server_PID);
    //    printf("%d %d ", players[i].spawn_location.x, players[i].spawn_location.y);
    //    pthread_create(&player_thr[i], NULL, player_connection, &players[i]);
    //}
//
    //int terminate = 0, counter = 0;
    //while (!terminate) {
    //    sem_wait(sem);
    //    sem_wait(&request->server_open_request);
    //    printf("[%03d:%03d]: %s\n", server->server_PID, request->player_pid, request->payload);
    //    terminate = strcasecmp(request->payload, "quit") == 0;
    //    sem_post(&request->server_open_request);
    //}

    return server;
}

void serverRun(infoServer *server) {
    setlocale(LC_ALL, "");
    start_color();
    srand(time(NULL));
    WINDOW *okno1;    // Okna programu
    int znak;

    player players[4];
    pthread_t player_thr[4];
    for (int i = 0; i < 4; i++) {
        players[i] = initPlayer(i, server->board, server->server_PID);
        pthread_create(&player_thr[i], NULL, player_connection, &players[i]);
    }

    initscr();    // Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);    // Nie wyswietlaj kursora
    noecho();    // Nie wyswietlaj znakow z klawiatury
    init_colors();

    okno1 = newwin(LINES, COLS, 0, 0);
    box(okno1, 0, 0);            // Standardowe ramki
    wrefresh(okno1);

    do {
        mapPrint(5, 5, okno1, server->board);
        serverInfoPrintServer(5, 55, okno1, *server);
        serverInfoPrintPlayers(7, 55, okno1, players);
        wrefresh(okno1);
        printLegend(19, 55, okno1);
        znak = wgetch(okno1);// Oczekiwanie na klawisz

        if (znak == 'c' && server->coinNumber < 10) {
            generateRandomCoin(server->board);
            server->coinNumber++;
        } else if (znak == 't' && server->treasureNumber < 5) {
            generateRandomTreasure(server->board);
            server->treasureNumber++;
        } else if (znak == 'T' && server->treasureNumber < 5) {
            generateRandomLargeTreasure(server->board);
            server->treasureNumber++;
        } else if ((znak == 'B' || znak == 'b') && server->beastNumber < 2) {
            server->beastNumber++;
        }

        mapPrint(5, 5, okno1, server->board);
        wrefresh(okno1);

    } while (znak != 'q' && znak != 'Q');


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
    //mvwprintw(window, y++, x+ 15,"%s",p);
    mvwprintw(window, y++, x + 1, "Deaths:       ");
    mvwprintw(window, y++, x + 1, "Curr X/Y:     ");
    mvwprintw(window, y++, x, "Coins        ");
    mvwprintw(window, y++, x + 1, "carried       ");
    mvwprintw(window, y, x + 1, "brought       ");
}

void serverInfoPrintPlayers(int y, int x, WINDOW *window, player player[]) {
    for (int i = 0; i < 4; i++) {
        mvwprintw(window, y + 2, x + 15 + (i * 25), "%s", player[i].name);
        mvwprintw(window, y + 3, x + 15 + (i * 25), "%s", player[i].playerPID);
        mvwprintw(window, y + 4, x + 15 + (i * 25), "%d", player[i].round_number);
        mvwprintw(window, y + 5, x + 15 + (i * 25), "%d", player[i].coins_found);//smierc
        mvwprintw(window, y + 6, x + 15 + (i * 25), "%d/%d", player[i].spawn_location.x, player[i].spawn_location.y);

        mvwprintw(window, y + 8, x + 15 + (i * 25), "%d", player[i].coins_found);
        mvwprintw(window, y + 9, x + 15 + (i * 25), "%d", player[i].coins_brought);
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
    //printf("%d %d ", pPlayer->spawn_location.x, pPlayer->spawn_location.y);

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

        //printf("HALOO?");

        printf("%c", SHPlayer->move);

        sem_post(&SHPlayer->received_data);
    }
//    printf("%d %d ", SHPlayer->spawn_location.x, SHPlayer->spawn_location.y);

}