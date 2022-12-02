#include "PlayerStrategy.h"
#include "Player.h"
#include "sources/Map/Map.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Cards/Cards.h"

#include <cmath>

using std::floor;

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
/**
 * Parameterized constructor for the BenevolentPlayerStrategy class
 * @param pPlayer the player that the strategy is for
 */
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

/**
 * This method returns the territories that the player needs to defend, prioritizing the territories with the least number of armies, i.e. the weakest territories
 * @return A vector of territory pointers that point to territories the benevolent player wants to defend
 */
vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    vector<Territory*> territories_to_defend = player->getTerritories();

    sort(begin(territories_to_defend), end(territories_to_defend),
         [](Territory* a, Territory* b) -> bool {

        // since the benevolent player wants to defend the weakest territories, the territories are sorted
        // by the number of armies in them then the territories with the least number of armies are returned first
        return a->getNumberOfArmies() < b->getNumberOfArmies();
    });

    return territories_to_defend;
}

/**
 * This method returns the territories that the benevolent player wants to attack
 * @return A vector of territory pointers that point to territories the benevolent player wants to attack
 */
vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    vector<Territory*> territories_to_attack;

    // the benevolent player never attacks therefore an empty vector of territories is returned
    return territories_to_attack;
}

/**
 * This method issues an order for the benevolent player
 * @param gameEngine
 * @return true if the order was issued successfully if the player has any card to play in their hand, false otherwise
 */
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
