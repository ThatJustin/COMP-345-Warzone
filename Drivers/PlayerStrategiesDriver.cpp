#include "Drivers/headers/PlayerStrategiesDriver.h"
#include "sources/LogObserver/LoggingObserver.h"
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Player/PlayerStrategy.h"
#include "sources/Map/Map.h"
#include <string>
#include <iostream>

using std::cout;
using std::endl;

void testPlayerStrategies(int argc, char** argv) {

    //BenevolentPlayerStrategy
    cout << "Testing HumanPlayerStrategy " << endl;
    auto* logObserver = new LogObserver();
    auto* commandProcessor = new CommandProcessor(false, "", logObserver);

    auto* gameEngine = new GameEngine(logObserver);
    gameEngine->setCommandProcessor(commandProcessor);
    gameEngine->changeStateByTransition(GameEngine::StartGame);
    auto* mapLoader = new MapLoader();
    auto* map = mapLoader->loadMap("./Map Files/Aden.map");
    Player* human = new Player("Benevolent");
    human->setPlayerStrategy(new HumanPlayerStrategy(human));
    gameEngine->addPlayer(human);
    gameEngine->addPlayer(new Player("Johnny"));
    gameEngine->setGameMap(map);
    gameEngine->setDemo(true);
    gameEngine->gameStart();

    human->issueOrder(gameEngine);
    human->useOrders();
    gameEngine->changeStateByTransition(GameEngine::GameStart); // assign reinforcement

    human->issueOrder(gameEngine);
    human->useOrders();
    //End benevolent

    // AggressivePlayerStrategy
//    LogObserver* logObserver = new LogObserver();
//    CommandProcessor* commandProcessor = new CommandProcessor(false, "", logObserver);
//    cout << "Testing AggressivePlayerStrategy " << endl;
//
//    GameEngine* gameEngine = new GameEngine(logObserver);
//    gameEngine->setCommandProcessor(commandProcessor);
//    gameEngine->changeStateByTransition(GameEngine::StartGame);
//    auto* mapLoader = new MapLoader();
//    Map* map = mapLoader->loadMap("./Map Files/Aden.map");
//    Player* agg = new Player("Aggressive");
//    agg->setPlayerStrategy(new AggressivePlayerStrategy(agg));
//    gameEngine->addPlayer(agg);
//    gameEngine->addPlayer(new Player("Johnny"));
//    gameEngine->setGameMap(map);
//    gameEngine->setDemo(true);
//    gameEngine->gameStart();
//
//    agg->issueOrder(gameEngine);
//    agg->useOrders();
//    gameEngine->changeStateByTransition(GameEngine::GameStart); // assign reinforcement
//
//    agg->issueOrder(gameEngine);
//    agg->useOrders();
//
//    delete gameEngine;
//    //End of Aggressive

//    //BenevolentPlayerStrategy
//    cout << "Testing BenevolentPlayerStrategy " << endl;
//    logObserver = new LogObserver();
//    commandProcessor = new CommandProcessor(false, "", logObserver);
//
//    gameEngine = new GameEngine(logObserver);
//    gameEngine->setCommandProcessor(commandProcessor);
//    gameEngine->changeStateByTransition(GameEngine::StartGame);
//    mapLoader = new MapLoader();
//    map = mapLoader->loadMap("./Map Files/Aden.map");
//    Player* ben = new Player("Benevolent");
//    ben->setPlayerStrategy(new BenevolentPlayerStrategy(ben));
//    gameEngine->addPlayer(ben);
//    gameEngine->addPlayer(new Player("Johnny"));
//    gameEngine->setGameMap(map);
//    gameEngine->setDemo(true);
//    gameEngine->gameStart();
//
//    ben->issueOrder(gameEngine);
//    ben->useOrders();
//    gameEngine->changeStateByTransition(GameEngine::GameStart); // assign reinforcement
//
//    ben->issueOrder(gameEngine);
//    ben->useOrders();
    //End benevolent


    //NeutralPlayerStrategy
//    cout << "Testing NeutralPlayerStrategy " << endl;
//    auto* logObserver = new LogObserver();
//    auto* commandProcessor = new CommandProcessor(false, "", logObserver);
//
//    auto* gameEngine = new GameEngine(logObserver);
//    gameEngine->setCommandProcessor(commandProcessor);
//    gameEngine->changeStateByTransition(GameEngine::StartGame);
//    auto* mapLoader = new MapLoader();
//    auto* map = mapLoader->loadMap("./Map Files/Aden.map");
//    Player* neutral = new Player("Neutral");
//    auto* johnny = new Player("Johnny");
//    neutral->setPlayerStrategy(new NeutralPlayerStrategy(neutral));
//    gameEngine->addPlayer(neutral);
//    gameEngine->addPlayer(johnny);
//    gameEngine->setGameMap(map);
//    gameEngine->setDemo(true);
//    gameEngine->gameStart();
//
//    neutral->issueOrder(gameEngine);
//    johnny->issueOrder(gameEngine);
//    neutral->issueOrder(gameEngine);
//    neutral->useOrders();
//    johnny->useOrders();
//    gameEngine->changeStateByTransition(GameEngine::GameStart); // assign reinforcement
//
//    neutral->issueOrder(gameEngine);
//    johnny->issueOrder(gameEngine);
//    neutral->useOrders();
//    johnny->useOrders();

    //CheaterPlayerStrategy
//    cout << "Testing CheaterPlayerStrategy " << endl;
//    auto* logObserver = new LogObserver();
//    auto* commandProcessor = new CommandProcessor(false, "", logObserver);
//
//    auto* gameEngine = new GameEngine(logObserver);
//    gameEngine->setCommandProcessor(commandProcessor);
//    gameEngine->changeStateByTransition(GameEngine::StartGame);
//    auto* mapLoader = new MapLoader();
//    auto* map = mapLoader->loadMap("./Map Files/Aden.map");
//    Player* cheat = new Player("Cheater");
//    cheat->setPlayerStrategy(new CheaterPlayerStrategy(cheat));
//    gameEngine->addPlayer(cheat);
//    gameEngine->addPlayer(new Player("Johnny"));
//    gameEngine->setGameMap(map);
//    gameEngine->setDemo(true);
//    gameEngine->gameStart();
//
//    cheat->issueOrder(gameEngine);
//    cheat->useOrders();
//    gameEngine->changeStateByTransition(GameEngine::GameStart); // assign reinforcement
//
//    cheat->issueOrder(gameEngine);
//    cheat->useOrders();
    //end cheat

}