include_directories(..)

set(SOURCES CardsDriver.cpp GameEngineDriver.cpp MapDriver.cpp OrdersDriver.cpp PlayerDriver.cpp CommandProcessingDriver.cpp LoggingObserverDriver.cpp PlayerStrategiesDriver.cpp TournamentDriver.cpp)
set(HEADERS headers/CardsDriver.h headers/GameEngineDriver.h headers/MapDriver.h headers/OrdersDriver.h headers/PlayerDriver.h headers/CommandProcessingDriver.h headers/LoggingObserverDriver.h headers/PlayerStrategiesDriver.h headers/TournamentDriver.h)

add_executable(MainDriver MainDriver.cpp ${SOURCES} ${HEADERS})
target_link_libraries(MainDriver SourceLib)