#include <iostream>
#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <set>
#include <algorithm>
#include <vector>
#include <time.h>
#include <random>

/**
 * Default constructor.
 */
Player::Player() {
    this->territories = vector<Territory*>();
    this->name = "";
    this->handCards = new Hand;
    this->ordersList = new OrdersList;
    this->isNegotiationWith = nullptr;
}

/**
 * Parameterized Constructor.
 * @param name name of the player
 */
Player::Player(const string& name) {
    this->territories = vector<Territory*>();
    this->name = name;
    this->handCards = new Hand;
    this->ordersList = new OrdersList;
    this->isNegotiationWith = nullptr;
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
    this->isNegotiationWith = nullptr;
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
 * Returns a vector of territories (pointer) the player will attack.
 * @return vector territories
 */
vector<Territory*> Player::toAttack() {

    //create list of attack territories
    vector<Territory*> attackTerritories;

    //for all territory
    for (Territory* terrritory: getTerritories()) {

        //check if the territory is adjacent
        for (Territory* adjacent: terrritory->getAdjacentTerritories()) {
            cout << "[debug] adjacent owner " << adjacent->getTerritoryOwner()->getPlayerName() << endl;
            //if does not belong to the same player
            if (adjacent->getTerritoryOwner()->getPlayerName() != this->getPlayerName() &&
                !adjacent->getTerritoryOwner()->checkIsNegotiation(this)) {

                //if adjacent territory not already in list
                if (find(attackTerritories.begin(), attackTerritories.end(), adjacent) == attackTerritories.end()) {

                    //add to the list of territories that can be attacked
                    attackTerritories.push_back(adjacent);
                }
            }
        }
    }

    return attackTerritories;
}

/**
 * Returns a vector of territories (pointer) the player defends.
 * @return vector territories
 */
vector<Territory*> Player::toDefend() {
    // defend their own territories
    //Maybe sort them on some sort of logic where it's orders from must defend to least needing to defend
    return this->getTerritories();
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
bool Player::issueOrder(Map* map, Player* neutral, vector<Player*> players, Deck* deck, Hand* hand) {
    //Keep issuing deploy orders until no more units in reinforcement pool
    srand(time(NULL));
    while (getReinforcementPool() > 0) {
        for (Territory* territory: this->toDefend()) {
            if (getReinforcementPool() == 0) {
                break;
            }
            int countToDeploy = (rand() % getReinforcementPool()) + 1;
            if ((getReinforcementPool() - countToDeploy) < 0) {
                //the remaining
                countToDeploy = getReinforcementPool();
            }

            cout << "Issuing Deploy Order for " << countToDeploy << " units to territory "
                 << territory->getTerritoryName() << ".";
            Orders* orders = new Deploy(this, countToDeploy, territory);
            this->ordersList->addOrder(orders);
            this->setReinforcementPool(this->getReinforcementPool() - countToDeploy);
            cout << " Remaining units in pool " << this->getReinforcementPool() << "." << endl;
        }
    }

    cout << "Out of units to deploy." << endl;

    //Advance
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::bernoulli_distribution toDefendRandom(0.6);

    std::random_device rd2;
    std::mt19937 gen2(rd1());
    std::bernoulli_distribution toAttackRandom(0.4);

    std::random_device rd3; // obtain a random number from hardware
    std::mt19937 gen3(rd3()); // seed the generator
    std::uniform_int_distribution<> distToDefend(1, this->toDefend().size()); // define the range
    std::random_device rd4; // obtain a random number from hardware
    std::mt19937 gen4(rd4()); // seed the generator
    std::uniform_int_distribution<> distToAttack(1, this->toAttack().size()); // define the range

    for (Territory* territory: this->toDefend()) {
        if (toDefendRandom(gen1)) {
            Territory* source = toDefend().at(distToDefend(gen3) - 1); // source is first to defend
            if (source->getMapTerritoryId() != territory->getMapTerritoryId()) {
                Orders* advance_order = new Advance(this, reinforcementPoolUnits, source, territory, deck);
                this->getOrdersList()->addOrder(advance_order);
            }
        }
    }
    for (Territory* territory: this->toAttack()) {
        if (toAttackRandom(gen2)) {
            Territory* source = toAttack().at(distToAttack(gen4) - 1); // source is first to defend
            if (source->getMapTerritoryId() != territory->getMapTerritoryId()) {
                Orders* advance_order = new Advance(this, reinforcementPoolUnits, source, territory, deck);
                this->getOrdersList()->addOrder(advance_order);
            }
        }
    }

    //Use one card every issue order phase if they have a card
    if (!this->getHandCards()->getCards().empty()) {
        Cards* cardsToPlays = this->getHandCards()->getCards().front();
        if (cardsToPlays != nullptr) {
            cout << "Issuing Card Order" << endl;
            Orders* orderToMake = nullptr;
            switch (cardsToPlays->getType()) { // ignore missing for now
                case BOMB:
                    if (!toAttack().empty()) {
                        Territory* target = toAttack().front();
                        orderToMake = new Bomb(this, target);
                    }
                    break;
                case BLOCKADE:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().front();
                        orderToMake = new Blockade(this, neutral, target);
                    }
                    break;
                case AIRLIFT:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().back(); // target is last to defend
                        Territory* source = toDefend().front(); // source is first to defend
                        orderToMake = new Airlift(this, reinforcementPoolUnits, source, target);
                    }
                    break;
                case DIPLOMACY:
                    if (!toAttack().empty()) {
                        Player* negotiatePlayer = toAttack().front()->getTerritoryOwner();
                        orderToMake = new Negotiate(this, negotiatePlayer);
                    }
                    break;
            }
            if (orderToMake != nullptr) {
                //this handles creating the order and removing it from players hand + back to deck
                cardsToPlays->play(this, deck, orderToMake);
            }
        }
    }
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
    return stream << endl;
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
 * Sets the initial army units for the player.
 * @param reinforcementPoolUnits
 */
void Player::setReinforcementPool(int reinforcementPoolUnits_) {
    this->reinforcementPoolUnits = reinforcementPoolUnits_;
}

int Player::getReinforcementPool() const {
    return this->reinforcementPoolUnits;
}

void Player::setNegotiationWith(Player* player) {
    this->isNegotiationWith = player;
}

bool Player::checkIsNegotiation(Player* player) {
//    cout << (this->isNegotiationWith == player) << endl;
    return this->isNegotiationWith == player;
}

void Player::removeOrders() {
    while (this->ordersList->getOrdersList().size() > 0) {
        ordersList->getOrdersList().pop_back();
    }
}

void Player::useOrders() {
    for (int i = 0; i < getOrdersList()->getOrdersList().size(); i++) {
        ordersList->getOrdersList().at(i)->execute();
    }
    ordersList->getOrdersList().clear();
    this->ordersList->getOrdersList() = vector<Orders*>();
//    while(!ordersList->getOrdersList().empty()) {
//        ordersList->getOrdersList().pop_back();
//    }
//    for (Orders* orders : player->getOrdersList()->getOrdersList()) {
//        //This assumes the first order in the orderlist is a Deploy order which it MUST BE
//        orders->execute();
//        //remove it after
////            player->getOrdersList()->remove(i);
//    }
}

void Player::removeTerritory(Territory* pTerritory) {
    for (int i = 0; i < this->territories.size(); ++i) {
        if (pTerritory->getTerritoryName() == this->territories.at(i)->getTerritoryName()) {
            this->territories.erase(this->territories.begin() + i);
            break;
        }
    }
}
