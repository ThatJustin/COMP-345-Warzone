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

using namespace std;

/**
 * default constructor
 */
Orders::Orders() {
}

/**
 * destructor
 */
Orders::~Orders() {

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
    //Depending on the orders subclass, will stream different text based on it
    orders.toStreamInsertion(stream);
    return stream;
}

void Orders::toStreamInsertion(ostream& os) const {

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
Orders* createOrderByCardType(int cardType) {
    switch (cardType) {
        case BOMB:
            return new Bomb();
        case BLOCKADE:
            return new Blockade();
        case AIRLIFT:
            return new Airlift();
        case DIPLOMACY:
            return new Negotiate();
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
 * @param o order to add
 */
void OrdersList::add(Orders* o) {
    list.push_back(o);
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
 * @param orderslist
 * @return
 */
OrdersList& OrdersList::operator=(const OrdersList& orderslist) {
    if (this == &orderslist) {
        return *this;
    }
    this->list = orderslist.list;
    return *this;
}

/**
 * Stream data for the class.
 * @param stream
 * @param ordersList
 * @return
 */
ostream& operator<<(ostream& stream, const OrdersList& ordersList) {
    stream << "Order list contains these orders: " << endl;
    for (auto order: ordersList.list) {
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

/**
 * Constructor.
 */
Deploy::Deploy() {
    this->m_numberOfArmyUnits = 0;
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param numberOfArmyUnits
 * @param targetTerritory
 */
Deploy::Deploy(int numberOfArmyUnits, Territory* targetTerritory, Player* player) { //need to add player as a parameter
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
    return true;
}

/**
 * Executes the deploy order.
 */
void Deploy::execute() {
    cout << "Executing Deploy Order" << endl;
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
Advance::Advance(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory, Player* player) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_sourceTerritory = sourceTerritory;
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param advance
 */
Advance::Advance(const Advance& advance) {
    this->m_targetTerritory = advance.m_targetTerritory;
    this->m_sourceTerritory = advance.m_sourceTerritory;
    this->m_numberOfArmyUnits = advance.m_numberOfArmyUnits;
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
    return true;
}

/**
 * Executes the advance order.
 */
void Advance::execute() {
    cout << "Executing Advance Order" << endl;
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
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param targetTerritory
 */
Bomb::Bomb(Territory* targetTerritory, Player* player) {
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param bomb
 */
Bomb::Bomb(const Bomb& bomb) {
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
    return true;
}

/**
 * Executes the bomb order.
 */
void Bomb::execute() {
    cout << "Executing Bomb Order" << endl;
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
    this->m_targetTerritory = nullptr;
}

/**
 * Parameterized constructor.
 * @param targetTerritory
 */
Blockade::Blockade(Territory* targetTerritory, Player* player) {
    this->m_targetTerritory = targetTerritory;
}

/**
 * Copy constructor.
 * @param blockade
 */
Blockade::Blockade(const Blockade& blockade) {
    this->m_targetTerritory = blockade.m_targetTerritory;
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
    return true;
}

/**
 * Executes the blockade order.
 */
void Blockade::execute() {
    cout << "Executing Blockade Order" << endl;
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
Airlift::Airlift(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory, Player* player) {
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
    return true;
}

/**
 * Executes the airlift order.
 */
void Airlift::execute() {
    cout << "Executing Airlift Order" << endl;
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
}

/**
 * Parameterized constructor.
 * @param targetPlayer
 */
Negotiate::Negotiate(Player* targetPlayer, Player* player) {
    this->m_targetPlayer = targetPlayer;
}

/**
 *  * Copy constructor.
 * @param airlift
 */
Negotiate::Negotiate(const Negotiate& airlift) {
    this->m_targetPlayer = airlift.m_targetPlayer;
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
    return true;
}

/**
 * Executes the negotiate order.
 */
void Negotiate::execute() {
    cout << "Executing Negotiate Order" << endl;
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


