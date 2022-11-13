//
// Created by Alex De Luca on 2022-09-14.
//
#include "Orders.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include <random>

using namespace std;

/**
 * default constructor
 */
Orders::Orders() {
    this->player = nullptr;
}

/**
 * Parameterized constructor
 */
Orders::Orders(Player* orderPlayer) {
    this->player = orderPlayer;
}

/**
 * destructor
 */
Orders::~Orders() {
    if (player != nullptr) {
        player = nullptr;
    }
    detach(observer);
}

/**
 * Assignment operator.
 * @param order
 * @return
 */
Orders& Orders::operator=(const Orders& order) {
    if (this == &order) {
        return *this;
    }
    return *this;
}

/**
 * Uses toStreamInsertion (implemented in every Order sub class) to stream out class specific data.
 * A nice mimic for inheritance on the << operator.
 * Learned about this here: https://cplusplus.com/forum/beginner/234681/#msg1053068
 * @param stream 
 * @param orders 
 * @return 
 */
ostream& operator<<(ostream& stream, const Orders& orders) {
    //Depending on the orders' subclass, will stream different text based on it
    orders.toStreamInsertion(stream);
    return stream;
}

void Orders::toStreamInsertion(ostream& os) const {

}

void Orders::attach(Observer* obs) {
    Subject::attach(obs);
    observer = obs;
}

void Orders::detach(Observer* obs) {
    Subject::detach(obs);
    observer = nullptr;
}

string Orders::stringToLog() {
    if (orderResult.empty()) {
        return "[Order] Order " + getNameByOrderType(getOrderType()) + " executed by player " + this->player->getPlayerName() + ". Result: " + orderResult;
    } else {
        return "[Order] Order " + getNameByOrderType(getOrderType()) + " executed by player " + this->player->getPlayerName() + " has failed.";
    }
}

/**
 * return the name based on the type of order
 * @param cardType
 * @return
 */
string getNameByOrderType(OrderType cardType) {
    switch (cardType) {
        case OrderType::DEPLOY:
            return "deploy";
        case OrderType::ADVANCE:
            return "advance";
        case OrderType::BOMB:
            return "bomb";
        case OrderType::BLOCKADE:
            return "blockade";
        case OrderType::AIRLIFT:
            return "airlift";
        case OrderType::NEGOTIATE:
            return "negotiate";
    }
    return "none";
}

/**
 * return order type based on card type
 * @param cardType
 * @return
 */
Orders* createOrderByCardType(int cardType, Player* player, Player* targetPlayer, int numberOfArmyUnits,
                              Territory* sourceTerritory, Territory* targetTerritory, Player* neutral) {
    switch (cardType) {
        case BOMB:
            return new Bomb(player, targetTerritory);
        case BLOCKADE:
            return new Blockade(player, neutral, targetTerritory);
        case AIRLIFT:
            return new Airlift(player, numberOfArmyUnits, sourceTerritory, targetTerritory);
        case DIPLOMACY:
            return new Negotiate(player, targetPlayer);
        default:
            return nullptr;
    }
}


/**
 * OrdersList default constructor
 */
OrdersList::OrdersList() {
    this->list = std::vector<Orders*>();
}

/**
 * /OrdersList destructor, also deletes every order in the list
 */
OrdersList::~OrdersList() {
    for (auto order: list) {
        delete order;
        order = nullptr;
    }
    list.clear();
    detach(observer);
}

/**
 * OrdersList copy constructor
 * @param ol
 */
OrdersList::OrdersList(const OrdersList& ol) {
    this->list = vector<Orders*>(ol.list.size());
    for (auto& temp: ol.list) {
        this->list.push_back(temp->copy());
    }
}

/**
 * Moves an element from the order list using the input index (from and to)
 * @param from what element index to move
 * @param to where to move it
 */
void OrdersList::move(int from, int to) {
    /**
     * if both inputs are equal
     */
    if (from == to) return;

    /*
     * if inputs are different
     */
    int index_from = from - 1, index_to = to - 1;
    Orders* temp = list[index_from];
    list.erase(list.begin() + index_from);
    list.insert(list.begin() + index_to, temp);
}

/**
 * Removes an order from the orderslist based on the index.
 * @param index to remove
 */
