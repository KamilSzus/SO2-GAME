//
// Created by kamil on 22.11.2022.
//

#include "../headers/point.h"

int isEqual(point one, point two) {
    if (one.x == two.x && one.y == two.y) {
        return 1;
    } else {
        return 0;
    }
}