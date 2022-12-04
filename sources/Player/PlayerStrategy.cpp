#include "PlayerStrategy.h"
#include "Player.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"
#include "sources/Cards/Cards.h"

#include <cmath>
using std::floor;

#include <Vector>
#include <iostream>
#include "algorithm"
using namespace std;

//PlayerStrategy
PlayerStrategy::PlayerStrategy(Player* pPlayer) {
    this->player = pPlayer;
}

Player* PlayerStrategy::getPlayer() const {
    return player;
}

// HumanPlayerStrategy
HumanPlayerStrategy::HumanPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

vector<Territory*> HumanPlayerStrategy::toDefend() {
    return {};
}

vector<Territory*> HumanPlayerStrategy::toAttack() {
    return {};
}

bool HumanPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    return true;
}

//AggressivePlayerStrategy
/**
 * computer player that focuses on attack (deploys or advances armies on its strongest country,
 * then always advances to enemy territories until it cannot do so anymore).
 * @param pPlayer
 */
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {

}

/*
 * AggessivePlayerStrategy toDefend method
 */
vector<Territory*> AggressivePlayerStrategy::toDefend() {

    //list of territory to defend
    vector<Territory*> defendPriority;

    //strongest territory
    Territory* strongestTerritory = player->getTerritories().at(0);

    //loop trough each territory owned by all the player
    for(int i = 0; i < player->getTerritories().size(); i++){
        //if the strongest country own, defend it
        if(strongestTerritory->getNumberOfArmies() <= player->getTerritories().at(i)->getNumberOfArmies()){
             //add the strongest territory in the list
             strongestTerritory = player->getTerritories().at(i);
        }
    }

    //add the strongest territory in the list
    defendPriority.push_back(strongestTerritory);

    return defendPriority; //{defend order};
}

/*
 * AggessivePlayerStrtegy toDefend method
 */
vector<Territory*> AggressivePlayerStrategy::toAttack() {

    //list of territory to attack
    vector<Territory*> attackPriority;

    //loop trough each territory owned by all the player
    for(Territory* territory: player->getTerritories()){
        //if the strongest country, own, if strongest enemy territory attack it
        if(territory->getNumberOfArmies() > 0){
            //verify if there is a territory adjacent to it
            for(Territory* adjacent: territory->getAdjacentTerritories()){
                //check if the adjacent territory is also not owned
                if(adjacent->getTerritoryOwner() != player){
                    //check if territory not already in the list
                    if(!(find(attackPriority.begin(), attackPriority.end(), adjacent) != attackPriority.end())){
                        //add the adjacent territory in the list
                        attackPriority.push_back(adjacent);
                    }
                }
            }
        }
    }

    return attackPriority; //{attackOrder};
}

bool AggressivePlayerStrategy::issueOrder(GameEngine* gameEngine) {

    //list of territory todefend
    vector<Territory*> defendPriority = toDefend();

    //get the strongest territory first
    Territory* defend = defendPriority.at(0);

    //deploys armies on its strongest country
    Orders* deployorders = new Deploy(player, player->getReinforcementPool(), defend);
    deployorders->execute();
    cout<< deployorders->orderResult<<endl;
    delete deployorders;

    //get the territory to attack
    vector<Territory*> attack = toAttack();

    //make sure its always a different territory
    vector<Territory*> differentTerritory;

    //loop through each territory in the attack list
    for (Territory* territory: attack){
        //get all territory adjacent
        for(Territory* source: territory->getAdjacentTerritories()){
            //check if the territory is owned by the player
            if(source->getTerritoryOwner() == player){
                //check if territory not already in the list
                if(!(find(differentTerritory.begin(), differentTerritory.end(), source) != differentTerritory.end())){

                    //advance order now that all deploy are over
                    Orders* orders = new Advance(player,source->getNumberOfArmies(), source, territory, gameEngine->deck, true);
                    //add order in the list
                    player->getOrdersList()->addOrder(orders);
                    //push the order in the orderlist
                    ordersList.push_back(orders);
                    //cout<< deployorders<<endl;
                    //add the current territory in the list of territory already visited
                    differentTerritory.push_back(source);
                }
            }
            //push the territories in the gameengine as well
            gameEngine->territories.push_back(source);
        }
    }

    //check if there are territory to defend
    //if(defendPriority.size() == 0){
    //    return false;
    //}

    return true;
}

//BenevolentPlayerStrategy
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

vector<Territory*> BenevolentPlayerStrategy::toDefend() {

    vector<Territory*> territories_to_defend = player->getTerritories();

    sort(begin(territories_to_defend), end(territories_to_defend),
         [](Territory* a, Territory* b) -> bool {

             // since the benevolent player wants to defend the weakest territories, the territories are sorted
             // by the number of armies in them then the territories with the least number of armies are returned first
             return a->getNumberOfArmies() < b->getNumberOfArmies();
         });

    return territories_to_defend;
    //return {};
}

vector<Territory*> BenevolentPlayerStrategy::toAttack() {

    vector<Territory*> territories_to_attack;

    // the benevolent player never attacks therefore an empty vector of territories is returned
    return territories_to_attack;
}

