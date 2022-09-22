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
    string getPlayerName();
    void setPlayerName(string newName);
    vector<Territory*> toDefend();
    vector<Territory*> toAttack();

    friend ostream& operator<<(ostream& stream, const Player &player);
private:
    vector<Territory*> territories;
    OrdersList* ordersList;
    Hand* handCards;

    bool issueOrder();
    string name;
};