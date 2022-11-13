#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//Declare the classes used
class Territory;

class OrdersList;

class Hand;

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
    bool issueOrder();

    //Adds a territory to be owned by player
    void addTerritory(Territory* territory);

    //Stream insertion operator overload
    friend ostream& operator<<(ostream& stream, const Player& player);

    //Assignment operator overload
    Player& operator=(const Player& player);

    void setReinforcementPool(int reinforcementPoolUnits);

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