void OrdersList::remove(int index) {
    /*
     * deletes order - 1 to account for the index, as the user will delete orders starting from 1
     */
    list.erase(list.begin() + index - 1);
}

/**
 * Pushes an order to the orderslist.
 * @param o order to addOrder
 */
void OrdersList::addOrder(Orders* o) {
    list.push_back(o);
    o->attach(observer);
    notify(this);
}

/**
 * Getter for orderslist.
 * @return orderslist
 */
vector<Orders*> OrdersList::getOrdersList() {
    return this->list;
}

/**
 * Assignment operator for the class.
 * @param orders_list
 * @return
 */
OrdersList& OrdersList::operator=(const OrdersList& orders_list) {
    if (this == &orders_list) {
        return *this;
    }
    this->list = orders_list.list;
    return *this;
}

/**
 * Stream data for the class.
 * @param stream
 * @param orders_list
 * @return
 */
ostream& operator<<(ostream& stream, const OrdersList& orders_list) {
    stream << "Order list contains these orders: " << endl;
    for (auto order: orders_list.list) {
        cout << *order;
    }
    return stream;
}

void OrdersList::displayList() {
    cout << "Orderslist contains (" << list.size() << ") orders in order: [";
    int size = 1;
    for (auto& order: this->list) {
        if (size == list.size()) {
            cout << getNameByOrderType(order->getOrderType()) << "]\r\n" << endl;
        } else {
            cout << getNameByOrderType(order->getOrderType()) << ", ";
        }
        size++;
    }
}

void OrdersList::attach(Observer* obs) {
    Subject::attach(obs);
    observer = obs;
}

void OrdersList::detach(Observer* obs) {
    Subject::detach(obs);
    observer = nullptr;
}

string OrdersList::stringToLog() {
    Orders* orders = this->getOrdersList().back();
    return "[OrderList] Order " + getNameByOrderType(orders->getOrderType()) + " added by player " +
           orders->player->getPlayerName() + ".";
}

/**
 * Constructor.
 */
