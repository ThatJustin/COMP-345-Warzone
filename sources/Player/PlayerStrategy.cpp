#include "PlayerStrategy.h"
#include "Player.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"

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

// AggressivePlayerStrategy
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

vector<Territory*> AggressivePlayerStrategy::toDefend() {
    return {};
}

vector<Territory*> AggressivePlayerStrategy::toAttack() {
    return {};
}

bool AggressivePlayerStrategy::issueOrder(GameEngine* gameEngine) {
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
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

vector<Territory*> NeutralPlayerStrategy::toDefend() {
    return {};
}

vector<Territory*> NeutralPlayerStrategy::toAttack() {
    return {};
}

bool NeutralPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    return {};
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

    //cheaterTerritory vector to return
    vector<Territory*> cheaterTerritory;

    //loop trough each territory owned by all the player
    for(Territory* territory: player->getTerritories()) {
        //loop through each adjacent territory
        for (Territory *adjacent: territory->getAdjacentTerritories()) {
            //check if the territory is not already owned by the cheater
            if (adjacent->getTerritoryOwner()->getPlayerName() != territory->getTerritoryOwner()->getPlayerName()){
                //if the adjacent territory isn't already in the list
                if (!(find(cheaterTerritory.begin(), cheaterTerritory.end(), adjacent) != cheaterTerritory.end())){
                    cheaterTerritory.push_back(adjacent);
                }
            }
        }
    }

    sort(toAttack().begin(), toAttack().end(),[](Territory* a, Territory* b) -> bool {
        return a->getNumberOfArmies() < b->getNumberOfArmies();
    });

    gameEngine->territories = cheaterTerritory;

    return true;
}
