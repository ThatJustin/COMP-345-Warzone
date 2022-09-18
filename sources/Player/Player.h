#pragma once
#include <vector>
#include <string>


using namespace std;
class Territory;
class OrdersList;
class Hand;

class Player {
public:
    Player();
    explicit Player(const string &name);
    ~Player();
    vector<Territory*> getTerritories();
    OrdersList* getOrdersList();
    Hand* getHandCards();
    string getPlayerName();

private:
    vector<Territory*> territories;
    OrdersList* ordersList;
    Hand* handCards;
    vector<Territory*> toDefend();
    vector<Territory*> toAttack();
    bool issueOrder();
    string name;
};