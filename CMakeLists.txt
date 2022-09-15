cmake_minimum_required(VERSION 3.23)
project(COMP_345_Warzone)

set(CMAKE_CXX_STANDARD 17)

add_executable(COMP_345_Warzone main.cpp Player.cpp Player.h Map.h Map.cpp)
add_executable(TestPlayers testPlayers.cpp Player.cpp Player.h)
