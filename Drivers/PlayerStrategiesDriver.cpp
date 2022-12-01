#include "sources/Player//PlayerStrategy.h"
#include "headers/PlayerStrategiesDriver.h"
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/**
 * test all the playerstrategy classes
 */
void testPlayerStrategies(){

    LogObserver* logObserver = new LogObserver();

    GameEngine* gameEngine = new GameEngine(logObserver);

    vector<Player*> players;
    vector<Continent*> continents;
    vector<Territory*> territories;

    Deck* deck = nullptr;

    Map* map = new Map(territories, continents);
    //auto* mapLoader = new MapLoader();
    //Map* map = mapLoader->loadMap("./Map Files/Aden.map");

    //(1) different players can be assigned different strategies that lead to different behavior
    //using the Strategy design pattern;
    Player* player1 = new Player("Rickky Bobby");

    Player* player2 = new Player("Buddy TheElf");

    Player* player3 = new Player("Kevin Hart");

    PlayerStrategy* aggressivePlayer = new AggressivePlayerStrategy(player1);

    PlayerStrategy* neutralPlayer = new NeutralPlayerStrategy(player2);

    PlayerStrategy* cheaterPlayer = new CheaterPlayerStrategy(player3);


    Territory* continent1_t1 = new Territory(1, "continent1_t1", player1);
    Territory* continent1_t2 = new Territory(2, "continent1_t2", player2);
    Territory* continent1_t3 = new Territory(3, "continent1_t3", player1);
    Territory* continent1_t4 = new Territory(4, "continent1_t4", player2);
    Territory* continent1_t5 = new Territory(5, "continent1_t5", player3);

    territories.push_back(continent1_t1);
    territories.push_back(continent1_t2);
    territories.push_back(continent1_t3);
    territories.push_back(continent1_t4);
    territories.push_back(continent1_t5);

    continent1_t1->addAdjacentTerritory(continent1_t2);
    continent1_t2->addAdjacentTerritory(continent1_t1);
    continent1_t2->addAdjacentTerritory(continent1_t3);
    continent1_t3->addAdjacentTerritory(continent1_t2);
    continent1_t3->addAdjacentTerritory(continent1_t4);
    continent1_t4->addAdjacentTerritory(continent1_t3);
    continent1_t4->addAdjacentTerritory(continent1_t5);
    continent1_t5->addAdjacentTerritory(continent1_t4)

    continent1_t1->setTerritoryOwner(player1);
    continent1_t2->setTerritoryOwner(player2);
    continent1_t3->setTerritoryOwner(player2);
    continent1_t4->setTerritoryOwner(player2);

    player1->setReinforcementPool(15);
    player1->addTerritory(continent1_t1);

    cout<<player1->getPlayerName()<< " turn to attack."<< endl;
    cout<<endl;

    player1->setPlayerStrategy(aggressivePlayer);
    player2->setPlayerStrategy(neutralPlayer);

    gameEngine->addPlayer(player1);
    gameEngine->addPlayer(player2);
    gameEngine->setGameMap(map);
    //gameEngine->setDemo(true);
    //gameEngine->getPlayer;
    //gameEngine->gameMap;
    //gameEngine->deck;
    player1->issueOrder(gameEngine);


    //(2) the strategy adopted by a player can be changed dynamically during play
    //how???

    //return the owner and the amount of army per territory
    cout<<player1->getPlayerName()<< " has the strategy pattern: "<< player1->getPlayerStrategy() <<endl;

    //execute the order of the player
    cout<<player1->getPlayerName()<< " execute orders."<< endl;

    for(Orders* orders: player1->getOrdersList()->getOrdersList()){
        orders->execute();
        cout<< "Order executed"<< endl;
    }

    delete gameEngine;
}