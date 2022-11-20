#include <iostream>
#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include "../GameEngine/GameEngine.h"
#include <set>
#include <algorithm>
#include <vector>
#include <time.h>
#include <random>
#include "PlayerStrategy.h"

/**
 * Default constructor.
 */
Player::Player() {
    this->territories = vector<Territory*>();
    this->name = "";
    this->handCards = new Hand;
    this->ordersList = new OrdersList;
    this->isNegotiationWith = nullptr;
    this->ps = new HumanPlayerStrategy(this);
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
    this->ps = new HumanPlayerStrategy(this);
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
    this->ps = new HumanPlayerStrategy(this);
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
    if (ps != nullptr) {
//        delete ps;
        ps = nullptr;
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

//    vector<Territory*> attackTerritories;
//    // Get all adjacent territories ties that aren't owned by the player.
//    for (Territory* ter: getTerritories()) {
//        //check if the territory is adjacent
//        for (Territory* adjacent: ter->getAdjacentTerritories()) {
//            if (adjacent->getTerritoryOwner()->getPlayerName() != ter->getTerritoryOwner()->getPlayerName()) {
//                //if adjacent territory not already in list
//                if (!(find(attackTerritories.begin(), attackTerritories.end(), adjacent) != attackTerritories.end())) {
//                    //add to the list of territories that can be attacked
//                    attackTerritories.push_back(adjacent);
//                }
//            }
//        }
//    }
//    // Similar to toDefend, but the opposite, but sort based on the lowest, so it's easier to kill
//    sort(attackTerritories.begin(), attackTerritories.end(),
//         [](Territory* a, Territory* b) -> bool {
//             return a->getNumberOfArmies() < b->getNumberOfArmies();
//         });
//    return attackTerritories;
    this->ps->toAttack();
}

/**
 * Returns a vector of territories (pointer) the player defends.
 * @return vector territories
 */
vector<Territory*> Player::toDefend() {
//    // Try sorting them based on army count, return territories with the most units,
//    // they are your strongest territory and might win the game for the player
//    vector<Territory*> territoriesToDefend = getTerritories();
//    sort(begin(territoriesToDefend), end(territoriesToDefend),
//         [](Territory* a, Territory* b) -> bool {
//             return a->getNumberOfArmies() >
//                    b->getNumberOfArmies();
//         });
//    return territoriesToDefend;
    this->ps->toDefend();
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
bool Player::issueOrder(GameEngine* gameEngine) {
    this->ps->issueOrder(gameEngine);
//    //Keep issuing deploy orders until no more units in reinforcement pool
//
//    //We do not want to modify actual pool here, it's done during the deploy order execution
//    int reinforcementPoolCount = getReinforcementPool();
//
//    while (reinforcementPoolCount > 0) {
//        srand(time(NULL));
//        for (Territory* territory: this->toDefend()) {
//            if (reinforcementPoolCount == 0) {
//                goto noMoreReinforcementPool;
//            }
//            int countToDeploy = (rand() % reinforcementPoolCount) + 1;
//            if ((reinforcementPoolCount - countToDeploy) < 0) {
//                //the remaining
//                countToDeploy = reinforcementPoolCount;
//            }
//
//            cout << "Issuing Deploy Order for " << countToDeploy << " units to territory "
//                 << territory->getTerritoryName() << ".";
//            Orders* orders = new Deploy(this, countToDeploy, territory);
//            this->ordersList->addOrder(orders);
//            reinforcementPoolCount -= countToDeploy;
//            cout << " Remaining units in pool " << reinforcementPoolCount << "." << endl;
//        }
//    }
//    noMoreReinforcementPool:
//
//    cout << "Out of units to deploy." << endl;
//
//    //Advance
//    std::random_device rd1;
//    std::mt19937 gen1(rd1());
//    std::bernoulli_distribution toDefendRandom(0.9);
//
//    std::random_device rd2;
//    std::mt19937 gen2(rd1());
//    std::bernoulli_distribution toAttackRandom(0.9);
//
//    std::random_device rd3; // obtain a random number from hardware
//    std::mt19937 gen3(rd3()); // seed the generator
//    std::uniform_int_distribution<> distToDefend(1, this->toDefend().size()); // define the range
//    std::random_device rd4; // obtain a random number from hardware
//    std::mt19937 gen4(rd4()); // seed the generator
//    std::uniform_int_distribution<> distToAttack(1, this->toAttack().size()); // define the range
//    vector<Territory*> territoriesToDefend = toDefend();
//    vector<Territory*> territoriesToAttack = toAttack();
//    int defendCounter = 0;
//    srand(time(NULL));
//    int maxDefendCount = (rand() % 5) + 1;
//    for (Territory* territory: territoriesToDefend) {
//        if (toDefendRandom(gen1)) {
//            for (Territory* source: this->getTerritories()) {
//                if (source->isAdjacent(territory)) {
//                    if (source->getTerritoryName() != territory->getTerritoryName()) {
//                        int armyCountToMove = std::floor(source->getNumberOfArmies() / 2);
//
//                        Orders* advance_order = new Advance(this, armyCountToMove, source, territory,
//                                                            gameEngine->getDeck(),
//                                                            true);
//                        this->getOrdersList()->addOrder(advance_order);
//                        cout << "Issuing Advance order to move player army. " << defendCounter << endl;
//                        defendCounter++;
//                        if (defendCounter == maxDefendCount) {
//                            cout << "Reached the maximum " << maxDefendCount << " move advance orders." << endl;
//                            goto finishedMoveAdv;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    finishedMoveAdv:
//    int attackCounter = 0;
//    srand(time(NULL));
//    int maxAttackCount = (rand() % 5) + 5;
//    for (Territory* territory: territoriesToAttack) {
//        if (toAttackRandom(gen2)) {
//
//            // This is a territory that is adjacent to my own, but first we need to find which the adjacent one is
//            for (Territory* source: this->getTerritories()) {
//                if (source->isAdjacent(territory) &&
//                    source->getPlayerName() != territory->getPlayerName()) {
//                    if (source->getTerritoryName() != territory->getTerritoryName()) {
//                        int armyCountToMove = std::floor(source->getNumberOfArmies() / 2);
//                        Orders* advance_order = new Advance(this, armyCountToMove, source, territory,
//                                                            gameEngine->getDeck(),
//                                                            false);
//                        this->getOrdersList()->addOrder(advance_order);
//                        cout << "Issuing Advance order to attack target army. " << attackCounter << endl;
//                        attackCounter++;
//                        if (attackCounter == maxAttackCount) {
//                            cout << "Reached the maximum " << maxAttackCount << " attack advance orders." << endl;
//                            goto finishAdvAttack;
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    finishAdvAttack:
//    //Use one card every issue order phase if they have a card
//    if (!this->getHandCards()->getCards().empty()) {
//        Cards* cardsToPlays = this->getHandCards()->getCards().front();
//        if (cardsToPlays != nullptr) {
//            cout << "Issuing Card " << getNameByCardType(cardsToPlays->getType()) << endl;
//            Orders* orderToMake = nullptr;
//            switch (cardsToPlays->getType()) { // ignore missing for now
//                case BOMB:
//                    if (!toAttack().empty()) {
//                        Territory* target = toAttack().front();
//                        orderToMake = new Bomb(this, target);
//                    }
//                    break;
//                case BLOCKADE:
//                    if (!toDefend().empty()) {
//                        Territory* target = toDefend().front();
//                        orderToMake = new Blockade(this, gameEngine->getNeutralPlayer(), target);
//                    }
//                    break;
//                case AIRLIFT:
//                    if (!toDefend().empty()) {
//                        Territory* target = toDefend().back(); // target is last to defend
//                        Territory* source = toDefend().front(); // source is first to defend
//                        orderToMake = new Airlift(this, reinforcementPoolUnits, source, target);
//                    }
//                    break;
//                case DIPLOMACY:
//                    if (!toAttack().empty()) {
//                        Player* negotiatePlayer = toAttack().front()->getTerritoryOwner();
//                        orderToMake = new Negotiate(this, negotiatePlayer);
//                    }
//                    break;
//            }
//            if (orderToMake != nullptr) {
//                //this handles creating the order and removing it from players hand + back to deck
//                cardsToPlays->play(this, gameEngine->getDeck(), orderToMake);
//            }
//        }
//    }
//    return true;
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
    if (reinforcementPoolUnits_ < 0) {
        cout << endl;
    }
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

void Player::useOrders() {
    for (int i = 0; i < getOrdersList()->getOrdersList().size(); i++) {
        ordersList->getOrdersList().at(i)->execute();
    }
    getOrdersList()->clearOrders();
    cout << endl;
}

void Player::removeTerritory(Territory* pTerritory) {
    for (int i = 0; i < this->territories.size(); ++i) {
        if (pTerritory->getTerritoryName() == this->territories.at(i)->getTerritoryName()) {
            this->territories.erase(this->territories.begin() + i);
            break;
        }
    }
}

PlayerStrategy* Player::getPlayerStrategy() const {
    return this->ps;
}

void Player::setPlayerStrategy(PlayerStrategy* playerStrategy) {
    this->ps = playerStrategy;
}
