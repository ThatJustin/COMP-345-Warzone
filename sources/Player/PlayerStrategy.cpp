#include "PlayerStrategy.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include "sources/Cards/Cards.h"
#include <algorithm>
#include <cmath>
#include "sources/GameEngine/GameEngine.h"
#include <set>
#include <vector>
#include <time.h>
#include <random>
#include <bits/stdc++.h>

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
    cout << "Issuing orders for HumanPlayerStrategy." << endl;
    int reinforcementPoolCount = player->getReinforcementPool();
    while (reinforcementPoolCount > 0) {
        if (player->getTerritories().empty()) {
            break;
        }
        int countToDeploy;
        cout << "How many units would you like to deploy? Available units " << reinforcementPoolCount << endl;
        cin >> countToDeploy;

        if ((reinforcementPoolCount - countToDeploy) < 0) {
            //the remaining
            countToDeploy = reinforcementPoolCount;
        }
        cout << "Here is a list of your territories (format [Index] TerritoryName - Units stationed)" << endl;
        for (int i = 0; i < HumanPlayerStrategy::toDefend().size(); i++) {
            auto territory = HumanPlayerStrategy::toDefend()[i];
            if (territory != nullptr) {
                cout << "[" << i << "] " << territory->getTerritoryName() << " - " << territory->getNumberOfArmies()
                     << endl;
            }
        }
        cout << "Input which territory by Index you'd like to deploy " << countToDeploy << " units to." << endl;
        int territoryIndex;
        cin >> territoryIndex;
        auto territory = HumanPlayerStrategy::toDefend()[territoryIndex];
        cout << "Issuing Deploy Order for " << countToDeploy << " units to territory "
             << territory->getTerritoryName() << ".";
        Orders* orders = new Deploy(player, countToDeploy, territory);
        player->getOrdersList()->addOrder(orders);
        reinforcementPoolCount -= countToDeploy;
        cout << " Remaining units in pool " << reinforcementPoolCount << "." << endl;
    }
    cout << "Out of units to deploy." << endl;

    //Advance moves
    if (!HumanPlayerStrategy::toDefend().empty()) {
        continueAdvMoveOrder:
        cout << "Would you like to issue Advance Move orders?" << endl;
        issueMoveOrder:
        cout << "Enter 1 for yes and 0 for no." << endl;
        int isIssuing;
        cin >> isIssuing;
        if (isIssuing == 0) {
            cout << "Skipping issuing advance move orders." << endl;
            goto finishedMoveAdv;
        } else if (isIssuing == 1) {
            pickSourceAgain:
            cout
                    << "Here is a list of your territories (format [Index] TerritoryName - Units stationed) that you can issue advance move orders to.\r\nEnter -1 to stop issuing advance move orders."
                    << endl;
            int sourceTargetArmyCountZero;
            for (int i = 0; i < HumanPlayerStrategy::toDefend().size(); i++) {
                auto territory = HumanPlayerStrategy::toDefend()[i];
                if (territory->getNumberOfArmies() != 0) {
                    cout << "[" << i << "] " << territory->getTerritoryName() << " - "
                         << territory->getNumberOfArmies()
                         << endl;
                    sourceTargetArmyCountZero++;
                }
            }
            if (sourceTargetArmyCountZero == 0) {
                cout << "Skipping issuing advance move orders. Your territories have no available units to move."
                     << endl;
                goto finishedMoveAdv;
            }
            int sourceTerritoryIndex;
            cin >> sourceTerritoryIndex;
            if (sourceTerritoryIndex == -1) {
                goto finishedMoveAdv;
            } else {
                int sourceTerritoryUnitMoveCount;
                cout << "How many units would you like to move from this territory? (Available: "
                     << HumanPlayerStrategy::toDefend()[sourceTerritoryIndex]->getNumberOfArmies() << ")" << endl;
                cin >> sourceTerritoryUnitMoveCount;
                cout
                        << "Here is a list of your territories (format [Index] TerritoryName - Units stationed) that you can move units to.\r\nEnter -1 to stop issuing advance move orders."
                        << endl;
                int adjCount = 0;
                for (int i = 0; i < HumanPlayerStrategy::toDefend().size(); i++) {
                    auto sourceTerritory = HumanPlayerStrategy::toDefend()[i];
                    if (sourceTerritory->getTerritoryName() !=
                        HumanPlayerStrategy::toDefend()[sourceTerritoryIndex]->getTerritoryName() &&
                        sourceTerritory->isAdjacent(HumanPlayerStrategy::toDefend()[sourceTerritoryIndex])) {
                        cout << "[" << i << "] " << sourceTerritory->getTerritoryName() << " - "
                             << sourceTerritory->getNumberOfArmies()
                             << endl;
                        adjCount++;
                    }
                }
                int targetTerritoryIndex;
                cin >> targetTerritoryIndex;
                if (adjCount == 0) {
                    cout << "You do not own any adjacent territories to this one. Pick another..." << endl;
                    goto pickSourceAgain;
                }
                if (sourceTerritoryIndex == -1) {
                    goto finishedMoveAdv;
                } else {
                    auto source = HumanPlayerStrategy::toDefend()[sourceTerritoryIndex];
                    auto target = HumanPlayerStrategy::toDefend()[targetTerritoryIndex];
                    Orders* advance_order = new Advance(player, sourceTerritoryUnitMoveCount,
                                                        source,
                                                        target,
                                                        gameEngine->getDeck(),
                                                        true);
                    player->getOrdersList()->addOrder(advance_order);
                    cout << "Issuing Advance order to move player army from " << source->getTerritoryName() << " to "
                         << target->getTerritoryName() << " for " << sourceTerritoryUnitMoveCount << " units." << endl;
                    goto continueAdvMoveOrder;
                }
            }
        } else {
            cout << "Invalid option. Try again" << endl;
            goto issueMoveOrder;
        }
    }
    finishedMoveAdv:
    if (!HumanPlayerStrategy::toAttack().empty()) {
        continueAdvAttackOrder:
        cout << "Would you like to issue Advance Attack orders?" << endl;
        issueAttackOrder:
        cout << "Enter 1 for yes and 0 for no." << endl;
        int isIssuing;
        cin >> isIssuing;
        if (isIssuing == 0) {
            cout << "Skipping issuing advance attack orders." << endl;
            goto finishedAttackAdv;
        } else if (isIssuing == 1) {
            // Map holds attacker (attacker index, vector of territories it can be attacked by using their index)
            std::map<int, std::vector<int>> territoriesToAttack;
            //Fetch all territories the player can attack (they must be adj to the players' territory )

            int targetArmyCountZero;
            for (int i = 0; i < HumanPlayerStrategy::toAttack().size(); i++) {
                auto targetTerritory = HumanPlayerStrategy::toAttack()[i];
                for (int j = 0; j < HumanPlayerStrategy::toDefend().size(); j++) {
                    auto sourceTerritory = HumanPlayerStrategy::toDefend()[j];
                    if (targetTerritory->isAdjacent(sourceTerritory) &&
                        sourceTerritory->getPlayerName() != targetTerritory->getPlayerName() &&
                        sourceTerritory->getTerritoryName() != targetTerritory->getTerritoryName() &&
                        sourceTerritory->getNumberOfArmies() > 0) {
                        if (!std::count(territoriesToAttack[i].begin(), territoriesToAttack[i].end(),
                                        j)) {
                            // not found add it
                            territoriesToAttack[i].push_back(j);
                            targetArmyCountZero++;
                        }
                    }
                }
            }
            if (targetArmyCountZero == 0) {
                cout
                        << "You are unable to attack any territory. You probably have no units on your territories to advance!"
                        << endl;
                goto finishedAttackAdv;
            }
            cout
                    << "Here are the territories you can attack with your territories. Please enter the number shown to issue that order. Enter -1 to stop issuing advance attack orders."
                    << endl;
            auto it = territoriesToAttack.begin();
            int whichToAttack = 0;
            while (it != territoriesToAttack.end()) {
                for (const auto& sourceIndex: it->second) {
                    auto targetT = HumanPlayerStrategy::toAttack()[it->first];
                    auto sourceT = HumanPlayerStrategy::toDefend()[sourceIndex];
                    cout << "[" << whichToAttack << "] Enemy territory " << targetT->getTerritoryName()
                         << " can be attacked by your territory " << sourceT->getTerritoryName() << "." << endl;
                    whichToAttack++;
                }
                it++;
            }
            if (whichToAttack == 0) {
                cout
                        << "Skipping issuing advance attack orders. Your territories have no available units to attack with."
                        << endl;
                goto finishedAttackAdv;
            }
            wrongAttackIndex:
            int attackIndex;
            cin >> attackIndex;
            if (attackIndex > 0 && attackIndex < whichToAttack) {
                Territory* finalTargetTerritory;
                Territory* finalSourceTerritory;
                it = territoriesToAttack.begin();
                whichToAttack = 0;
                while (it != territoriesToAttack.end()) {
                    for (const auto& sourceIndex: it->second) {
                        if (whichToAttack == attackIndex) {
                            finalTargetTerritory = HumanPlayerStrategy::toAttack()[it->first];
                            finalSourceTerritory = HumanPlayerStrategy::toDefend()[sourceIndex];
                            goto outOfHere;
                        }
                        whichToAttack++;
                    }
                    it++;
                }
                outOfHere:
                if (finalTargetTerritory != nullptr && finalSourceTerritory != nullptr) {
                    cout << "How many units would you like to attack with? Available: "
                         << finalSourceTerritory->getNumberOfArmies() << "." << endl;
                    wrongArmyCount:
                    int unitsToAttackWith;
                    cin >> unitsToAttackWith;
                    if (unitsToAttackWith > 0 && unitsToAttackWith <= finalSourceTerritory->getNumberOfArmies()) {
                        Orders* advance_order = new Advance(player, unitsToAttackWith, finalSourceTerritory,
                                                            finalTargetTerritory,
                                                            gameEngine->getDeck(),
                                                            false);
                        player->getOrdersList()->addOrder(advance_order);
                        cout << "Issuing Advance order to attack enemy territory "
                             << finalTargetTerritory->getTerritoryName() << " with your territory "
                             << finalSourceTerritory->getTerritoryName() << " with " << unitsToAttackWith
                             << " army units." << endl;
                        goto continueAdvAttackOrder;
                    } else {
                        cout << " Invalid number of army units to attack with. Try again." << endl;
                        goto wrongArmyCount;
                    }
                }
            } else {
                cout << "here" << endl;
                cout << "Whoops, you wrote an invalid index. Try again." << endl;
                goto wrongAttackIndex;
            }
        } else {
            cout << "Invalid option. Try again" << endl;
            goto issueAttackOrder;
        }
    }
    finishedAttackAdv:

    //Use one card every issue order phase if they have a card
    startOfCards:
    if (!player->getHandCards()->getCards().empty()) {
        cout << "Would you like to use cards? Enter 1 for yes and 0 for no. You have ("
             << player->getHandCards()->getCards().size() << ") cards remaining." << endl;
        int cardUsage;
        cin >> cardUsage;
        if (cardUsage == 1) {
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
                switch (cardsToPlays->getType()) {
                    case BOMB:
                        if (!HumanPlayerStrategy::toAttack().empty()) {
                            int index = 0;
                            cout << "Input which index of the territory you want to use this card on: " << endl;
                            for (auto toAttackTer: HumanPlayerStrategy::toAttack()) {
                                cout << "[" << index << "] Territory " << toAttackTer->getTerritoryName() << endl;
                                index++;
                            }
                            wrongIndexBomb:
                            int i;
                            cin >> i;
                            if (i > 0 && i < HumanPlayerStrategy::toAttack().size()) {
                                Territory* target = toAttack()[i];
                                orderToMake = new Bomb(player, target);
                            } else {
                                cout << "Invalid index. Try again." << endl;
                                goto wrongIndexBomb;
                            }
                        }
                        break;
                    case BLOCKADE:
                        if (!HumanPlayerStrategy::toDefend().empty()) {
                            int index = 0;
                            cout << "Input which index of the territory you want to use this card on: " << endl;
                            for (auto toDef: HumanPlayerStrategy::toDefend()) {
                                cout << "[" << index << "] Territory " << toDef->getTerritoryName() << endl;
                                index++;
                            }
                            wrongIndexBlockade:
                            int i;
                            cin >> i;
                            if (i > 0 && i < HumanPlayerStrategy::toDefend().size()) {
                                Territory* target = HumanPlayerStrategy::toDefend()[i];
                                orderToMake = new Blockade(player, gameEngine->getNeutralPlayer(), target);
                            } else {
                                cout << "Invalid index. Try again." << endl;
                                goto wrongIndexBlockade;
                            }
                        }
                        break;
                    case AIRLIFT:
                        if (!HumanPlayerStrategy::toDefend().empty()) {
                            int index = 0;
                            cout << "Input the index of the source territory: " << endl;
                            for (auto toDef: HumanPlayerStrategy::toDefend()) {
                                cout << "[" << index << "] Territory " << toDef->getTerritoryName() << endl;
                                index++;
                            }
                            wrongIndexAirliftSource:
                            int i;
                            cin >> i;
                            if (i > 0 && i < HumanPlayerStrategy::toDefend().size()) {
                                Territory* source = HumanPlayerStrategy::toDefend()[i];
                                index = 0;
                                cout << "Input the index of the target territory: " << endl;
                                for (auto toDef: HumanPlayerStrategy::toDefend()) {
                                    if (toDef->getTerritoryName() != source->getTerritoryName()) {
                                        cout << "[" << index << "] Territory " << toDef->getTerritoryName() << endl;
                                        index++;
                                    }
                                }
                                wrongIndexAirliftTarget:
                                int j;
                                cin >> j;
                                if (i > 0 && i < HumanPlayerStrategy::toDefend().size()) {
                                    Territory* target = HumanPlayerStrategy::toDefend()[j];
                                    orderToMake = new Airlift(player, player->getReinforcementPool(), source, target);
                                } else {
                                    goto wrongIndexAirliftTarget;
                                }
                            } else {
                                cout << "Invalid index. Try again." << endl;
                                goto wrongIndexAirliftSource;
                            }
                        }
                        break;
                    case DIPLOMACY:
                        if (!HumanPlayerStrategy::toAttack().empty()) {
                            int index = 0;
                            cout << "Input which index of the territory you want to use this card on: " << endl;
                            for (auto toDef: HumanPlayerStrategy::toAttack()) {
                                cout << "[" << index << "] Territory " << toDef->getTerritoryName() << endl;
                                index++;
                            }
                            wrongIndexDiplomacy:
                            int i;
                            cin >> i;
                            if (i > 0 && i < HumanPlayerStrategy::toAttack().size()) {
                                Player* negotiatePlayer = HumanPlayerStrategy::toAttack()[i]->getTerritoryOwner();
                                orderToMake = new Negotiate(player, negotiatePlayer);
                            } else {
                                cout << "Invalid index. Try again." << endl;
                                goto wrongIndexDiplomacy;
                            }
                        }
                        break;
                }
                if (orderToMake != nullptr) {
                    //this handles creating the order and removing it from players hand + back to deck
                    cardsToPlays->play(player, gameEngine->getDeck(), orderToMake);
                    goto startOfCards;
                }
            } else {
                cout << "This card doesn't exist, please try again." << endl;
                goto pickNewCard;
            }
        }
    } else {
        cout << "You do not have any cards to play." << endl;
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
/**
 * Parameterized constructor for the BenevolentPlayerStrategy class
 * @param pPlayer the player that the strategy is for
 */
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* pPlayer) : PlayerStrategy(pPlayer) {}

