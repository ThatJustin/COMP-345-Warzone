include_directories(..)

set(SOURCES Player/Player.cpp Map/Map.cpp Orders/Orders.cpp Cards/Cards.cpp)
set(HEADERS Player/Player.h Map/Map.h Orders/Orders.h Cards/Cards.h)
add_library(SourceLib STATIC ${SOURCES} ${HEADERS})
