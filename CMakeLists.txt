cmake_minimum_required(VERSION 3.16)
project(SO2 C)

set(CMAKE_C_STANDARD 99)
add_link_options(-pthread)
include_directories(${CURSES_INCLUDE_DIR})


add_executable(SO2 main.c client.c client.h board.h board.c server.c server.h)
target_link_libraries(SO2 ${CURSES_LIBRARIES})
target_link_libraries(SO2 ncurses)
target_link_libraries(SO2 rt)

add_executable(SO2Client client.c client.h clientMain.c board.h board.c server.c server.h)
target_link_libraries(SO2Client ${CURSES_LIBRARIES})
target_link_libraries(SO2Client ncurses)
target_link_libraries(SO2Client rt)