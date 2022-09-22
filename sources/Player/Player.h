#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class Territory;
class OrdersList;
class Hand;

class Player {
public:
    Player();
    explicit Player(const string &name);
    Player(const Player &player);
    ~Player();
    vector<Territory*> getTerritories();
    OrdersList* getOrdersList();
    Hand* getHandCards();
    void setHandCards(Hand* hand);
    string getPlayerName();
    void setPlayerName(string newName);
    vector<Territory*> toDefend();
    vector<Territory*> toAttack();
    bool issueOrder();
    void addTerritory(Territory* territory);
    friend ostream& operator<<(ostream& stream, const Player &player);
private:
    vector<Territory*> territories;
    OrdersList* ordersList;
    Hand* handCards;
    string name;
};