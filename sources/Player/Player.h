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

    //Issues an order
    bool issueOrder(Map* map, vector<Player*> player, Deck* deck); //need to add commandprocessor as a parameter here

    //Adds a territory to be owned by player
    void addTerritory(Territory* territory);

    //Stream insertion operator overload
    friend ostream& operator<<(ostream& stream, const Player& player);

    //Assignment operator overload
    Player& operator=(const Player& player);

    //get the amount of army per player
    int getArmy();

    //set the amount of army per player
    void setArmy(int army);

    //remove the order
    Orders* removeOrder();

    //list of players
    vector<Player*> players;

private:
    //Collection of territories
    vector<Territory*> territories;

    //List of orders
    OrdersList* ordersList;

    //Hand cards
    Hand* handCards;

    //Player name
    string name;

    //amount of soldier for the player
    int army;
};