cmake_minimum_required(VERSION 3.10)

set(This tests)

set(Sources
    test.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC gtest_main ${PROJECT_NAME}_lib)

add_test( NAME ${This} COMMAND ${This})