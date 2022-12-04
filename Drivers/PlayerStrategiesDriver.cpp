//#include "sources/Player//PlayerStrategy.h"
//#include "headers/PlayerStrategiesDriver.h"
//#include "sources/GameEngine/CommandProcessor.h"
//#include "sources/GameEngine/GameEngine.h"
//#include "sources/Player/Player.h"
//#include "sources/Map/Map.h"
//#include <iostream>
//#include <algorithm>
//#include <vector>
//using namespace std;

///**
// * test all the playerstrategy classes
// */
//void testPlayerStrategies(){
//
//    vector<Territory*> territories;
//
//    //(1) different players can be assigned different strategies that lead to different behavior
//    //using the Strategy design pattern;
//    Player* player1 = new Player("Rickky Bobby");
//
//    Player* player2 = new Player("Buddy TheElf");
//
//    Player* player3 = new Player("Kevin Hart");
//
//    Player* player4 = new Player("The Rock");
//
//    PlayerStrategy* aggressivePlayer = new AggressivePlayerStrategy(player1);
//
//    PlayerStrategy* neutralPlayer = new NeutralPlayerStrategy(player2);
//
//    PlayerStrategy* cheaterPlayer = new CheaterPlayerStrategy(player3);
//
//    PlayerStrategy* benevolentPlayer = new BenevolentPlayerStrategy(player4);
//
//    player1->setPlayerStrategy(aggressivePlayer);
//    player2->setPlayerStrategy(neutralPlayer);
//    player3->setPlayerStrategy(cheaterPlayer);
//    player4->setPlayerStrategy(benevolentPlayer);
//
//    Deck* deck = nullptr;
//
//    player1->setReinforcementPool(15);
//    player3->setReinforcementPool(15);
//
//    //vector<Player*> players;
//    vector<Continent*> continents;
//
//    Territory* continent1_t1 = new Territory(1, "continent1_t1", player1);
//    Territory* continent1_t2 = new Territory(2, "continent1_t2", player2);
//    Territory* continent1_t3 = new Territory(3, "continent1_t3", player1);
//    Territory* continent1_t4 = new Territory(4, "continent1_t4", player2);
//    Territory* continent1_t5 = new Territory(5, "continent1_t5", player3);
//    Territory* continent1_t6 = new Territory(6, "continent1_t6", player4);
//    Territory* continent1_t7 = new Territory(7, "continent1_t7", player3);
//
//    territories.push_back(continent1_t1);
//    territories.push_back(continent1_t2);
//    territories.push_back(continent1_t3);
//    territories.push_back(continent1_t4);
//    territories.push_back(continent1_t5);
//    territories.push_back(continent1_t6);
//    territories.push_back(continent1_t7);
//
//    continent1_t1->addAdjacentTerritory(continent1_t2);
//    continent1_t2->addAdjacentTerritory(continent1_t1);
//    continent1_t2->addAdjacentTerritory(continent1_t3);
//    continent1_t3->addAdjacentTerritory(continent1_t2);
//    continent1_t3->addAdjacentTerritory(continent1_t4);
//    continent1_t4->addAdjacentTerritory(continent1_t3);
//    continent1_t4->addAdjacentTerritory(continent1_t5);
//    continent1_t5->addAdjacentTerritory(continent1_t4);
//    continent1_t5->addAdjacentTerritory(continent1_t6);
//    continent1_t6->addAdjacentTerritory(continent1_t5);
//    continent1_t7->addAdjacentTerritory(continent1_t5);
//    continent1_t5->addAdjacentTerritory(continent1_t7);
//
//    continent1_t1->setNumberOfArmies(14);
//    continent1_t2->setNumberOfArmies(15);
//    continent1_t3->setNumberOfArmies(4);
//    continent1_t4->setNumberOfArmies(15);
//    continent1_t5->setNumberOfArmies(15);
//    continent1_t6->setNumberOfArmies(15);
//    continent1_t7->setNumberOfArmies(15);
//
//    //continent1_t1->setTerritoryOwner(player1);
//    //continent1_t2->setTerritoryOwner(player2);
//    //continent1_t3->setTerritoryOwner(player2);
//    //continent1_t4->setTerritoryOwner(player2);
//    //continent1_t5->setTerritoryOwner(player3);
//    //continent1_t6->setTerritoryOwner(player4);
//
//    player1->addTerritory(continent1_t1);
//    player1->addTerritory(continent1_t3);
//    player2->addTerritory(continent1_t2);
//    player2->addTerritory(continent1_t4);
//    player3->addTerritory(continent1_t5);
//    player4->addTerritory(continent1_t6);
//    player3->addTerritory(continent1_t7);
//
//    cout<<player1->getPlayerName()<< " turn to attack, he is an aggressive player."<< endl;
//
//    //Map* map = new Map(territories, continents);
//    auto* mapLoader = new MapLoader();
//    Map* map = mapLoader->loadMap("./Map Files/Aden.map");
//
//    LogObserver* logObserver = new LogObserver();
//
//    GameEngine* gameEngine = new GameEngine(logObserver);
//
//    gameEngine->addPlayer(player1);
//    gameEngine->addPlayer(player2);
//    gameEngine->addPlayer(player3);
//    gameEngine->addPlayer(player4);
//    gameEngine->setGameMap(map);
//    gameEngine->getDeck();
//    //player1->issueOrder(gameEngine);
//    //player2->issueOrder(gameEngine);
//    //player3->issueOrder(gameEngine);
//
//    //(2) the strategy adopted by a player can be changed dynamically during play
//    //return the owner and the amount of army per territory
////    cout<<player1->getPlayerName()<< " has "<< player1->getReinforcementPool() << " army." <<endl;
////    cout<<player1->getPlayerName()<< " has "<< player1->getTerritories().size()<< " territory." <<endl;
////    cout<<endl;
////
////    cout<<player1->getPlayerName() << " issue order is: " << player1->issueOrder(gameEngine) << endl;
////    cout<<endl;
////
////    cout<<player2->getPlayerName() << " issue order is: " << player2->issueOrder(gameEngine) << endl;
////    cout<<endl;
//
//    cout<<player3->getPlayerName()<< " has "<< player3->getReinforcementPool() << " army." <<endl;
//    cout<<player3->getPlayerName()<< " has "<< player3->getTerritories().size()<< " territory." <<endl;
//    cout<<endl;
//
//    cout<<player3->getPlayerName() << " issue order is: " << player3->issueOrder(gameEngine) << endl;
//    cout<<endl;
//
//    // cout<<player3->getPlayerName()<< " has "<< player3->getTerritories().size()<< " territory." <<endl;
//    cout<<endl;
//
//    //cout<<player4->getPlayerName() << " issue order is: " << player4->issueOrder(gameEngine) << endl;
//    //cout<<endl;
//
//=======
#include "Drivers/headers/PlayerStrategiesDriver.h"
#include "sources/LogObserver/LoggingObserver.h"
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;

