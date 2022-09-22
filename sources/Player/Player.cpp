#include <iostream>
#include <utility>
#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

Player::Player() {
    this->territories = vector<Territory*>();
    this->name = "";
    this->handCards = new Hand;
    this->ordersList = new OrdersList;
}

Player::Player(const string& name) {
    this->territories = vector<Territory*>();
    this->name = name;
    this->handCards = new Hand;
    this->ordersList = new OrdersList;
}

// Copy Constructor
Player::Player(const Player& player) {
    this->territories = vector<Territory*>(player.territories.size());
    for (auto& temp: player.territories) {
        this->territories.push_back(new Territory(*temp));
    }
    this->handCards = new Hand(*player.handCards);
    this->name = player.name;
    this->ordersList = new OrdersList(*player.ordersList);
}

Player::~Player() {
    for (Territory* terr: territories) {
        delete terr;
        terr = nullptr;
    }
    delete handCards;
    handCards = nullptr;
    delete ordersList;
    ordersList = nullptr;
}

vector<Territory*> Player::getTerritories() {
    return this->territories;
}

OrdersList* Player::getOrdersList() {
    return ordersList;
}

Hand* Player::getHandCards() {
    return handCards;
}

vector<Territory*> Player::toDefend() {
    // For now return an arbitrary list of territories to defend
    vector<Territory*> defendTerritories;
    auto* territory1 = new Territory(1, "Plule", this);
    auto* territory2 = new Territory(2, "Woflington", this);
    auto* territory3 = new Territory(3, "Chuih Cha", this);
    auto* territory4 = new Territory(4, "Soscele", this);
    defendTerritories.push_back(territory1);
    defendTerritories.push_back(territory2);
    defendTerritories.push_back(territory3);
    defendTerritories.push_back(territory4);
    return defendTerritories;
}

vector<Territory*> Player::toAttack() {
    // For now return an arbitrary list of territories to attack
    vector<Territory*> attackTerritories;
    auto* territory1 = new Territory(5, "Voflein", this);
    auto* territory2 = new Territory(6, "Ocrax", this);
    auto* territory3 = new Territory(7, "Tewhuicia Caskein", this);
    auto* territory4 = new Territory(8, "Eshana", this);
    attackTerritories.push_back(territory1);
    attackTerritories.push_back(territory2);
    attackTerritories.push_back(territory3);
    attackTerritories.push_back(territory4);
    //Temporary return
    return attackTerritories;
}

bool Player::issueOrder() {
    //TODO wait for Orders Implementation
    return false;
}

string Player::getPlayerName() {
    return name;
}

void Player::setPlayerName(string newName) {
    this->name = std::move(newName);
}

ostream& operator<<(ostream& stream, const Player& player) {
    stream << "Player Name: " << player.name << endl;
    stream << "Owned Territories:" << endl;
    int size = 1;
    for (auto& terr: player.territories) {
        if (size == player.territories.size()) {
            cout << terr->getTerritoryName() << "\r\n" << endl;
        } else {
            cout << terr->getTerritoryName() << ", ";
        }
        size++;
    }
    stream << "HandCards: " << endl;
    stream << player.handCards << endl;
    stream << "OrderList: " << endl;
    stream << player.ordersList << endl;
    return stream;
}

