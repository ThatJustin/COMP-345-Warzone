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

//default constructor
Orders::Orders() {
}

//destructor
Orders::~Orders() {

}

//<< operator
std::ostream& operator<<(ostream& stream, const Orders& order) {
    return stream<<order.toString();
}

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

OrdersList::OrdersList() {
    this->list = std::vector<Orders*>();
}

OrdersList::~OrdersList() {
    for (auto order: list) {
        delete order;
        order = nullptr;
    }
}

OrdersList::OrdersList(const OrdersList& ol) {
    this->list = vector<Orders*>(ol.list.size());
    for (auto& temp: ol.list) {
        this->list.push_back(temp->copy());
    }
}

void OrdersList::move(int from, int to) {
    //if both inputs are equal
    if(from == to) return;

    //if inputs are different
    int index_from = from-1, index_to = to-1;
    Orders* temp=list[index_from];
    list.erase(list.begin()+index_from);
    list.insert(list.begin()+index_to, temp);
}

void OrdersList::remove(int order) {
    list.erase(list.begin() + order);
}

void OrdersList::add(Orders* o) {
    list.push_back(o);
    cout<<*o;
}

vector<Orders*> OrdersList::getOrdersList() {
    return this->list;
}

void OrdersList::displayList() {
    for(int i=0;i<list.size();i++){
        cout<<i+1<<". "<<*list[i]<<endl;
    }
}

Deploy::Deploy() {
    this->m_numberOfArmyUnits = 0;
    this->m_targetTerritory = nullptr;
}

Deploy::Deploy(int numberOfArmyUnits, Territory* targetTerritory) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_targetTerritory = targetTerritory;
}

Deploy::Deploy(const Deploy& deploy) {
    this->m_targetTerritory = deploy.m_targetTerritory;
    this->m_numberOfArmyUnits = deploy.m_numberOfArmyUnits;
}

Deploy::~Deploy() {
    if (m_targetTerritory != nullptr) {
        delete m_targetTerritory;
        m_targetTerritory = nullptr;
    }
}

bool Deploy::validate() {
    return false;
}

void Deploy::execute() {
    cout << "Executing Deploy Order" << endl;
}

Orders* Deploy::copy() {
    return new Deploy(*this);
}

int Deploy::getNumberOfArmyUnits() const {
    return m_numberOfArmyUnits;
}

void Deploy::setNumberOfArmyUnits(int numberOfArmyUnits) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
}

Territory* Deploy::getTargetTerritory() {
    return m_targetTerritory;
}

void Deploy::setTargetTerritory(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}
//toString output of the order descriptions

std::string Deploy::toString() const {
    std::stringstream ss;
    ss<<"Deploy: "<<endl;
    ss
            << "move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories";
    return ss.str();
}

OrderType Deploy::getOrderType() {
    return OrderType::DEPLOY;
}

Advance::Advance() {
    this->m_numberOfArmyUnits = 0;
    this->m_sourceTerritory = nullptr;
    this->m_targetTerritory = nullptr;
}

Advance::Advance(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_sourceTerritory = sourceTerritory;
    this->m_targetTerritory = targetTerritory;
}

Advance::Advance(const Advance& advance) {
    this->m_targetTerritory = advance.m_targetTerritory;
    this->m_sourceTerritory = advance.m_sourceTerritory;
    this->m_numberOfArmyUnits = advance.m_numberOfArmyUnits;
}

Advance::~Advance() {
    if (m_targetTerritory != nullptr) {
        delete m_sourceTerritory;
        m_sourceTerritory = nullptr;
    }
    if (m_targetTerritory != nullptr) {
        delete m_targetTerritory;
        m_targetTerritory = nullptr;
    }
}

bool Advance::validate() {
    return false;
}

void Advance::execute() {
    cout << "Executing Advance Order" << endl;
}

Orders* Advance::copy() {
    return new Advance(*this);
}

int Advance::getNumberOfArmyUnits() const {
    return m_numberOfArmyUnits;
}

void Advance::setNumberOfArmyUnits(int numberOfArmyUnits) {
    m_numberOfArmyUnits = numberOfArmyUnits;
}

Territory* Advance::getSourceTerritory() {
    return m_sourceTerritory;
}

void Advance::setSourceTerritory(Territory* sourceTerritory) {
    m_sourceTerritory = sourceTerritory;
}

Territory* Advance::getTargetTerritory() {
    return m_targetTerritory;
}

void Advance::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

//toString output of the order descriptions

