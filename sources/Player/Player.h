#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "sources/Orders/Orders.h"

using namespace std;

//Declare the classes used
class Territory;

class OrdersList;

class Hand;

class Map;

class Deck;

class Player {
public:
    //Constructor
    Player();

    //Parameterized Constructor
    explicit Player(const string& name);

    //Copy constructor
    Player(const Player& player);

    //Destructor
    ~Player();

    //Getter for owned territories
    vector<Territory*> getTerritories();

    //Getter for order list
    OrdersList* getOrdersList();

    //Getter for player hand
    Hand* getHandCards();

    //Setter for hand
    void setHandCards(Hand* hand);

    //Getter for player name
    string getPlayerName();

    //Setter for player name
    void setPlayerName(string newName);

    //Territories for the player to defend
    vector<Territory*> toDefend();

    //Territories for the player to attack
    vector<Territory*> toAttack();

    //Territories from which the army come from
    Territory* sourceTerritory(Map* map);

    //Territories to send army to
    Territory* targetTerritory(Map* map);

    //Issues an order
    bool issueOrder(Map* map, Player* neutral, vector<Player*> player, Deck* deck, Hand* hand);

    //Adds a territory to be owned by player
    void addTerritory(Territory* territory);

    //Stream insertion operator overload
    friend ostream& operator<<(ostream& stream, const Player& player);

    //Assignment operator overload
    Player& operator=(const Player& player);

    void setReinforcementPool(int reinforcementPoolUnits);

    //remove the order
    Orders* removeOrder();

    //list of players

    int getReinforcementPool() const;

    void setNegotiationWith(Player *player);

    bool checkIsNegotiation(Player* player);

private:
    //Collection of territories
    vector<Territory*> territories;

    //List of orders
    OrdersList* ordersList;

    //Hand cards
    Hand* handCards;

    //Player name
    string name;

    Player* isNegotiationWith;

    //initial army units
    int reinforcementPoolUnits = 50;

};