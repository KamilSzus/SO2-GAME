cmake_minimum_required(VERSION 3.16)
project(SO2 C)

set(CMAKE_C_STANDARD 99)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})


add_executable(SO2 main.c board.h board.c)
target_link_libraries(SO2 ${CURSES_LIBRARIES})