std::string Advance::toString() const {
    std::stringstream ss;
    ss<<"Advance: "<<endl;
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

OrderType Advance::getOrderType() {
    return OrderType::ADVANCE;
}

Bomb::Bomb() {
    this->m_targetTerritory = nullptr;
}

Bomb::Bomb(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}

Bomb::Bomb(const Bomb& bomb) {
    this->m_targetTerritory = bomb.m_targetTerritory;
}

Bomb::~Bomb() {
    if (m_targetTerritory != nullptr) {
        delete m_targetTerritory;
        m_targetTerritory = nullptr;
    }
}

bool Bomb::validate() {
    return false;
}

void Bomb::execute() {
    cout << "Executing Bomb Order" << endl;
}

Orders* Bomb::copy() {
    return new Bomb(*this);
}

Territory* Bomb::getTargetTerritory() {
    return m_targetTerritory;
}

void Bomb::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

//toString output of the order descriptions

string Bomb::toString() const {
    std::stringstream ss;
    ss<<"Bomb: "<<endl;
    ss << "destroy half of the army units located on an opponent’s territory that is adjacent to one of the current\n"
          "player’s territories.";
    return ss.str();
}

OrderType Bomb::getOrderType() {
    return OrderType::BOMB;
}

Blockade::Blockade() {
    this->m_targetTerritory = nullptr;
}

Blockade::Blockade(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}

Blockade::Blockade(const Blockade& blockade) {
    this->m_targetTerritory = blockade.m_targetTerritory;
}

Blockade::~Blockade() {
    if (m_targetTerritory != nullptr) {
        delete m_targetTerritory;
        m_targetTerritory = nullptr;
    }
}

bool Blockade::validate() {
    return false;
}

void Blockade::execute() {
    cout << "Executing Blockade Order" << endl;
}

Orders* Blockade::copy() {
    return new Blockade(*this);
}

Territory* Blockade::getTargetTerritory() {
    return m_targetTerritory;
}

void Blockade::setTargetTerritory(Territory* targetTerritory) {
    this->m_targetTerritory = targetTerritory;
}

//toString output of the order descriptions

std::string Blockade::toString() const {
    std::stringstream ss;
    ss<<"Blockade: "<<endl;
    ss << "triple the number of army units on one of the current player’s territories and make it a neutral territory.";
    return ss.str();
}

OrderType Blockade::getOrderType() {
    return OrderType::BLOCKADE;
}

Airlift::Airlift() {
    this->m_numberOfArmyUnits = 0;
    this->m_sourceTerritory = nullptr;
    this->m_targetTerritory = nullptr;
}

Airlift::Airlift(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory) {
    this->m_numberOfArmyUnits = numberOfArmyUnits;
    this->m_sourceTerritory = sourceTerritory;
    this->m_targetTerritory = targetTerritory;
}

Airlift::Airlift(const Airlift& airlift) {
    this->m_targetTerritory = airlift.m_targetTerritory;
    this->m_sourceTerritory = airlift.m_sourceTerritory;
    this->m_numberOfArmyUnits = airlift.m_numberOfArmyUnits;
}

Airlift::~Airlift() {
    if (m_targetTerritory != nullptr) {
        delete m_sourceTerritory;
        m_sourceTerritory = nullptr;
    }
    if (m_targetTerritory != nullptr) {
        delete m_targetTerritory;
        m_targetTerritory = nullptr;
    }
}

bool Airlift::validate() {
    return false;
}

void Airlift::execute() {
    cout << "Executing Airlift Order" << endl;
}

Orders* Airlift::copy() {
    return new Airlift(*this);
}

int Airlift::getNumberOfArmyUnits() const {
    return m_numberOfArmyUnits;
}

void Airlift::setNumberOfArmyUnits(int numberOfArmyUnits) {
    m_numberOfArmyUnits = numberOfArmyUnits;
}

Territory* Airlift::getSourceTerritory() {
    return m_sourceTerritory;
}

void Airlift::setSourceTerritory(Territory* sourceTerritory) {
    m_sourceTerritory = sourceTerritory;
}

Territory* Airlift::getTargetTerritory() {
    return m_targetTerritory;
}

void Airlift::setTargetTerritory(Territory* targetTerritory) {
    m_targetTerritory = targetTerritory;
}

//toString output of the order descriptions

std::string Airlift::toString() const {
    std::stringstream ss;
    ss<<"Airlift: "<<endl;
    ss << "advance a certain number of army units from one of the current player’s territories to any another\n"
          "territory.";
    return ss.str();
}

OrderType Airlift::getOrderType() {
    return OrderType::AIRLIFT;
}

Negotiate::Negotiate() {
    this->m_targetPlayer = nullptr;
}

Negotiate::Negotiate(Player* targetPlayer) {
    this->m_targetPlayer = targetPlayer;
}

Negotiate::Negotiate(const Negotiate& airlift) {
    this->m_targetPlayer = airlift.m_targetPlayer;
}

Negotiate::~Negotiate() {
    if (m_targetPlayer != nullptr) {
        m_targetPlayer = nullptr;
    }
}

bool Negotiate::validate() {
    return false;
}

void Negotiate::execute() {
    cout << "Executing Negotiate Order" << endl;
}

Orders* Negotiate::copy() {
    return new Negotiate(*this);
}

Player* Negotiate::getTargetPlayer() {
    return this->m_targetPlayer;
}

void Negotiate::setTargetPlayer(Player* targetPlayer) {
    targetPlayer->getPlayerName();
    this->m_targetPlayer = targetPlayer;
}

//toString output of the order descriptions

std::string Negotiate::toString() const {
    std::stringstream ss;
    ss<<"Negotiate: "<<endl;
    ss << "prevent attacks between the current player and the player targeted by the negotiate order until the\n"
          "end of the turn.";
    return ss.str();
}

OrderType Negotiate::getOrderType() {
    return OrderType::NEGOTIATE;
}


