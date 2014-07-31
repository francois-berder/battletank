cmake_minimum_required(VERSION 2.6)

#Configuration du projet
project(spritesheetmaker)

set(CMAKE_CXX_FLAGS "-std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Weffc++ -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -ggdb3")

#Configuration de l'exécutable
file(
GLOB_RECURSE
source_files
src/*
)

include_directories(src/)

add_executable(
spritesheetmaker
${source_files}
)

target_link_libraries(
spritesheetmaker
sfml-system
sfml-graphics
boost_system
boost_filesystem
)


