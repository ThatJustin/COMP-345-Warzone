#include <iostream>
#include "sources/Player/Player.h"
#include "sources/Map//Map.h"

void testPlayers() {
    auto* player = new Player("lols");
    std::cout << player->getPlayerName() << std::endl;
    player->getPlayerName() = "lel";
    std::cout << player->getPlayerName() << std::endl;
    auto* terr = new Territory();
    terr->territory = "lol";
    std::cout << terr->territory << std::endl;
    std::cout << terr->getPlayerName() << std::endl;
    delete terr;
//    delete player;
//    auto* player = new Player("lols");
//    player->getTerritories();
    delete player;
}
int main() {
    testPlayers();
}



