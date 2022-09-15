#include "Player.h"

Player::Player() {
    this->territories = vector<Territory*>();
    this->name = "";
    this->handCards = new Hand;
    this->orderList = new OrderList;
}

Player::Player(const string& name) {
    this->territories = vector<Territory*>();
    this->name = name;
    this->handCards = new Hand;
    this->orderList = new OrderList;
}

Player::~Player() {
    delete handCards;
    delete orderList;
}

vector<Territory*> Player::getTerritories() {
    return territories;
}

OrderList* Player::getOrderList() {
    return orderList;
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
