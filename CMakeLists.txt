cmake_minimum_required(VERSION 3.10)

set(This WarGame)

project(${This} C CXX)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(googletest)

set(Headers player.h)

set(Sources main.cpp)

add_library(${This}_lib STATIC ${Sources} ${Headers})

target_compile_definitions(${This}_lib PRIVATE ARGS="-mapa mapa.txt -status status.txt -rozkazy rozkazy.txt" )

add_executable(${This} ${Sources})

target_link_libraries(${This} PRIVATE ${This}_lib)

add_subdirectory(test)