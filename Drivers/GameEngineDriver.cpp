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

    vector<Territory*> ter = vector<Territory*>();
    Player* player1 = new Player("Rickky Bobby");
    Player* player2 = new Player("Buddy TheElf");
    Player* player3 = new Player("Santana");
    Player* player4 = new Player("James Bond");

    vector<Continent*> cont = vector<Continent*>();
    Continent* continent1 = new Continent;
    continent1->setContinentControlBonusValue(3);
    Territory* continent1_t1 = new Territory(1, "continent1_t1", player1);
    Territory* continent1_t2 = new Territory(2, "continent1_t2", player2);
    Territory* continent1_t3 = new Territory(3, "continent1_t3", player1);
    Territory* continent1_t4 = new Territory(4, "continent1_t4", player2);

    ter.push_back(continent1_t1);
    ter.push_back(continent1_t2);
    ter.push_back(continent1_t3);
    ter.push_back(continent1_t4);
    continent1_t1->addAdjacentTerritory(continent1_t2);
    continent1_t1->addAdjacentTerritory(continent1_t3);

    continent1_t2->addAdjacentTerritory(continent1_t1);
    continent1_t2->addAdjacentTerritory(continent1_t4);

    continent1_t3->addAdjacentTerritory(continent1_t1);
    continent1_t3->addAdjacentTerritory(continent1_t4);

    continent1_t4->addAdjacentTerritory(continent1_t2);
    continent1_t4->addAdjacentTerritory(continent1_t3);

    player1->addTerritory(continent1_t1);
    player1->addTerritory(continent1_t3);

    player2->addTerritory(continent1_t2);
    player2->addTerritory(continent1_t4);
    cont.push_back(continent1);
    // End of continent 1

    //    Continent 2 is owned all by player 2 to show continent bonus
    Continent* continent2 = new Continent;
    continent2->setContinentControlBonusValue(5);
    Territory* continent2_t1 = new Territory(5, "continent2_t1", player2);
    Territory* continent2_t2 = new Territory(6, "continent2_t2", player2);
    Territory* continent2_t3 = new Territory(7, "continent2_t3", player2);

    continent2_t1->addAdjacentTerritory(continent2_t2);
    continent2_t1->addAdjacentTerritory(continent2_t3);

    continent2_t2->addAdjacentTerritory(continent2_t1);
    continent2_t2->addAdjacentTerritory(continent2_t3);

    continent2_t3->addAdjacentTerritory(continent2_t1);
    continent2_t3->addAdjacentTerritory(continent2_t2);

    player2->addTerritory(continent2_t1);
    player2->addTerritory(continent2_t2);
    player2->addTerritory(continent2_t3);
    cont.push_back(continent2);
    // End of continent 2

    auto* mapLoader = new MapLoader();
    Map* map = mapLoader->loadMap("./Map Files/Aden.map");
    LogObserver* logObserver = new LogObserver();
    GameEngine* gameEngine = new GameEngine(logObserver);
    gameEngine->addPlayer(player1);
    gameEngine->addPlayer(player2);
    gameEngine->addPlayer(player3);
    gameEngine->addPlayer(player4);
    gameEngine->setGameMap(map);
    gameEngine->setDemo(true);
    gameEngine->gameStart();
//(1) a player receives the correct number of army units in the reinforcement phase (showing different cases);
    cout << "Testing that a player receives the correct number of army units in the reinforcement phase" << endl;
    gameEngine->changeStateByTransition(GameEngine::GameStart);

    // (2) a player will only issue deploy orders and no other kind of orders if they still have army units in their reinforcement pool;
// (3) a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists;
// (4) a player can play cards to issue orders;
    cout << "Testing issuing deploy, advance and card orders." << endl;

    gameEngine->changeStateByTransition(GameEngine::IssueOrder);

// (5) a player that does not control any territory is removed from the game;
// (6) the game ends when a single player controls all the territories.
    cout << "Testing people who lose all territories are removed from the game." << endl;
    cout << "and that if one player owns all territories the game is over." << endl;
    cout << "Emulating only one player with territories remaining." << endl << endl;
    Player* neutral = new Player("NeutralPlayer");
    for (int i = 1; i < gameEngine->getGamePlayers().size(); i++) {
        for (Territory* t: gameEngine->getGamePlayers().at(i)->getTerritories()) {
            cout << "Removing territory " << t->getTerritoryName() << " from player "
                 << t->getTerritoryOwner()->getPlayerName() << endl;
            t->getTerritoryOwner()->removeTerritory(t);
            t->setTerritoryOwner(neutral);
            neutral->addTerritory(t);
        }
    }

    gameEngine->changeStateByTransition(GameEngine::IssueOrdersEnd);
    cout << "End of demo for maingameloop functionality." << endl;
    delete gameEngine;
}
