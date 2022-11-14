#include <iostream>
#include <sstream>
#include <filesystem>
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Map/Map.h"
#include "sources/Player/Player.h"
#include <algorithm>
#include <random>

void testGameStates() {
    LogObserver* obs = new LogObserver;
    GameEngine* gameEngine = new GameEngine(obs);

    // Show the transitions for demo by going to the start game first
    gameEngine->changeStateByTransition(GameEngine::StartGame);
    delete gameEngine;
}

void testStartupPhase(int argc, char** argv) {
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
            cout << "Displaying the outcome of gameStart being called." << endl;
            for (auto p: gameEngine->getGamePlayers()) {
                cout << *p << endl;
            }
            cout << "Simulating a win transition for demo purpose." << endl;
            gameEngine->changeStateByTransition(GameEngine::Win);
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

/**
 * You must deliver a driver as a free function named testMainGameLoop() that demonstrates that:
 * (1) a player receives the correct number of army units in the reinforcement phase (showing different cases);
 * (2) a player will only issue deploy orders and no other kind of orders if they still have army units in their reinforcement pool;
 * (3) a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists;
 * (4) a player can play cards to issue orders;
 * (5) a player that does not control any territory is removed from the game;
 * (6) the game ends when a single player controls all the territories. All of this must be implemented in a single
 * .cpp/.h file duo named GameEngine.cpp/GameEngine.h, except the issueOrder() method, which is a member 5 of the Player class,
 * implemented in the Player.h/Player.h file duo. This driver function must be in the GameEngineDriver.cpp file.
 */
//free function for the main game loop
void testMainGameLoop() {
    //Setup test data
    LogObserver* logObserver = new LogObserver();
    GameEngine* gameEngine = new GameEngine(logObserver);


    Player* player1 = new Player("Rickky Bobby");
    Player* player2 = new Player("Buddy TheElf");
    Player* player3 = new Player("Ron Burgundy");
    gameEngine->addPlayer(player1);
    gameEngine->addPlayer(player2);
    gameEngine->addPlayer(player3);

//    Territory* t1 = new Territory(0, "territory1", player1);
//    Territory* t2 = new Territory(1, "territory2", player2);
//    Territory* t3 = new Territory(2, "territory3", player3);
//    Territory* t4 = new Territory(3, "territory4", player1);
//    Territory* t5 = new Territory(4, "territory5", player3);
//
//    player1->addTerritory(t1);
//    player1->addTerritory(t4);
//    player2->addTerritory(t2);
//    player3->addTerritory(t3);
//    player3->addTerritory(t5);
//
//    t1->addAdjacentTerritory(t2);
//    t1->addAdjacentTerritory(t4);
//    t1->addAdjacentTerritory(t5);
//
//    t2->addAdjacentTerritory(t1);
//    t2->addAdjacentTerritory(t4);
//    t2->addAdjacentTerritory(t3);
//
//    t3->addAdjacentTerritory(t2);
//    t3->addAdjacentTerritory(t4);
//    t3->addAdjacentTerritory(t5);
//
//    t5->addAdjacentTerritory(t1);
//    t5->addAdjacentTerritory(t4);
//    t5->addAdjacentTerritory(t3);
//
//    t4->addAdjacentTerritory(t1);
//    t4->addAdjacentTerritory(t2);
//    t4->addAdjacentTerritory(t3);
//    t4->addAdjacentTerritory(t5);
//
//    continent->addTerritory(t1);
//    continent->addTerritory(t2);
//    continent->addTerritory(t3);
//    continent->addTerritory(t4);
//    continent->addTerritory(t5);
//    vector<Territory*> territories = vector<Territory*>();
//    territories.push_back(t1);
//    territories.push_back(t2);
//    territories.push_back(t3);
//    territories.push_back(t4);
//    territories.push_back(t5);

//    vector<Continent*> cont = vector<Continent*>();
//    cont.push_back(continent);
    // End of test data

    auto* mapLoader = new MapLoader();
    Map* map = mapLoader->loadMap("./Map Files/Aden.map");
    gameEngine->setGameMap(map);
    gameEngine->gameStart();

    cout << endl;
    gameEngine->mainGameLoop();

    delete gameEngine;
}