/**
 * Copy constructor for the BenevolentPlayerStrategy class
 * @param pPlayer The player that the strategy is for
 * @param benevolent_player_strategy The BenevolentPlayerStrategy object to copy
 */
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *pPlayer, const BenevolentPlayerStrategy& benevolent_player_strategy) : PlayerStrategy(pPlayer) {
    this->player = benevolent_player_strategy.player;
}

/**
 * Assignment operator overload for the BenevolentPlayerStrategy class
 * @param benevolent_player_strategy The BenevolentPlayerStrategy object to assign
 * @return A reference to the BenevolentPlayerStrategy object assigned
 */
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& benevolent_player_strategy) {
    if (this == &benevolent_player_strategy) {
        return *this;
    }
    this->player = benevolent_player_strategy.player;
    return *this;
}

/**
 * Stream insertion operator overload for the BenevolentPlayerStrategy class
 * @param outs The output stream
 * @param benevolent_player_strategy The BenevolentPlayerStrategy object to output
 * @return A reference to the output stream which contains class information about the BenevolentPlayerStrategy object
 */
ostream& operator<<(ostream& outs, const BenevolentPlayerStrategy& benevolent_player_strategy) {
    outs << "Name of the Player that uses the BenevolentPlayerStrategy: "
         << benevolent_player_strategy.player->getPlayerName() << endl;
    return outs;
}

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
    if (toDefend().empty()) {
        cout << player->getPlayerName() << " has no territories to defend." << endl;
        return false;
    }
    if (toDefend()[0] == nullptr) {
        cout << player->getPlayerName()
             << " cannot deploy to this territory, which is their weakest one, with their entire/whole reinforcement pool since it does not exist."
             << endl;
        return false;
    }
    if (toDefend()[1] == nullptr) {
        cout << player->getPlayerName()
             << " cannot advance to this territory, which is their second weakest one, or use this territory to deploy army units to a weaker territory using an Airlift order since it does not exist."
             << endl;
        return false;
    }
    if (toDefend().back() == nullptr) {
        cout << player->getPlayerName()
             << " cannot use this territory, which is their strongest one, to advance to another weaker territory since it does not exist."
             << endl;
        return false;
    }

    int count_to_advance = floor(toDefend().back()->getNumberOfArmies() / 2);
    int count_to_airlift = floor(toDefend()[1]->getNumberOfArmies() / 2);

    // here, I am getting the player's two territories with the lowest number of armies, i.e. the two weakest territories,
    // and deploying their entire/whole reinforcement pool to the first territory and advancing half of the number of armies
    // that the player's strongest territory (toDefend().back()) has to the second weakest territory on the vector of territories
    // returned from the toDefend() method. This is a design decision that could be different/changed depending on preference
    Territory* weakest_territory = toDefend()[0];
    Territory* second_weakest_territory = toDefend()[1];

    cout << "Issuing Deploy Order for " << player->getReinforcementPool() << " units to territory "
         << weakest_territory->getTerritoryName() << ".";

    Orders* deploy_order = new Deploy(player, player->getReinforcementPool(), weakest_territory);
    player->getOrdersList()->addOrder(deploy_order);

    cout << "Issuing Advance Order for " << count_to_advance << " units from territory "
         << toDefend().back()->getTerritoryName() << " to territory "
         << second_weakest_territory->getTerritoryName() << ".";

    Orders* advance_order = new Advance(player, count_to_advance, toDefend().back(), second_weakest_territory,
                                        gameEngine->getDeck(), true);
    player->getOrdersList()->addOrder(advance_order);

    if (!player->getHandCards()->getCards().empty()) {
        if (player->getHandCards()->getCards().front() != nullptr) {
            cout << "Issuing Card " << getNameByCardType(player->getHandCards()->getCards().front()->getType()) << endl;
            Orders* order_to_be_made = nullptr;

            switch (player->getHandCards()->getCards().front()->getType()) {
                case BOMB:
                case BLOCKADE:
                case REINFORCEMENT:
                    break;
                case AIRLIFT:
                    order_to_be_made = new Airlift(player, count_to_airlift, second_weakest_territory,
                                                   weakest_territory);
                    break;
                case DIPLOMACY: {
                    Player* negotiation_player = nullptr;
                    for (Player* another_player: gameEngine->getGamePlayers()) {
                        if (another_player != player) {
                            if (negotiation_player == nullptr ||
                                another_player->getTerritories().size() > negotiation_player->getTerritories().size()) {
                                negotiation_player = another_player;
                            }
                        }
                    }
                    order_to_be_made = new Negotiate(player, negotiation_player);
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
                if (!(find(attackTerritories.begin(), attackTerritories.end(), adjacent) !=
                      attackTerritories.end())) {
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
