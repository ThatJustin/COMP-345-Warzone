#include "PlayerStrategy.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include "sources/Cards/Cards.h"
#include <algorithm>
#include <cmath>
#include "sources/Orders/Orders.h"
#include "sources/Cards/Cards.h"
#include "sources/GameEngine/GameEngine.h"
#include <set>
#include <algorithm>
#include <vector>
#include <time.h>
#include <random>

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
    // Try sorting them based on army count, return territories with the most units,
    vector<Territory*> territoriesToDefend = player->getTerritories();
    sort(begin(territoriesToDefend), end(territoriesToDefend),
         [](Territory* a, Territory* b) -> bool {
             return a->getNumberOfArmies() >
                    b->getNumberOfArmies();
         });
    return territoriesToDefend;
}

vector<Territory*> HumanPlayerStrategy::toAttack() {
    vector<Territory*> attackTerritories;
    // Get all adjacent territories ties that aren't owned by the player.
    for (Territory* ter: this->player->getTerritories()) {
        //check if the territory is adjacent
        for (Territory* adjacent: ter->getAdjacentTerritories()) {
            if (adjacent->getTerritoryOwner()->getPlayerName() != ter->getTerritoryOwner()->getPlayerName()) {
                //if adjacent territory not already in list
                if (!(find(attackTerritories.begin(), attackTerritories.end(), adjacent) != attackTerritories.end())) {
                    //add to the list of territories that can be attacked
                    attackTerritories.push_back(adjacent);
                }
            }
        }
    }
    // Similar to toDefend, but the opposite, but sort based on the lowest, so it's easier to kill
    sort(attackTerritories.begin(), attackTerritories.end(),
         [](Territory* a, Territory* b) -> bool {
             return a->getNumberOfArmies() < b->getNumberOfArmies();
         });
    return attackTerritories;
}

bool HumanPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    int reinforcementPoolCount = player->getReinforcementPool();
    while (reinforcementPoolCount > 0) {
        if (reinforcementPoolCount == 0) {
            goto noMoreReinforcementPool;
        }
        int countToDeploy;
        cout << "How many units would you like to deploy? Available units " << reinforcementPoolCount << endl;
        cin >> countToDeploy;

        if ((reinforcementPoolCount - countToDeploy) < 0) {
            //the remaining
            countToDeploy = reinforcementPoolCount;
        }
        cout << "Here is a list of your territories (format [Index] TerritoryName - Units stationed)" << endl;
        for (int i = 0; i < this->toDefend().size(); i++) {
            auto territory = toDefend()[i];
            if (territory != nullptr) {
                cout << "[" << i << "] " << territory->getTerritoryName() << " - " << territory->getNumberOfArmies()
                     << endl;
            }
        }
        cout << "Input which territory by Index you'd like to deploy " << countToDeploy << " units to." << endl;
        int territoryIndex;
        cin >> territoryIndex;
        auto territory = toDefend()[territoryIndex];
        cout << "Issuing Deploy Order for " << countToDeploy << " units to territory "
             << territory->getTerritoryName() << ".";
        Orders* orders = new Deploy(player, countToDeploy, territory);
        player->getOrdersList()->addOrder(orders);
        reinforcementPoolCount -= countToDeploy;
        cout << " Remaining units in pool " << reinforcementPoolCount << "." << endl;
    }
    noMoreReinforcementPool:
    cout << "Out of units to deploy." << endl;

