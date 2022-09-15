include_directories(..)

set(SOURCES Player/Player.cpp Map/Map.cpp Orders/Orders.cpp Cards/Cards.cpp GameEngine/GameEngine.cpp)
set(HEADERS Player/Player.h Map/Map.h Orders/Orders.h Cards/Cards.h GameEngine/GameEngine.h)
add_library(SourceLib STATIC ${SOURCES} ${HEADERS})
