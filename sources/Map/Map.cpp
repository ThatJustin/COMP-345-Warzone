#include "Map.h"

Territory::Territory() {

}

Territory::Territory(Player* player, int numberOfArmies) {
    this->player = player;
}

Territory::~Territory() {
    delete player;
}

std::string Territory::getPlayerName() {
    return player->getPlayerName();
}