//Advance moves

    //Use one card every issue order phase if they have a card
    if (!player->getHandCards()->getCards().empty()) {
        cout << "Available cards to play :" << endl;
        for (int i = 0; i < player->getHandCards()->getCards().size(); i++) {
            auto card = player->getHandCards()->getCards()[i];
            if (card != nullptr) {
                cout << "[" << i << "] " << getNameByCardType(card->getType()) << endl;
            }
        }
        pickNewCard:
        int cardIndex;
        cout << "Input which card by Index you'd like to play." << endl;
        cin >> cardIndex;
        if (cardIndex >= 0 && cardIndex <= player->getHandCards()->getCards().size()) {
            Cards* cardsToPlays = player->getHandCards()->getCards().at(cardIndex);
            cout << "Issuing Card " << getNameByCardType(cardsToPlays->getType()) << endl;
            Orders* orderToMake = nullptr;
            switch (cardsToPlays->getType()) { // ignore missing for now
                case BOMB:
                    if (!toAttack().empty()) {
                        Territory* target = toAttack().front();
                        orderToMake = new Bomb(player, target);
                    }
                    break;
                case BLOCKADE:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().front();
                        orderToMake = new Blockade(player, gameEngine->getNeutralPlayer(), target);
                    }
                    break;
                case AIRLIFT:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().back(); // target is last to defend
                        Territory* source = toDefend().front(); // source is first to defend
                        orderToMake = new Airlift(player, player->getReinforcementPool(), source, target);
                    }
                    break;
                case DIPLOMACY:
                    if (!toAttack().empty()) {
                        Player* negotiatePlayer = toAttack().front()->getTerritoryOwner();
                        orderToMake = new Negotiate(player, negotiatePlayer);
                    }
                    break;
            }
            if (orderToMake != nullptr) {
                //this handles creating the order and removing it from players hand + back to deck
                cardsToPlays->play(player, gameEngine->getDeck(), orderToMake);
            }
        } else {
            cout << "This card doesn't exist, please try again." << endl;
            goto pickNewCard;
        }
    }
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

DefaultPlayerStrategy::DefaultPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {

}

vector<Territory*> DefaultPlayerStrategy::toDefend() {
    // Try sorting them based on army count, return territories with the most units,
    vector<Territory*> territoriesToDefend = player->getTerritories();
    sort(begin(territoriesToDefend), end(territoriesToDefend),
         [](Territory* a, Territory* b) -> bool {
             return a->getNumberOfArmies() >
                    b->getNumberOfArmies();
         });
    return territoriesToDefend;
}

vector<Territory*> DefaultPlayerStrategy::toAttack() {
    vector<Territory*> attackTerritories;
    // Get all adjacent territories ties that aren't owned by the player.
    for (Territory* ter: this->player->getTerritories()) {
        //check if the territory is adjacent
        for (Territory* adjacent: ter->getAdjacentTerritories()) {
            if (adjacent->getTerritoryOwner()->getPlayerName() != ter->getTerritoryOwner()->getPlayerName()) {
                //if adjacent territory not already in list
                if (!(find(attackTerritories.begin(), attackTerritories.end(), adjacent) != attackTerritories.end())) {
                    //add to the list of territories that can be attacked
                    attackTerritories.push_back(adjacent);
                }
            }
        }
    }
    // Similar to toDefend, but the opposite, but sort based on the lowest, so it's easier to kill
    sort(attackTerritories.begin(), attackTerritories.end(),
         [](Territory* a, Territory* b) -> bool {
             return a->getNumberOfArmies() < b->getNumberOfArmies();
         });
    return attackTerritories;
}

