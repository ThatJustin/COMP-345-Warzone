#pragma once
#include <vector>
#include <memory>
#include <string>
//TODO include Territory.Orders,Cards when they are implemented

//For now this is here so there's no errors, remove when official implementations are added
class Territory {};
class OrderList {};
class Hand {};

using namespace std;

class Player {
public:
    Player();
    explicit Player(const string &name);
    ~Player();
    string name;
    vector<Territory*> getTerritories();
    OrderList* getOrderList();
    Hand* getHandCards();

private:
    vector<Territory*> territories;
    OrderList* orderList;
    Hand* handCards;
    vector<Territory*> toDefend();
    vector<Territory*> toAttack();
    bool issueOrder();
};