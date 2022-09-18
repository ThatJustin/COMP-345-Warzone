include_directories(..)

add_executable(MapDriver MapDriver.cpp)
target_link_libraries(MapDriver SourceLib)

add_executable(PlayerDriver PlayerDriver.cpp)
target_link_libraries(PlayerDriver SourceLib)

add_executable(OrdersDriver OrdersDriver.cpp)
target_link_libraries(OrdersDriver SourceLib)

add_executable(CardsDriver CardsDriver.cpp)
target_link_libraries(CardsDriver SourceLib)

add_executable(GameEngineDriver GameEngineDriver.cpp)
target_link_libraries(GameEngineDriver SourceLib)