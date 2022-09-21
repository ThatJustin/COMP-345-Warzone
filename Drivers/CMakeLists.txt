include_directories(..)

add_executable(MainDriver MainDriver.cpp)
target_link_libraries(MainDriver SourceLib)