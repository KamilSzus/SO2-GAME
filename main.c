#include <stdio.h>

#include <ncurses.h>
#include <curses.h>
#include <malloc.h>
#include "board.h"

boardData *mapLoad(char *filename) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    int wight, height;

    fscanf(file, "%d %d\n", &wight, &height);

    boardData *map = mapInit(wight, height);
    if (!map) {
        mapDestroy(map);
        fclose(file);
        return NULL;
    }

    while (!feof(file)) {
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->wight; j++) {
                char temp = (char) getc(file);
                if (temp == '\n' || temp == '\r') {
                    break;
                }
                //printf("%c" , temp);
                *(*(map->map + i) + j) = temp;
            }
        }

    }

    fclose(file);
    return map;
}

boardData *mapInit(int wight, int height) {
    boardData *map = (boardData *) calloc(1, sizeof(boardData));
    if (!map) {
        return NULL;
    }
    map->wight = wight;
    map->height = height;

    if (map->wight <= 0 || map->height <= 0) {
        return NULL;
    }

    map->map = (char **) malloc(map->height * sizeof(char *));
    if (!map->map) {
        return NULL;
    }
    for (int i = 0; i < map->height; ++i) {
        *(map->map + i) = (char *) malloc(map->wight * sizeof(char));
        if (!*(map->map + i)) {
            return NULL;
        }
    }

    return map;
}

void mapDestroy(boardData *map) {
    if (!map || !map->map || map->height < 0 || map->wight < 0 || map->campLocationX < 0 || map->campLocationY < 0) {
        return;
    }
    for (int i = 0; i < map->height; i++) {
        free(*(map->map + i));
    }
    free(map->map);
    free(map);
    map = NULL;
}

int main() {

    boardData *map = mapLoad("map.txt");

    WINDOW *okno1, *okno2;    // Okna programu
    int znak;

    //for (int i = 5; i < map->height + 5; i++) {
    //    for (int j = 5; j < map->wight + 5; j++) {
    //        printf( "%c", *(*(map->map + i - 5) + j - 5));
    //    }
    //}
//
    //printf("ddd");

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

    int licznik = 0;
    for (int i = 5; i < map->height + 5; i++) {
        for (int j = 5; j < map->wight + 5; j++) {
            char object = map->map[i - 5][j - 5];
            if (object == '\0') continue;
            //  printf("%d %d ",i-5, j-5);
            //printf("%c ", object);
            printf("%d ", licznik++);

            mvwprintw(okno1, i, j, "%c", object);
            wrefresh(okno1);
        }
    }

    do {
        znak = wgetch(okno1);        // Oczekiwanie na klawisz
        mvwprintw(okno2, 2, 1, "Nacisnale: %c", znak);
        wrefresh(okno2);
    } while (znak != 32);

    delwin(okno1);        // Usuniecie okien
    delwin(okno2);
    mapDestroy(map);
    endwin();        // Koniec pracy z CURSES
    return (0);
}
