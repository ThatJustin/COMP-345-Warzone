#include <iostream>
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include "sources/Cards/Cards.h"

void testPlayers() {
    //Create the player
    auto* player = new Player("Adam");

    //Create the territory and assign it to the player
    auto* terrXasnines = new Territory(9, "Xasnines", player);
    auto* terrZlami = new Territory(9, "Zlami", player);
    player->addTerritory(terrXasnines);
    player->addTerritory(terrZlami);

    //Test player cards by creating a card and giving it to the player
    auto* cardB = new Cards(CardsType::BOMB);
    auto* cardA = new Cards(CardsType::AIRLIFT);
    auto* cardD = new Cards(CardsType::DIPLOMACY);
    Hand* hand = new Hand;
    hand->addCard(cardB);
    hand->addCard(cardA);
    hand->addCard(cardD);
    player->setHandCards(hand);

    //Issues an order for assignment 1
    player->issueOrder();

    cout << *player << endl;

    //Returns an arbitrary list of territories to be defended,
    cout << "Territories to Defend:\n" << endl;
    int size = 1;
    for (auto& terr: player->toDefend()) {
        if (size == player->toDefend().size()) {
            cout << terr->getTerritoryName() << "\r\n" << endl;
        } else {
            cout << terr->getTerritoryName() << ", ";
        }
        size++;
    }
    player->toDefend().clear();
    size = 1;

    // Returns an arbitrary list of territories to be attacked,
    cout << "Territories to Attack:\n" << endl;
    for (auto& terr: player->toAttack()) {
        if (size == player->toAttack().size()) {
            cout << terr->getTerritoryName() << endl;
        } else {
            cout << terr->getTerritoryName() << ", ";
        }
        size++;
    }
    player->toAttack().clear();

    delete terrXasnines;
    delete terrZlami;
    delete player; // player destructor will clean up hand and orderlist
}