void testPlayerStrategies(int argc, char** argv) {
    if (argc < 2 || (argv[1] != "-console"sv && argv[1] != "-file"sv)) {
        cout << "Please run the program using either -console or -file filename arguments." << endl;
        exit(0);
    }
    bool isUsingConsole = true;
    string mapFileName;
    if (argv[1] == "-file"sv) {
        if (argc < 3) {
            cout << "Please include a file name for this argument like -file filename." << endl;
            exit(0);
        }
        mapFileName = argv[2];
        isUsingConsole = false;
    }
    LogObserver* logObserver = new LogObserver();
    CommandProcessor* commandProcessor;
    if (isUsingConsole) {
        commandProcessor = new CommandProcessor(isUsingConsole, mapFileName, logObserver);
    } else {
        commandProcessor = new FileCommandProcessorAdapter(mapFileName, logObserver);
    }

    GameEngine* gameEngine = new GameEngine(logObserver);
    gameEngine->setCommandProcessor(commandProcessor);
    gameEngine->changeStateByTransition(GameEngine::StartGame);

    while (true) {
        string curStateName = gameEngine->getCurrentGameState()->name;
        Command* c = commandProcessor->getCommand(curStateName);
        if (c == nullptr) {
            cout << "This command is not valid for this state." << endl;
            continue;
        }
        gameEngine->commandTransitionName = c->getTransitionName();
        if (c->getTransitionName() == "loadmap") {
            gameEngine->commandParam = c->getParam();
            gameEngine->changeStateByTransition(GameEngine::LoadMap);
        } else if (c->getTransitionName() == "validatemap") {
            gameEngine->changeStateByTransition(GameEngine::ValidateMap);
        } else if (c->getTransitionName() == "addplayer") {
            gameEngine->commandParam = c->getParam();
            gameEngine->changeStateByTransition(GameEngine::AddPlayer);
        } else if (c->getTransitionName() == "gamestart") {
            gameEngine->gameStart();
        } else if (c->getTransitionName() == "replay") {
            gameEngine->prepareForReplay();
            gameEngine->changeStateByTransition(GameEngine::Play);
        } else if (c->getTransitionName() == "quit") {
            cout << "Thanks for playing!" << endl;
            break;
        }
    }
    delete gameEngine;
}