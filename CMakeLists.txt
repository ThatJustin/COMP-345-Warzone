cmake_minimum_required(VERSION 3.23)
project(COMP_345_Warzone)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(Drivers)
add_subdirectory(sources)