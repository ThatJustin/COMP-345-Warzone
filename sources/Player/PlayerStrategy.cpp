#include "PlayerStrategy.h"
#include "Player.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"

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

        //not sure what to add here
        this->player = pPlayer;
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

    /*need to use gameEngine for the implementation?*/
    //GameEngine* gameEngine = new GameEngine();

    //list of territory todefend
    vector<Territory*> defendPriority = toDefend();

    //get the strongest territory first
    Territory* defend = defendPriority.at(0);

    //deploys armies on its strongest country
    Orders* deployorders = new Deploy(player,player->getReinforcementPool(), defend);
    deployorders->execute();
    cout<< deployorders<<endl;
    delete deployorders;

    //get the territory to attack
    vector<Territory*>  attack = toAttack();

    //make sure its always a different territory
    vector<Territory*>  differentTerritory;

    //dummy definition to be able to use the Advance order, might change later
    Deck* deck;

    //loop through each territory in the attack list
    for (Territory* territory: attack){
        //get all territory adjacent
        for(Territory* source: territory->getAdjacentTerritories()){
            //check if the territory is owned by the player
            if(source->getTerritoryOwner() == player){
                //check if territory not already in the list
                if(!(find(differentTerritory.begin(), differentTerritory.end(), source) != differentTerritory.end())){

                    //advance order now that all deploy are over
                    Orders* orders = new Advance(player,source->getNumberOfArmies(), source, territory, deck, true);
                    //add order in the list
                    player->getOrdersList()->addOrder(orders);
                    //push the order in the orderlist
                    ordersList.push_back(orders);
                    cout<< deployorders<<endl;
                    //add the current territory in the list of territory already visited
                    differentTerritory.push_back(source);
                }
            }
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
    return {};
}

vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    return {};
}

bool BenevolentPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    return true;
}

//NeutralPlayerStrategy
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {

    //PlayerStrategy* ps = new PlayerStrategy(this->player);
    AggressivePlayerStrategy* aps = new AggressivePlayerStrategy(pPlayer);

    //check if it is attack
    if(isattacked){
        //if so turn into an aggressive player
        //PlayerStrategy(pPlayer->setPlayerStrategy(aps->player->getPlayerStrategy());
    }
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
    return true;
}

//CheaterPlayerStrategy
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

vector<Territory*> CheaterPlayerStrategy::toDefend() {
    return {};
}

vector<Territory*> CheaterPlayerStrategy::toAttack() {
    return {};
}

bool CheaterPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    return false;
}