bool DefaultPlayerStrategy::issueOrder(GameEngine* gameEngine) {
    //Keep issuing deploy orders until no more units in reinforcement pool

    //We do not want to modify actual pool here, it's done during the deploy order execution
    int reinforcementPoolCount = player->getReinforcementPool();
    while (reinforcementPoolCount > 0) {
        srand(time(NULL));
        for (Territory* territory: this->toDefend()) {
            if (reinforcementPoolCount == 0) {
                goto noMoreReinforcementPool;
            }
            int countToDeploy = (rand() % reinforcementPoolCount) + 1;
            if ((reinforcementPoolCount - countToDeploy) < 0) {
                //the remaining
                countToDeploy = reinforcementPoolCount;
            }

            cout << "Issuing Deploy Order for " << countToDeploy << " units to territory "
                 << territory->getTerritoryName() << ".";
            Orders* orders = new Deploy(player, countToDeploy, territory);
            player->getOrdersList()->addOrder(orders);
            reinforcementPoolCount -= countToDeploy;
            cout << " Remaining units in pool " << reinforcementPoolCount << "." << endl;
        }
    }
    noMoreReinforcementPool:

    cout << "Out of units to deploy." << endl;

    //Advance
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::bernoulli_distribution toDefendRandom(0.9);

    std::random_device rd2;
    std::mt19937 gen2(rd1());
    std::bernoulli_distribution toAttackRandom(0.9);

    std::random_device rd3; // obtain a random number from hardware
    std::mt19937 gen3(rd3()); // seed the generator
    std::uniform_int_distribution<> distToDefend(1, this->toDefend().size()); // define the range
    std::random_device rd4; // obtain a random number from hardware
    std::mt19937 gen4(rd4()); // seed the generator
    std::uniform_int_distribution<> distToAttack(1, this->toAttack().size()); // define the range
    vector<Territory*> territoriesToDefend = toDefend();
    vector<Territory*> territoriesToAttack = toAttack();
    int defendCounter = 0;
    srand(time(NULL));
    int maxDefendCount = (rand() % 5) + 1;
    for (Territory* territory: territoriesToDefend) {
        if (toDefendRandom(gen1)) {
            for (Territory* source: player->getTerritories()) {
                if (source->isAdjacent(territory)) {
                    if (source->getTerritoryName() != territory->getTerritoryName()) {
                        int armyCountToMove = std::floor(source->getNumberOfArmies() / 2);

                        Orders* advance_order = new Advance(player, armyCountToMove, source, territory,
                                                            gameEngine->getDeck(),
                                                            true);
                        player->getOrdersList()->addOrder(advance_order);
                        cout << "Issuing Advance order to move player army. " << defendCounter << endl;
                        defendCounter++;
                        if (defendCounter == maxDefendCount) {
                            cout << "Reached the maximum " << maxDefendCount << " move advance orders." << endl;
                            goto finishedMoveAdv;
                        }
                    }
                }
            }
        }
    }
    finishedMoveAdv:
    int attackCounter = 0;
    srand(time(NULL));
    int maxAttackCount = (rand() % 5) + 5;
    for (Territory* territory: territoriesToAttack) {
        if (toAttackRandom(gen2)) {

            // This is a territory that is adjacent to my own, but first we need to find which the adjacent one is
            for (Territory* source: player->getTerritories()) {
                if (source->isAdjacent(territory) &&
                    source->getPlayerName() != territory->getPlayerName()) {
                    if (source->getTerritoryName() != territory->getTerritoryName()) {
                        int armyCountToMove = std::floor(source->getNumberOfArmies() / 2);
                        Orders* advance_order = new Advance(player, armyCountToMove, source, territory,
                                                            gameEngine->getDeck(),
                                                            false);
                        player->getOrdersList()->addOrder(advance_order);
                        cout << "Issuing Advance order to attack target army. " << attackCounter << endl;
                        attackCounter++;
                        if (attackCounter == maxAttackCount) {
                            cout << "Reached the maximum " << maxAttackCount << " attack advance orders." << endl;
                            goto finishAdvAttack;
                        }
                    }
                }
            }
        }
    }

    finishAdvAttack:
    //Use one card every issue order phase if they have a card
    if (!player->getHandCards()->getCards().empty()) {
        Cards* cardsToPlays = player->getHandCards()->getCards().front();
        if (cardsToPlays != nullptr) {
            cout << "Issuing Card " << getNameByCardType(cardsToPlays->getType()) << endl;
            Orders* orderToMake = nullptr;
            switch (cardsToPlays->getType()) { // ignore missing for now
                case BOMB:
                    if (!toAttack().empty()) {
                        Territory* target = toAttack().front();
                        orderToMake = new Bomb(player, target);
                    }
                    break;
                case BLOCKADE:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().front();
                        orderToMake = new Blockade(player, gameEngine->getNeutralPlayer(), target);
                    }
                    break;
                case AIRLIFT:
                    if (!toDefend().empty()) {
                        Territory* target = toDefend().back(); // target is last to defend
                        Territory* source = toDefend().front(); // source is first to defend
                        orderToMake = new Airlift(player, player->getReinforcementPool(), source, target);
                    }
                    break;
                case DIPLOMACY:
                    if (!toAttack().empty()) {
                        Player* negotiatePlayer = toAttack().front()->getTerritoryOwner();
                        orderToMake = new Negotiate(player, negotiatePlayer);
                    }
                    break;
            }
            if (orderToMake != nullptr) {
                //this handles creating the order and removing it from players hand + back to deck
                cardsToPlays->play(player, gameEngine->getDeck(), orderToMake);
            }
        }
    }
    return true;
}
