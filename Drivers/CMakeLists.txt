include_directories(..)

add_executable(PlayerDriver PlayerDriver.cpp)
target_link_libraries(PlayerDriver SourceLib)

add_executable(MapDriver MapDriver.cpp)
target_link_libraries(MapDriver SourceLib)


