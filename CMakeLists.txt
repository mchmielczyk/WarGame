cmake_minimum_required(VERSION 3.10)

set(This WarGame)

project(${This} C CXX)

set(CMAKE_CXX_STANDARD 11)

set(Headers player.h)

set(Sources main.cpp player.cpp)

add_library(${This}_lib STATIC ${Sources} ${Headers})

target_compile_options(${This}_lib PRIVATE -DARGS="-mapa mapa.txt -status status.txt -rozkazy rozkazy.txt")

add_executable(${This} ${Sources})

target_link_libraries(${This} PRIVATE ${This}_lib)