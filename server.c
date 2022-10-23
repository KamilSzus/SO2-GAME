//
// Created by kamil on 23.10.2022.
//

#include <locale.h>
#include <pthread.h>
#include <semaphore.h>
#include "server.h"
#include "board.h"

void serverInit(infoServer *server) {
    setlocale(LC_ALL, "");

    boardData *map = mapLoad("map.txt");

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

    //pthread_t stdinController;
    // pthread_create(&stdinController, NULL, maintain_stdin_controller, NULL);


    do {
        mapPrint(5, 5, okno1, map);
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
    mapDestroy(map);
}
