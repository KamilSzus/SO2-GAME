#include <stdio.h>


#include "board.h"

int main() {

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

    mapPrint(5, 5, okno1, map);

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
