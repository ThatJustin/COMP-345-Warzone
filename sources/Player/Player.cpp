#include <iostream>
#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

/**
 * Default constructor.
 */
Player::Player() {
    this->territories = vector<Territory*>();
    this->name = "";
    this->handCards = new Hand;
    this->ordersList = new OrdersList();
}

/**
 * Parameterized Constructor.
 * @param name name of the player
 */
Player::Player(const string& name) {
    this->territories = vector<Territory*>();
    this->name = name;
    this->handCards = new Hand;
    this->ordersList = new OrdersList();
}

/**
 * Copy Constructor.
 * @param player
 */
Player::Player(const Player& player) {
    this->territories = player.territories;
    this->handCards = new Hand(*player.handCards);
    this->name = player.name;
    this->ordersList = new OrdersList(*player.ordersList);
}

/**
 * Destructor.
 */
Player::~Player() {
    //Player doesn't delete the territory as it doesn't own the territories object,
    // the player just points to it.
    if (!this->getTerritories().empty()) {
        for (auto& terr: this->getTerritories()) {
            terr = nullptr;
        }
    }
    if (handCards != nullptr) {
        delete handCards;
        handCards = nullptr;
    }
    if (ordersList != nullptr) {
        delete ordersList;
        ordersList = nullptr;
    }
}

/**
 * Getter fpr the list of player territories.
 * @return vector of territory pointers
 */
vector<Territory*> Player::getTerritories() {
    return this->territories;
}

/*
 * Adds a territory to the player.
 */
void Player::addTerritory(Territory* territory) {
    this->territories.push_back(territory);
}

/**
 * Getter for player orderlist which holds orders the player has issued.
 * @return OrdersList pointer
 */
OrdersList* Player::getOrdersList() {
    return ordersList;
}

/**
 * Getter for players hand which holds cards.
 * @return Hand pointer
 */
Hand* Player::getHandCards() {
    return handCards;
}

/**
 * Returns a vector of territories (pointer) the player defends.
 * @return vector territories
 */
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

/**
 * Returns a vector of territories (pointer) the player will attack.
 * @return vector territories
 */
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
    return attackTerritories;
}

/**
 * Issues an order during the payer issue order phase.
 * OrderIssuingPhase:
 * 1.The player decides which neighboring territories are to be attacked in priority
    (as a list return by the toAttack() method),
    and which of their own territories are to be defended in priority (as a list returned by the toDefend() method).
 * 2.The player issues deploy orders on its own territories that are in the list returned by toDefend().
    As long as the player has army units in their reinforcement pool (see startup phase and reinforcement phase),
    it will issue a deploy order and no other order. Once it has deployed all its available army units, it can proceed with other kinds of orders.
 * 3.The player issues advance orders to either (1) move army units from one of its own territory to another of its own territories in order to defend it
    (using toDefend() to make the decision), and/or (2) move army units from one of its own territories to a neighboring enemy territory to attack them
    (using toAttack() to make the decision).
 * 4.The player uses one of the cards in their hand to issue an order that corresponds to the card in question.
 * @return
 */
bool Player::issueOrder(Map *map, Player *player, Deck *deck) { //need to add commandprocessor as a parameter here

    /*
    //For testing purposes in assignment 1, will create an order and issue it
    Orders* bomb = new Bomb();
    Orders* airlift = new Airlift();
    Orders* negotiate = new Negotiate();
    this->ordersList->add(bomb);
    this->ordersList->add(airlift);
    this->ordersList->add(negotiate);
     */

    //The player decides which neighboring territories are to be attacked in priority
    //(as a list return by the toAttack() method),
    //and which of their own territories are to be defended in priority (as a list returned by the toDefend() method).



    return true;
}

/**
 * Getter for player name.
 * @return payer name
 */
string Player::getPlayerName() {
    return name;
}

/**
 * Set's the player named based on the passed input.
 * @param newName
 */
void Player::setPlayerName(string newName) {
    this->name = std::move(newName);
}

/*
 * Assigns the players hand of cards.
 */
void Player::setHandCards(Hand* newHand) {
    this->handCards = newHand;
}

/**
 * Overrides the stream insertion operator to show data for the player class.
 * @param stream
 * @param player
 * @return Player class information
 */
ostream& operator<<(ostream& stream, const Player& player) {
    stream << "Player Name: " << player.name << endl;
    stream << "Owned Territories: ";
    int size = 1;
    //Show all the owned territories of this player
    for (auto& terr: player.territories) {
        if (size == player.territories.size()) {
            cout << terr->getTerritoryName() << "\r\n";
        } else {
            cout << terr->getTerritoryName() << ", ";
        }
        size++;
    }
    stream << endl;
    stream << "HandCards: ";
    size = 1;
    //Show all the hand cards of this player
    for (auto& card: player.handCards->getCards()) {
        if (size == player.handCards->getCards().size()) {
            cout << getNameByCardType(card->getType()) << "\r\n";
        } else {
            cout << getNameByCardType(card->getType()) << ", ";
        }
        size++;
    }
    stream << "OrderList: ";
    size = 1;
    //Show all the orders issued for this player
    for (auto& order: player.ordersList->getOrdersList()) {
        if (size == player.ordersList->getOrdersList().size()) {
            cout << getNameByOrderType(order->getOrderType()) << "\r\n";
        } else {
            cout << getNameByOrderType(order->getOrderType()) << ", ";
        }
        size++;
    }
    return stream;
}

/**
 * Overrides the assignment operator for the player class.
 * @param player
 * @return
 */
Player& Player::operator=(const Player& player) {
    if (this == &player) {
        return *this;
    }
    this->name = player.name;
    this->ordersList = new OrdersList(*player.ordersList);
    this->handCards = new Hand(*player.handCards);
    this->territories = player.territories;
    return *this;
}

/**
 * for part 4
 * @return
 */
int Player::getArmy() {

    return army;
}

/**
 * for part 4
 * @param army
 */
void Player::setArmy(int reinforcement) {

    army = reinforcement;
}

/**
 *remove the order from the ordered list
 * @return
 */
Orders* Player::removeOrder() {
    //if there is no order in the list return null
    if (ordersList->getOrdersList().size() == 0) {
        return NULL;
    }
    //put the order in the orderlist
    auto orderlist = ordersList->getOrdersList();

    Orders* order = orderlist.at(0);
    ordersList->remove(0);
    return order;
}

