cmake_minimum_required(VERSION 3.16)
project(SO2 C)

set(CMAKE_C_STANDARD 99)
add_link_options(-pthread)
add_link_options(-lncursesw)
include_directories(${CURSES_INCLUDE_DIR})


add_executable(SO2 main.c src/client.c headers/client.h headers/board.h src/board.c src/server.c headers/server.h src/player.c headers/player.h headers/point.h src/beast.c headers/beast.h)
target_link_libraries(SO2 ${CURSES_LIBRARIES})
target_link_libraries(SO2 ncurses)
target_link_libraries(SO2 rt)

add_executable(SO2Client src/client.c headers/client.h clientMain.c headers/board.h src/board.c src/server.c headers/server.h src/player.c headers/player.h src/beast.c headers/beast.h)
target_link_libraries(SO2Client ${CURSES_LIBRARIES})
target_link_libraries(SO2Client ncurses)
target_link_libraries(SO2Client rt)