bool BenevolentPlayerStrategy::issueOrder(GameEngine* gameEngine) {

    int count_to_advance = floor(toDefend().back()->getNumberOfArmies()/2);

    // here, I am getting the player's two territories with the lowest number of armies, i.e. the two weakest territories,
    // and deploying their entire/whole reinforcement pool to the first territory and advancing half of the number of armies
    // that the player's strongest territory (toDefend().back()) has to the second weakest territory on the vector of territories
    // returned from the toDefend() method. This is a design decision that could be different/changed depending on preference
    Territory* territory_for_deploy_order = toDefend()[0];
    Territory* territory_for_advance_order = toDefend()[1];

    cout << "Issuing Deploy Order for " << player->getReinforcementPool() << " units to territory "
         << territory_for_deploy_order->getTerritoryName() << ".";

    Orders* deploy_order = new Deploy(player, player->getReinforcementPool(), territory_for_deploy_order);
    player->getOrdersList()->addOrder(deploy_order);

    cout << "Issuing Advance Order for " << count_to_advance << " units from territory "
         << toDefend().back()->getTerritoryName() << " to territory "
         << territory_for_advance_order->getTerritoryName() << ".";

    Orders* advance_order = new Advance(player, count_to_advance, toDefend().back(), territory_for_advance_order, gameEngine->getDeck(), true);
    player->getOrdersList()->addOrder(advance_order);

    if (player->getHandCards()->getCards().empty()) {
        return false;
    }

    else {
        if (player->getHandCards()->getCards().front() != nullptr) {
            cout << "Issuing Card " << getNameByCardType(player->getHandCards()->getCards().front()->getType()) << endl;
            Orders* order_to_be_made = nullptr;

            switch (player->getHandCards()->getCards().front()->getType()) {
                case BOMB: case REINFORCEMENT: case BLOCKADE: case AIRLIFT: {
                    return false;
                }

                case DIPLOMACY: {
                    Player* negotiation_player = nullptr;

                    for (Player* another_player : gameEngine->getGamePlayers()) {
                        if (another_player != player) {
                            if (negotiation_player == nullptr || another_player->getTerritories().size() > negotiation_player->getTerritories().size()) {
                                negotiation_player = another_player;
                            }
                        }
                    }

                    order_to_be_made = new Negotiate(player, negotiation_player);
                    player->getOrdersList()->addOrder(order_to_be_made);
                }
                    break;
            }

            if (order_to_be_made != nullptr) {
                player->getHandCards()->getCards().front()->play(player, gameEngine->getDeck(), order_to_be_made);
                return true;
            }
        }
    }

    return true;
}

//NeutralPlayerStrategy
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {

}

vector<Territory*> NeutralPlayerStrategy::toDefend() {

    vector<Territory*> defendPriority;

    //return true if the neutral player is being attacked
    isattacked = true;

    return defendPriority; //{};
}

vector<Territory*> NeutralPlayerStrategy::toAttack() {

    vector<Territory*> attackPriority;

    return attackPriority; //{};
}

bool NeutralPlayerStrategy::issueOrder(GameEngine* gameEngine) {

    cout<<gameEngine->getPlayer->getPlayerName()<<endl;

    //PlayerStrategy* ps = new PlayerStrategy(this->player);
    AggressivePlayerStrategy* aps = new AggressivePlayerStrategy(gameEngine->getPlayer);

    //check if it is attack
    if(isattacked){
        //if so turn into an aggressive player
        player->setPlayerStrategy(aps);
        cout<<gameEngine->getPlayer->getPlayerName()<< " is now an aggressive player."<< endl;
    }

    return true;
}

//CheaterPlayerStrategy

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* pPlayer, const CheaterPlayerStrategy& cheater_player_strategy) : PlayerStrategy(pPlayer) {
    this->player = cheater_player_strategy.player;
}

vector<Territory*> CheaterPlayerStrategy::toDefend() {
    //return the territory
    return {};
}

vector<Territory*> CheaterPlayerStrategy::toAttack() {
    //cheaterTerritory vector to return
    vector<Territory*> territories_to_attack;
    //loop trough each territory owned by all the player
    for(Territory* territory: player->getTerritories()) {
        //loop through each adjacent territory
        for (Territory* adjacent: territory->getAdjacentTerritories()) {
            //check if the territory is not already owned by the cheater
            if (adjacent->getTerritoryOwner()->getPlayerName() != territory->getTerritoryOwner()->getPlayerName()){
                cout<<"Stealing from: " <<adjacent->getTerritoryOwner()->getPlayerName()<<endl;
                //if the adjacent territory isn't already in the list
                if (!(find(territories_to_attack.begin(), territories_to_attack.end(), adjacent) != territories_to_attack.end())){
                    territories_to_attack.push_back(adjacent);
                }
            }
        }
    }

    cout<<player->getPlayerName()<<" has "<<territories_to_attack.size()<<" territories. "<<endl;


    return territories_to_attack;
}

bool CheaterPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    vector<Territory*> territoriesAdjToConquer = toAttack();

    for (auto t : territoriesAdjToConquer ) {
        t->getPlayer()->removeTerritory(t);
        t->setTerritoryOwner(player);
        player->addTerritory(t);
    }

    return true;
}

CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& cheater_player_strategy) {
    if (this == &cheater_player_strategy) {
        return *this;
    }
    this->player = cheater_player_strategy.player;
    return *this;
}

ostream& operator<<(ostream& outs, const CheaterPlayerStrategy& cheater_player_strategy) {
    outs << "Name of the Player that uses the CheaterPlayerStrategy: "
         << cheater_player_strategy.player->getPlayerName() << endl;
    return outs;
}

