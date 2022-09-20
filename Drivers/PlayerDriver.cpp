#include <iostream>
#include "sources/Player/Player.h"
#include "sources/Map//Map.h"

void testPlayers() {
    auto* player = new Player("Adam");
    auto* territory1 = new Territory(9, "Xasnines", player);
    player->getTerritories().push_back(territory1);

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
        if (size == player->toDefend().size()) {
            cout << terr->getTerritoryName() << endl;
        } else {
            cout << terr->getTerritoryName() << ", ";
        }
        size++;
    }
    player->toAttack().clear();
    delete territory1;
    delete player;
}

int main() {
    testPlayers();
}



