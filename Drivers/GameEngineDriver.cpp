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

    Continent* continent = new Continent();

    Player* player1 = new Player("Rickky Bobby");
    Player* player2 = new Player("Buddy TheElf");
    Player* player3 = new Player("Ron Burgundy");
    gameEngine->addPlayer(player1);
    gameEngine->addPlayer(player2);
    gameEngine->addPlayer(player3);

    Territory* t1 = new Territory(0, "territory1", player1);
    Territory* t2 = new Territory(1, "territory2", player2);
    Territory* t3 = new Territory(2, "territory3", player3);
    Territory* t4 = new Territory(3, "territory4", player1);
    Territory* t5 = new Territory(4, "territory5", player3);

    player1->addTerritory(t1);
    player1->addTerritory(t4);
    player2->addTerritory(t2);
    player3->addTerritory(t3);
    player3->addTerritory(t5);

    t1->addAdjacentTerritory(t2);
    t1->addAdjacentTerritory(t4);
    t1->addAdjacentTerritory(t5);

    t2->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(t4);
    t2->addAdjacentTerritory(t3);

    t3->addAdjacentTerritory(t2);
    t3->addAdjacentTerritory(t4);
    t3->addAdjacentTerritory(t5);

    t5->addAdjacentTerritory(t1);
    t5->addAdjacentTerritory(t4);
    t5->addAdjacentTerritory(t3);

    t4->addAdjacentTerritory(t1);
    t4->addAdjacentTerritory(t2);
    t4->addAdjacentTerritory(t3);
    t4->addAdjacentTerritory(t5);

    continent->addTerritory(t1);
    continent->addTerritory(t2);
    continent->addTerritory(t3);
    continent->addTerritory(t4);
    continent->addTerritory(t5);
    vector<Territory*> territories = vector<Territory*>();
    territories.push_back(t1);
    territories.push_back(t2);
    territories.push_back(t3);
    territories.push_back(t4);
    territories.push_back(t5);

    vector<Continent*> cont = vector<Continent*>();
    cont.push_back(continent);

    Map* map = new Map(territories, cont);

    gameEngine->setGameMap(map);
    // End of test data

    gameEngine->gameStart();

    Orders* ordefrs = new Deploy(player1,1, t1);
    player1->getOrdersList()->addOrder(ordefrs);

    gameEngine->mainGameLoop();






