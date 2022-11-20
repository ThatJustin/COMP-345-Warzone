include_directories(..)

set(SOURCES Player/Player.cpp Map/Map.cpp Orders/Orders.cpp Cards/Cards.cpp GameEngine/GameEngine.cpp GameEngine/CommandProcessor.cpp Player/PlayerStrategy.cpp)
set(HEADERS Player/Player.h Map/Map.h Orders/Orders.h Cards/Cards.h GameEngine/GameEngine.h GameEngine/CommandProcessor.h LogObserver/LoggingObserver.cpp LogObserver/LoggingObserver.h Player/PlayerStrategy.h)
add_library(SourceLib STATIC ${SOURCES} ${HEADERS})
