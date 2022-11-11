include_directories(..)

set(SOURCES CardsDriver.cpp GameEngineDriver.cpp MapDriver.cpp OrdersDriver.cpp PlayerDriver.cpp CommandProcessingDriver.cpp)
set(HEADERS headers/CardsDriver.h headers/GameEngineDriver.h headers/MapDriver.h headers/OrdersDriver.h headers/PlayerDriver.h headers/CommandProcessingDriver.h LoggingObserverDriver.cpp headers/LoggingObserverDriver.h)

add_executable(MainDriver MainDriver.cpp ${SOURCES} ${HEADERS})
target_link_libraries(MainDriver SourceLib)