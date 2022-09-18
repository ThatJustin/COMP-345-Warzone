#include <iostream>
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
    this->territories = player.territories;
    this->handCards = player.handCards;
    this->name = player.name;
    this->ordersList = player.ordersList;
}

Player::~Player() {
    delete handCards;
    delete ordersList;
}

vector<Territory*> Player::getTerritories() {
//    std::cout << "called " << std::endl;
    return territories;
}

OrdersList* Player::getOrdersList() {
    return ordersList;
}

Hand* Player::getHandCards() {
    return handCards;
}

vector<Territory*> Player::toDefend() {
    //Temporary return
    return {};
}

vector<Territory*> Player::toAttack() {
    //Temporary return
    return {};
}

bool Player::issueOrder() {
    //TODO wait for Orders Implementation
    return false;
}

string Player::getPlayerName() {
    return name;
}

