#include <iostream>
#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <set>
#include <algorithm>
#include <vector>
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
 * Returns a vector of territories (pointer) the player will attack.
 * @return vector territories
 */
vector<Territory*> Player::toAttack() {

    //create list of attack territories
    vector<Territory*> attackTerritories;

    //for all territory
    for(Territory* terrritory: territories){

        //check if the territory is adjacent
        for(Territory* adjacent: terrritory->getAdjacentTerritories()){

            //if does not belong to the same player
            if(adjacent->getTerritoryOwner() != this){

                //if adjacent territory not already in list
                if(!(find(attackTerritories.begin(),attackTerritories.end(), adjacent) != attackTerritories.end())){

                    //add to the list of territories that can be attacked
                    attackTerritories.push_back(adjacent);
                }
            }
        }
    }

    /*
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
     */

    return attackTerritories;
}

/**
 * Returns a vector of territories (pointer) the player defends.
 * @return vector territories
 */
vector<Territory*> Player::toDefend() {

    vector<Territory*> defendTerritories;

    //loop through every territory own by the player
    for(int i = 0; i < getTerritories().size(); i++){

        //add the territory to the list of territory to be defended
        defendTerritories.push_back(getTerritories().at(i));
    }
    /*
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

     */
    return defendTerritories;
}

//Territories from which the army come from
Territory* Player::sourceTerritory(Map* map){

    Territory* source = NULL;

    for (Territory* territory : map->getTerritories()) {
        source = territory;
        break;
    }

    return source;
}

//Territories to send army to
Territory* Player::targetTerritory(Map* map){

    Territory* target = NULL;

    for (Territory* territory : map->getTerritories()) {
        target = territory;
        break;
    }

    return target;
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
bool Player::issueOrder(Map *map, vector<Player*> player,Deck *deck, Hand* hand) { //need to add commandprocessor as a parameter here

    //while there is still army to the player, deploy them to conquer other territories
    while (getArmy() > 0){

        //show the player the amount of territory they own
        cout<< "You own the territory: "<<endl;

        //list all the territory own by the player as well as the army in each
        for(Territory* territory: toDefend()){

            cout<< territory->getTerritoryName() << " and you have " << territory->getNumberOfArmies()<< " soldier, in that territory" <<endl;
        }

        //create territory to be attack by the player
        Territory* toDefendTerritory = NULL;

        //loop through all the territory in the game
        for(Territory* territory: territories){
            //set the territory as the target territory to be deployed at
            //if(territory->getTerritoryName() == player.territoryname()) {//verify if the terrritory the player want to defend is in thelist
                toDefendTerritory = territory;
                break;
           //}
        }

        //check if the territory is invalid
        if(toDefendTerritory == NULL){

            cout<< "Invalid Territory: "<<endl;
        }

        //army available for specific player
        int deployArmy = getArmy();

        //it will issue a deploy order and no other order
        Orders* orders = new Deploy(deployArmy, toDefendTerritory,this);
        orders->execute();
        delete orders;
    }

    //Once it has deployed all its available army units, it can proceed with other kinds of orders.
    bool check = true;
    while (check){

        int amountofcards;

        //The player uses one of the cards in their hand to issue an order that corresponds to the card in question.
        for(Cards* cards: handCards->getCards()){

            cout<<"Player has: " <<endl;
            cout<<to_string(++amountofcards)<< " and " << cards->getType()<<endl;
        }

        if(!hand->getCards().empty()) {
            //issue all remainding cards
            Cards *cards = handCards->getCards().at(amountofcards);
            Orders *orders = NULL;

            //issue airlift type of order cards
            if (getNameByCardType(cards->getType()) == getNameByOrderType(OrderType::AIRLIFT)) {

                Territory* source = sourceTerritory(map);

                Territory* target = targetTerritory(map);

                //army, source, target, player
                orders = new Airlift(army,source,target, this); //need player parameter
            } else if (getNameByCardType(cards->getType()) == getNameByOrderType(OrderType::BOMB)) {

                Territory* target = targetTerritory(map);

                orders = new Bomb(target, this);

            } else if (getNameByCardType(cards->getType()) == getNameByOrderType(OrderType::BLOCKADE)) {

                Territory* target = targetTerritory(map);

                orders = new Blockade(target, this);

            } else if (getNameByCardType(cards->getType()) == getNameByOrderType(OrderType::NEGOTIATE)) {

                Player* targetPlayer = NULL;

                for (Player* player :players) {
                    targetPlayer = player;
                    break;
                }
                orders = new Negotiate(targetPlayer, this);
            }

            //remove the cards from the hands and place it back into the deck
            handCards->remove(cards);
            deck->addCard(cards);
        }
        else{
            //The player issues advance orders to either (1) move army units from one of its own territory to another of its own territories
            //in order to defend it (using toDefend() to make the decision)
            //and/or (2) move army units from one of its own territories to a neighboring enemy territory to attack them (using toAttack() to make the decision).
            Territory* targetArea = NULL;

            for(Territory* territory: territories){

                cout<< territory->getTerritoryName() << " and you have " << territory->getNumberOfArmies()<< " soldier, in that territory" <<endl;
            }

            //the source territory
            Territory* sourceArea = sourceTerritory(map);

            //decide between defend or attack, return a list of targets
            vector <Territory*> targets;

            //call todefend if the territory has no army defending it
            if(targetArea->getNumberOfArmies() == 0){

                targets = toDefend();
            }

            //call toattack if the all its territory has army
            else{

                targets = toAttack();
            }

            //show the territories at which the player can move
            cout << "You can advance in the territories: "<< endl;
            for(Territory* territory: targets){

                cout<<territory->getTerritoryName() << " and "<< territory->getNumberOfArmies() <<endl;

                targetArea = territory;
                break;
            }

            Orders* order = new Advance(army, sourceArea, targetArea, this);
            ordersList->add(order);

            //break out of the loop once the player turn is over
            //if(player.turnover()){//need to check when the player turn is over

                cout<<"Player turn over"<<endl;
                check = false;
            //}
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

