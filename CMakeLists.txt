cmake_minimum_required(VERSION 3.29)
project(p4)

set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR "C:/tools/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(SFML_STATIC_LIBRARIES_TRUE)

add_executable(p4
        minesweeper.cpp
        minesweeper.h
        Toolbox.cpp
        Button.cpp
        Tile.cpp
        GameState.cpp
        )

target_link_libraries(p4 sfml-graphics sfml-window sfml-system)