//    LogObserver* logObserver= new LogObserver();
//    GameEngine* gameEngine = new GameEngine(logObserver);
//    //AssignReinforcement* assignReinforcement = new AssignReinforcement(gameEngine);
//
//    //test if all other function work in mainGameLoop
//    //gameEngine->mainGameLoop();
//
//    //create/ hardcoding the players
//    Player* player1 = new Player("Rickky Bobby");
//    Player* player2 = new Player("Buddy TheElf");
//    Player* player3 = new Player("Ron Burgundy");
//
//    //push all the player in the assignreinforcement
//    //assignReinforcement->players.push_back(player1);
//    //assignReinforcement->players.push_back(player2);
//    //assignReinforcement->players.push_back(player3);
//
//    //push all the player in the gameengine players vector list of the startup phase
//    gameEngine->getGamePlayers().push_back(player1);
//    gameEngine->getGamePlayers().push_back(player2);
//    gameEngine->getGamePlayers().push_back(player3);
//
//    //push all the player in the gameengine players vector list of the maingameloop
//    //gameEngine->players.push_back(player1);
//    //gameEngine->players.push_back(player2);
//    //gameEngine->players.push_back(player3);
//
//    //hardcode a map
//    //create a map and distribute them
//    //Map* map = new Map(gameEngine->territories, gameEngine->gameMap->getContinents());
//    //Map* map = new Map();
//
//    //map->getTerritories();
//
//    //gameEngine->gameMap->getContinents();
///*
//    //testing hardcode map
//    auto* mapLoader = new MapLoader();
//    vector<Map*> maps;
//    vector<string> map_file_names;
//    auto path = std::filesystem::path("./Map Files");
//
//    //Here, I am looping through all the map files in the Map Files folder and loading them one by one
//    for (auto& map_file: std::filesystem::directory_iterator(path)) {
//        if (map_file.path().string().substr(12) == ".DS_Store") {
//            continue;
//        }
//        cout << "\tMap file: " << map_file.path().string().substr(12) << endl;
//
//        map_file_names.push_back(map_file.path().string().substr(12));
//
//        maps.push_back(mapLoader->loadMap(map_file.path().string()));
//    }
//*/
//    cout<<endl;
//    // I make sure to delete the MapLoader object here before the program/process is finished, which deletes the Map objects that were loaded as well
//    //delete mapLoader;
//
//    Continent* continent;
//    vector<Continent*> continentList;
//
//    //vector<Territory*> territorylist;
//
//    //testing hard coding the map
//    Territory* territory1 = new Territory(0,"territory1",player1);
//    Territory* territory1_1 = new Territory(1,"territory1",player1);
//    Territory* territory2 = new Territory(2,"territory2",player2);
//    Territory* territory3 = new Territory(3,"territory3",player3);
//
//    /*try to create continents and add territory in them to use with map(continent, territroy) method
//    for(int i = 0; i < continentList.size(); i++) {
//
//        territory1->setMapTerritoryId(i);
//
//        continent->addTerritory(territory1);
//        continentList.push_back(continent);
//
//        continent->addTerritory(territory2);
//        continentList.push_back(continent);
//
//        continent->addTerritory(territory3);
//        continentList.push_back(continent);
//    }
//
//    //Territory* territory3_2 = new Territory(0,"t3",player3);
//    //Territory* territory3_3 = new Territory(0,"t3",player3);
//    territorylist.push_back(territory1);
//    territorylist.push_back(territory2);
//    territorylist.push_back(territory3);
//    territorylist.push_back(territory1_1);
//    */
//
//    player1->addTerritory(territory1);
//    player2->addTerritory(territory3);
//    player3->addTerritory(territory2);
//
//    territory1->addAdjacentTerritory(territory1_1);
//    territory1_1->addAdjacentTerritory(territory1);
//
//    territory1->addAdjacentTerritory(territory3);
//    territory3->addAdjacentTerritory(territory1);
//
//    territory3->setNumberOfArmies(5);
//    territory1->setNumberOfArmies(3);
//    territory1_1->setNumberOfArmies(2);
//
//    //gameEngine->gameMap->getTerritories();
//
//    //go through the gamestart phase
//    gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::AddPlayer);
//
//    //leave gamestart phase to go into maingameloop
//    //gameEngine->changeStateByTransition(GameEngine::GameStart); //same as gameEngine->mainGameLoop();
//
//    //amount of player suppose to return 50 army regardless of territory own
//    cout<<player1->getPlayerName()<<" has: " <<player1->getReinforcementPool()<<" has army" << endl;
//    cout<<player2->getPlayerName()<<" has: " <<player2->getReinforcementPool()<<" has army" << endl;
//    cout<<player3->getPlayerName()<<" has: " <<player3->getReinforcementPool()<<" has army" << endl;
//    cout<< endl;
//
//    //enter the assignreinforcementphase
//    gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::GameStart); //same as //gameEngine->gameStart();
//
//    //suppose to return army per player based on the territory they own
//    cout<<"After the territory has been distributed:"<<endl;
//    cout<<player1->getPlayerName()<<" has: " <<player1->getReinforcementPool()<<" has army" << endl;
//    cout<<player2->getPlayerName()<<" has: " <<player2->getReinforcementPool()<<" has army" << endl;
//    cout<<player3->getPlayerName()<<" has: " <<player3->getReinforcementPool()<<" has army" << endl;
//    cout<< endl;
//
//    //a player will only issue deploy orders and no other kind of orders if they still have army units in their reinforcement pool
//    cout<<"Player issue order deploy:"<<endl;
//    cout<<player1->getPlayerName()<<" has: " <<player1->getReinforcementPool()<<" has army, it can start to execute other order"<< endl;
//    cout<<player2->getPlayerName()<<" has: " <<player2->getReinforcementPool()<<" has army, it can start to execute other order"<< endl;
//    cout<<player3->getPlayerName()<<" has: " <<player3->getReinforcementPool()<<" has army, it can start to execute other order"<< endl;
//    cout<< endl;
//
//    //a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists
//
//    //a player can play cards to issue orders
//
//    //enter the issueorderphase
//    //gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::IssueOrder);
//
//    //enter the executeorderphase
//    //gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::IssueOrdersEnd);
//
//    //add test cases in a round robin fashion by part 2
//    //AssignReinforcement;
//
//    //executeOrdersPhase();
//
//    //issueOrdersPhase();
//
//    //delete mapLoader;
//    delete gameEngine;
}