Deploy::Deploy() : Orders() {
    this->player = nullptr;
    this->m_numberOfArmyUnits = 0;
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param numberOfArmyUnits
 * @param targetTerritory
 */
Deploy::Deploy(Player* player, int numberOfArmyUnits, Territory* targetTerritory) : Orders(player) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param deploy
 */
Deploy::Deploy(const Deploy& deploy) {
    this->m_targetTerritory = deploy.m_targetTerritory;
    this->m_numberOfArmyUnits = deploy.m_numberOfArmyUnits;
    this->player = deploy.player;
}

/**
 * Destructor.
 */
Deploy::~Deploy() {
    if (m_targetTerritory != nullptr) {
        m_targetTerritory = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Deploy::validate() {
    if (m_targetTerritory->getPlayerName() == player->getPlayerName() &&
        !player->checkIsNegotiation(m_targetTerritory->getPlayer())) {
        cout << "Deploy Order is valid for " << player->getPlayerName() << " on " << m_targetTerritory->getTerritoryName() << "." << endl;
        return true;
    }
    return false;
}

/**
 * Executes the deploy order.
 */
void Deploy::execute() {
    cout << "Executing Deploy Order" << endl;
    if (validate()) {
        if (player->getReinforcementPool() - m_numberOfArmyUnits > 0) {
            int number_of_armies_target_territory = m_targetTerritory->getNumberOfArmies();
            //Add the units to the territory
            m_targetTerritory->setNumberOfArmies(number_of_armies_target_territory + m_numberOfArmyUnits);
            // remove them from the players pool
            player->setReinforcementPool(player->getReinforcementPool() - m_numberOfArmyUnits);
            orderResult = "Deploy Order successfully executed: Successfully deployed " + to_string(m_numberOfArmyUnits) + " army units to " + m_targetTerritory->getTerritoryName() +
                    "! Remaining army units in " + player->getPlayerName() + "'s reinforcement pool: " + to_string(player->getReinforcementPool()) + ". " +
                    m_targetTerritory->getTerritoryName() + " now has " + to_string(m_targetTerritory->getNumberOfArmies()) + " army units.";
        } else {
            cout << player->getPlayerName() + " does not have enough armies to deploy to " + m_targetTerritory->getTerritoryName() + "." << endl;
            orderResult = player->getPlayerName() + " does not have enough armies to deploy to " + m_targetTerritory->getTerritoryName() + ".";
        }
        notify(this);
    } else {
        cout << "Failed to execute Deploy Order." << endl;
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */
Orders* Deploy::copy() {
    return new Deploy(*this);
}

/**
 * Getter for army unit number.
 * @return
 */
int Deploy::getNumberOfArmyUnits() {
    return m_numberOfArmyUnits;
}

/**
 * Setter for army unit number.
 * @param numberOfArmyUnits
 */
void Deploy::setNumberOfArmyUnits(int numberOfArmyUnits) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
}

/**
 * Getter for target territory of this order.
 * @return
 */
Territory* Deploy::getTargetTerritory() {
    return m_targetTerritory;
}

/*
 * Setter for target territory of this order.
 */
void Deploy::setTargetTerritory(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
std::string Deploy::showDescription() {
    std::stringstream ss;
    ss << "Deploy: " << endl;
    ss
            << "move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Deploy::getOrderType() {
    return OrderType::DEPLOY;
}

/**
 * Assignment operator for the class.
 * @param deploy
 * @return
 */
Deploy& Deploy::operator=(const Deploy& deploy) {
    if (this == &deploy) {
        return *this;
    }
    Orders::operator=(deploy);
    this->m_targetTerritory = deploy.m_targetTerritory;
    this->m_numberOfArmyUnits = deploy.m_numberOfArmyUnits;
    this->player = deploy.player;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Deploy::toStreamInsertion(ostream& os) const {
    os << "Deploy" << endl;
    os << "\tArmy number units: " << this->m_numberOfArmyUnits << endl;
    if (this->m_targetTerritory != nullptr) {
        os << "\tTarget Territory: " << this->m_targetTerritory->getTerritoryName() << endl;
    } else {
        os << "\tTarget Territory: NONE " << endl;
    }
}

/**
 * Constructor.
 */
Advance::Advance() {
    this->player = nullptr;
    this->m_numberOfArmyUnits = 0;
    this->m_sourceTerritory = nullptr;
    this->m_targetTerritory = nullptr;
    this->m_deck = nullptr;
}

/**
 * Parameterized constructor.
 * @param numberOfArmyUnits
 * @param sourceTerritory
 * @param targetTerritory
 */
Advance::Advance(Player* player, int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory,
                 Deck* deck)
        : Orders(player) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_sourceTerritory = sourceTerritory;
    this->m_targetTerritory = targetTerritory;
    this->m_deck = deck;
}

/**
 * Copy constructor.
 * @param advance
 */
Advance::Advance(const Advance& advance) {
    this->m_targetTerritory = advance.m_targetTerritory;
    this->m_sourceTerritory = advance.m_sourceTerritory;
    this->m_numberOfArmyUnits = advance.m_numberOfArmyUnits;
    this->player = advance.player;
    this->m_deck = advance.m_deck;
}

/**
 * Destructor.
 */
Advance::~Advance() {
    if (m_targetTerritory != nullptr) {
        m_sourceTerritory = nullptr;
    }
    if (m_targetTerritory != nullptr) {
        m_targetTerritory = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Advance::validate() {
    if (m_sourceTerritory->getPlayerName() == player->getPlayerName()
        && m_targetTerritory->isAdjacent(m_sourceTerritory) && m_sourceTerritory->isAdjacent(m_targetTerritory) &&
        !player->checkIsNegotiation(m_targetTerritory->getPlayer())) {
        cout << "Advance Order is valid for " << player->getPlayerName() << " from "
        << m_sourceTerritory->getTerritoryName() << " to " << m_targetTerritory->getTerritoryName() << "." << endl;
        return true;
    }
    cout << "Advance Order is not valid." << endl;
    return false;
}

/**
 * Executes the advance order.
 */
void Advance::execute() {
    cout << "Executing Advance Order" << endl;
    if (validate()) {
        if (m_targetTerritory->getPlayerName() == m_sourceTerritory->getPlayerName()) {
            int target_army_count = m_targetTerritory->getNumberOfArmies();
            int source_army_count = m_sourceTerritory->getNumberOfArmies();
            cout << "Moving Army: source_army_count -  " << source_army_count << "   target_army_count - "
                 << target_army_count << endl;
            m_targetTerritory->setNumberOfArmies(target_army_count + m_numberOfArmyUnits);
            m_sourceTerritory->setNumberOfArmies(source_army_count - m_numberOfArmyUnits);
            cout << "Army Moved Results: source_army_count -  " << m_sourceTerritory->getNumberOfArmies()
                 << "   target_army_count - " << m_targetTerritory->getNumberOfArmies() << endl;
            orderResult = "Advance Order successfully executed: Advancing army from " + m_sourceTerritory->getTerritoryName() + " to " +
                    m_targetTerritory->getTerritoryName() + " on " + m_targetTerritory->getTerritoryName() + " by " + m_sourceTerritory->getPlayerName() + ".";
        } else {
            int source_army_count = m_numberOfArmyUnits;
            int target_army_count = m_targetTerritory->getNumberOfArmies();
            cout << "Advance Order Start" << endl;
            cout << "Attacking Army Count: " << source_army_count << endl;
            cout << "Defending Army Count: " << target_army_count << endl;

            std::random_device rd1;
            std::mt19937 gen1(rd1());
            std::bernoulli_distribution killAttack(0.6);

            std::random_device rd2;
            std::mt19937 gen2(rd2());
            std::bernoulli_distribution killDefend(0.7);

            for (int i = 0; i < std::max(source_army_count, target_army_count); ++i) {
                if (source_army_count != 0 && target_army_count != 0) {
                    if (killAttack(gen1)) {
//                        cout << "killing defender ";
                        target_army_count = target_army_count - 1;
                    }
                    if (killDefend(gen2)) {
//                        cout << "killing attacker ";
                        source_army_count = source_army_count - 1;
                    }
                }
            }
            cout << "Remaining Attacking Army Count: " << source_army_count << endl;
            cout << "Remaining Defending Army Count: " << target_army_count << endl;
            cout << "Advance Order End" << endl;
            orderResult = "Advance Order end, the fight has ended! " + m_sourceTerritory->getTerritoryName() + " has " + to_string(source_army_count) + " armies left and " +
                    m_targetTerritory->getTerritoryName() + " has " + to_string(target_army_count) + " armies left.";
            if (target_army_count == 0) {
                cout << "Successfully conquered the target." << endl;
                Player* target_player = m_targetTerritory->getPlayer();
                m_targetTerritory->setTerritoryOwner(player);
                m_targetTerritory->setNumberOfArmies(source_army_count);
                m_deck->draw(player);
                orderResult = "Advance Order successful, territory is conquered: " + player->getPlayerName() + " has conquered " +
                        m_targetTerritory->getTerritoryName() + " from " + target_player->getPlayerName() + ".";
            }
        }
        notify(this);
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */
Orders* Advance::copy() {
    return new Advance(*this);
}

/**
 * Getter for army unit number.
 * @return
 */
int Advance::getNumberOfArmyUnits() {
    return m_numberOfArmyUnits;
}

/**
 * Setter for army unit number.
 * @param numberOfArmyUnits
 */
void Advance::setNumberOfArmyUnits(int numberOfArmyUnits) {
    m_numberOfArmyUnits = numberOfArmyUnits;
}

/*
 * Setter for source territory of this order.
 */
Territory* Advance::getSourceTerritory() {
    return m_sourceTerritory;
}

/**
 * Setter for source territory,
 * @param sourceTerritory
 */
void Advance::setSourceTerritory(Territory* sourceTerritory) {
    m_sourceTerritory = sourceTerritory;
}

/**
 * Getter for target territory of this order.
 * @return
 */
Territory* Advance::getTargetTerritory() {
    return m_targetTerritory;
}

/*
 * Setter for target territory of this order.
 */
void Advance::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
std::string Advance::showDescription() {
    std::stringstream ss;
    ss << "Advance: " << endl;
    ss << "move a certain number of army units from one of the current player’s territories (source) to another\n"
          "territory (target) that is adjacent to the source territory. If the target territory belongs to the current\n"
          "player, the armies are moved from the source territory to the target territory. If the target territory\n"
          "belongs to another player, an attack happens between the two territories. An attack is simulated by\n"
          "the following battle simulation mechanism: First, the attacking player decides how many armies in\n"
          "the source territory are involved in the attack. Then, each attacking army unit involved has 60%\n"
          "chances of destroying one defending army. At the same time, each defending army unit has 70%\n"
          "chances of destroying one attacking army unit. If all the army units on the target territory are\n"
          "destroyed as a result of the battle, the remaining army units left from the attacking army units from\n"
          "the source territory are moved to the target territory and the target territory now belongs to the player\n"
          "that declared the advance order, i.e. the player has conquered this territory. In any given turn, a\n"
          "player receives a card if they conquered at least one territory during this turn.";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Advance::getOrderType() {
    return OrderType::ADVANCE;
}

/**
 * Assignment operator for the class.
 * @param advance
 * @return
 */
Advance& Advance::operator=(const Advance& advance) {
    if (this == &advance) {
        return *this;
    }
    Orders::operator=(advance);
    this->m_targetTerritory = advance.m_targetTerritory;
    this->m_sourceTerritory = advance.m_sourceTerritory;
    this->m_numberOfArmyUnits = advance.m_numberOfArmyUnits;
    this->player = advance.player;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Advance::toStreamInsertion(ostream& os) const {
    os << "Advance" << endl;
    os << "\tArmy number units: " << this->m_numberOfArmyUnits << endl;
    if (this->m_sourceTerritory != nullptr) {
        os << "\tSource Territory: " << this->m_sourceTerritory->getTerritoryName() << endl;
    } else {
        os << "\tSource Territory: NONE " << endl;
    }
    if (this->m_targetTerritory != nullptr) {
        os << "\tTarget Territory: " << this->m_targetTerritory->getTerritoryName() << endl;
    } else {
        os << "\tTarget Territory: NONE " << endl;
    }
}

/**
 * Constructor.
 */
Bomb::Bomb() {
    this->player = nullptr;
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param targetTerritory
 */
Bomb::Bomb(Player* player, Territory* targetTerritory) : Orders(player) {
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param bomb
 */
Bomb::Bomb(const Bomb& bomb) {
    this->player = bomb.player;
    this->m_targetTerritory = bomb.m_targetTerritory;
}

/**
 * Destructor.
 */
Bomb::~Bomb() {
    if (m_targetTerritory != nullptr) {
        m_targetTerritory = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Bomb::validate() {
    bool flag = false;
    for (auto territory: player->getTerritories()) {
        if (m_targetTerritory->isAdjacent(territory)) {
            flag = true;
        }
    }
    if (m_targetTerritory->getPlayerName() != player->getPlayerName() && flag &&
        !player->checkIsNegotiation(m_targetTerritory->getPlayer())) {
        cout << "Bomb Order is valid for " << player->getPlayerName() << " on " << m_targetTerritory->getTerritoryName()
        << " (one of " << m_targetTerritory->getPlayerName() << "'s territories)." << endl;
        return true;
    }
    cout << "Bomb Order is not valid." << endl;
    return false;
}

/**
 * Executes the bomb order.
 */
void Bomb::execute() {
    cout << "Executing Bomb Order" << endl;
    if (validate()) {
        int army = m_targetTerritory->getNumberOfArmies();
        m_targetTerritory->setNumberOfArmies(round(army / 2));
        orderResult = "Bomb Order successfully executed: Eliminating half of " + m_targetTerritory->getPlayerName() + "'s army units on " +
                      m_targetTerritory->getTerritoryName() + " by " + player->getPlayerName() + ".";
        notify(this);
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */

Orders* Bomb::copy() {
    return new Bomb(*this);
}

/**
 * Getter for target territory of this order.
 * @return
 */
Territory* Bomb::getTargetTerritory() {
    return m_targetTerritory;
}

/**
 * Setter for target territory of this order.
 * @param targetTerritory
 */
void Bomb::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
string Bomb::showDescription() {
    std::stringstream ss;
    ss << "Bomb: " << endl;
    ss << "destroy half of the army units located on an opponent’s territory that is adjacent to one of the current\n"
          "player’s territories.";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Bomb::getOrderType() {
    return OrderType::BOMB;
}

/**
 * Assignment operator for the class.
 * @param bomb
 * @return
 */
Bomb& Bomb::operator=(const Bomb& bomb) {
    if (this == &bomb) {
        return *this;
    }
    Orders::operator=(bomb);
    this->player = bomb.player;
    this->m_targetTerritory = bomb.m_targetTerritory;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Bomb::toStreamInsertion(ostream& os) const {
    os << "Bomb" << endl;
    if (this->m_targetTerritory != nullptr) {
        os << "\tTarget Territory: " << this->m_targetTerritory->getTerritoryName() << endl;
    } else {
        os << "\tTarget Territory: NONE " << endl;
    }
}

/**
 * Constructor.
 */
Blockade::Blockade() {
    this->player = nullptr;
    this->m_targetTerritory = nullptr;
    this->neutral = nullptr;
}

/**
 * Parameterized constructor.
 * @param targetTerritory
 */
Blockade::Blockade(Player* player, Player* neutral, Territory* targetTerritory) : Orders(player) {
    this->m_targetTerritory = targetTerritory;
    this->neutral = neutral;
}

/**
 * Copy constructor.
 * @param blockade
 */
Blockade::Blockade(const Blockade& blockade) {
    this->m_targetTerritory = blockade.m_targetTerritory;
    this->player = blockade.player;
    this->neutral = blockade.neutral;
}

/**
 * Destructor.
 */
Blockade::~Blockade() {
    if (m_targetTerritory != nullptr) {
        m_targetTerritory = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Blockade::validate() {
    if (m_targetTerritory->getPlayerName() == player->getPlayerName() ||
        m_targetTerritory->getPlayerName() == neutral->getPlayerName()) {
        cout << "Blockade Order is valid for " << player->getPlayerName() << " on "
        << m_targetTerritory->getTerritoryName() << " (one of " << player->getPlayerName() << "'s territories)." << endl;
        return true;
    }
    cout << "Blockade Order is not valid." << endl;
    return false;
}

/**
 * Executes the blockade order.
 */
void Blockade::execute() {
    cout << "Executing Blockade Order" << endl;
    if (validate()) {
        int army = m_targetTerritory->getNumberOfArmies();
        m_targetTerritory->setNumberOfArmies(army * 2);
        m_targetTerritory->setTerritoryOwner(neutral);
        orderResult = "Blockade Order successfully executed: Doubled the army units on " + m_targetTerritory->getTerritoryName() +
                      " by " + player->getPlayerName() + " and transferred the ownership of " + m_targetTerritory->getTerritoryName()
                      + " to " + neutral->getPlayerName() + " (the neutral player in the game).";
        notify(this);
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */
Orders* Blockade::copy() {
    return new Blockade(*this);
}

/**
 * Getter for target territory of this order.
 * @return
 */
Territory* Blockade::getTargetTerritory() {
    return m_targetTerritory;
}

/**
 * Setter for target territory of this order.
 * @param targetTerritory
 */
void Blockade::setTargetTerritory(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
std::string Blockade::showDescription() {
    std::stringstream ss;
    ss << "Blockade: " << endl;
    ss << "triple the number of army units on one of the current player’s territories and make it a neutral territory.";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Blockade::getOrderType() {
    return OrderType::BLOCKADE;
}

/**
 * Assignment operator for the class.
 * @param blockade
 * @return
 */

Blockade& Blockade::operator=(const Blockade& blockade) {
    if (this == &blockade) {
        return *this;
    }
    Orders::operator=(blockade);
    this->player = blockade.player;
    this->m_targetTerritory = blockade.m_targetTerritory;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Blockade::toStreamInsertion(ostream& os) const {
    os << "Blockade" << endl;
    if (this->m_targetTerritory != nullptr) {
        os << "\tTarget Territory: " << this->m_targetTerritory->getTerritoryName() << endl;
    } else {
        os << "\tTarget Territory: NONE " << endl;
    }
}

/**
 * Constructor.
 */
Airlift::Airlift() {
    this->player = nullptr;
    this->m_numberOfArmyUnits = 0;
    this->m_sourceTerritory = nullptr;
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param numberOfArmyUnits
 * @param sourceTerritory
 * @param targetTerritory
 */
Airlift::Airlift(Player* player, int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory)
        : Orders(player) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_sourceTerritory = sourceTerritory;
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param airlift
 */
Airlift::Airlift(const Airlift& airlift) {
    this->m_targetTerritory = airlift.m_targetTerritory;
    this->m_sourceTerritory = airlift.m_sourceTerritory;
    this->m_numberOfArmyUnits = airlift.m_numberOfArmyUnits;
    this->player = airlift.player;
}

/**
 * Destructor.
 */
Airlift::~Airlift() {
    if (m_targetTerritory != nullptr) {
        m_sourceTerritory = nullptr;
    }
    if (m_targetTerritory != nullptr) {
        m_targetTerritory = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Airlift::validate() {
    if (m_sourceTerritory->getPlayerName() == player->getPlayerName() &&
        m_targetTerritory->getPlayerName() == player->getPlayerName()) {
        cout << "Airlift Order is valid for " << player->getPlayerName() << " to move army units from "
        << m_sourceTerritory->getTerritoryName() << " to " << m_targetTerritory->getTerritoryName() << "." << endl;
        return true;
    }
    cout << "Airlift Order is not valid." << endl;
    return false;
}

/**
 * Executes the airlift order.
 */
void Airlift::execute() {
    cout << "Executing Airlift Order" << endl;
    if (validate()) {
        int source_army_count = m_targetTerritory->getNumberOfArmies();
        int target_army_count = m_sourceTerritory->getNumberOfArmies();
        m_targetTerritory->setNumberOfArmies(target_army_count + m_numberOfArmyUnits);
        m_sourceTerritory->setNumberOfArmies(source_army_count - m_numberOfArmyUnits);
        orderResult = "Airlift Order successfully executed: Moved " + to_string(m_numberOfArmyUnits) + " army units from " +
                      m_sourceTerritory->getTerritoryName() + " to " + m_targetTerritory->getTerritoryName() + " by " +
                      player->getPlayerName() + ".";
        notify(this);
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */
Orders* Airlift::copy() {
    return new Airlift(*this);
}

/**
 * Getter for army unit number.
 * @return
 */
int Airlift::getNumberOfArmyUnits() {
    return m_numberOfArmyUnits;
}

/**
 * Setter for army unit number.
 * @param numberOfArmyUnits
 */
void Airlift::setNumberOfArmyUnits(int numberOfArmyUnits) {
    m_numberOfArmyUnits = numberOfArmyUnits;
}

/**
 * Getter for source territory of this order.
 * @return
 */
Territory* Airlift::getSourceTerritory() {
    return m_sourceTerritory;
}

/**
 * Setter for source territory of this order.
 * @param sourceTerritory
 */
void Airlift::setSourceTerritory(Territory* sourceTerritory) {
    m_sourceTerritory = sourceTerritory;
}

/**
 * Getter for target territory of this order.
 * @return
 */
Territory* Airlift::getTargetTerritory() {
    return m_targetTerritory;
}

/**
 * Setter for target territory of this order.
 * @param targetTerritory
 */
void Airlift::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
std::string Airlift::showDescription() {
    std::stringstream ss;
    ss << "Airlift: " << endl;
    ss << "advance a certain number of army units from one of the current player’s territories to any another\n"
          "territory.";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Airlift::getOrderType() {
    return OrderType::AIRLIFT;
}

/**
 * Assignment operator for the class.
 * @param airlift
 * @return
 */

Airlift& Airlift::operator=(const Airlift& airlift) {
    if (this == &airlift) {
        return *this;
    }
    Orders::operator=(airlift);
    this->player = airlift.player;
    this->m_targetTerritory = airlift.m_targetTerritory;
    this->m_sourceTerritory = airlift.m_sourceTerritory;
    this->m_numberOfArmyUnits = airlift.m_numberOfArmyUnits;
    this->player = airlift.player;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Airlift::toStreamInsertion(ostream& os) const {
    os << "Airlift" << endl;
    os << "\tArmy number units: " << this->m_numberOfArmyUnits << endl;
    if (this->m_sourceTerritory != nullptr) {
        os << "\tSource Territory: " << this->m_sourceTerritory->getTerritoryName() << endl;
    } else {
        os << "\tSource Territory: NONE " << endl;
    }
    if (this->m_targetTerritory != nullptr) {
        os << "\tTarget Territory: " << this->m_targetTerritory->getTerritoryName() << endl;
    } else {
        os << "\tTarget Territory: NONE " << endl;
    }
}

/**
 * Constructor.
 */
Negotiate::Negotiate() {
    this->m_targetPlayer = nullptr;
    this->player = nullptr;
}

/**
 * Parameterized constructor.
 * @param targetPlayer
 */
Negotiate::Negotiate(Player* player, Player* targetPlayer) : Orders(player) {
    this->m_targetPlayer = targetPlayer;
}

/**
 *  * Copy constructor.
 * @param negotiate
 */
Negotiate::Negotiate(const Negotiate& negotiate) {
    this->m_targetPlayer = negotiate.m_targetPlayer;
    this->player = negotiate.player;
}

/**
 * Destructor.
 */
Negotiate::~Negotiate() {
    if (m_targetPlayer != nullptr) {
        m_targetPlayer = nullptr;
    }
}

/**
 * Checks if the order is valid
 * @return true if valid, false otherwise
 */
bool Negotiate::validate() {
    if (m_targetPlayer->getPlayerName() != player->getPlayerName()) {
        cout << "Negotiate Order is valid between " << player->getPlayerName() << " and " << m_targetPlayer->getPlayerName() << "." << endl;
        return true;
    }
    cout << "Negotiate Order is not valid: cannot negotiate with yourself (Current Player: "
    << player->getPlayerName() << "; Target Player to Negotiate with: " << m_targetPlayer->getPlayerName() << ")." << endl;
    return false;
}

/**
 * Executes the negotiate order.
 */
void Negotiate::execute() {
    cout << "Executing Negotiate Order" << endl;
    if (validate()) {
        player->setNegotiationWith(m_targetPlayer);
        m_targetPlayer->setNegotiationWith(player);
        cout << "Negotiate Order successfully executed: " << player->getPlayerName()
        << " is successfully negotiating with " << m_targetPlayer->getPlayerName() << "." << endl;
        orderResult = "Negotiate Order successfully executed: " + player->getPlayerName()
                + " is successfully negotiating with " + m_targetPlayer->getPlayerName() + ".";
        notify(this);
    }
}

/**
 * Creates a deep copy of the order and returns it.
 * @return deep copy of order
 */
Orders* Negotiate::copy() {
    return new Negotiate(*this);
}

/**
 * Getter for target player for this order.
 * @return
 */
Player* Negotiate::getTargetPlayer() {
    return this->m_targetPlayer;
}

/**
 * Setter for target player to use this order on.
 * @param targetPlayer
 */
void Negotiate::setTargetPlayer(Player* targetPlayer) {
    this->m_targetPlayer = targetPlayer;
}

/**
 * Show's a description of what the order does.
 * @return string of description
 */
std::string Negotiate::showDescription() {
    std::stringstream ss;
    ss << "Negotiate: " << endl;
    ss
            << "prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.";
    return ss.str();
}

/**
 * Gets the order type.
 * @return order type
 */
OrderType Negotiate::getOrderType() {
    return OrderType::NEGOTIATE;
}

/**
 * Assignment operator for the class.
 * @param negotiate
 * @return
 */

Negotiate& Negotiate::operator=(const Negotiate& negotiate) {
    if (this == &negotiate) {
        return *this;
    }
    Orders::operator=(negotiate);
    this->m_targetPlayer = negotiate.m_targetPlayer;
    return *this;
}

/**
 * Mimics << operator inheritance for this sub class. Is called in the base classes << operator function.
 * @param os
 */
void Negotiate::toStreamInsertion(ostream& os) const {
    os << "Negotiate" << endl;
    if (this->m_targetPlayer != nullptr) {
        os << "\tTarget Player: " << this->m_targetPlayer->getPlayerName() << endl;
    } else {
        os << "\tTarget Player: NONE " << endl;
    }
}


