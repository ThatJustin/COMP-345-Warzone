#include "Map.h"

Territory::Territory() {
    this->territoryName = "";
    this->player = nullptr;
}

Territory::Territory(Player* player, int numberOfArmies) {
    this->player = player;
}

Territory::~Territory() {
    delete player;
}

string Territory::getTerritoryName() {
    return territoryName;
}

string Territory::getPlayerName() {
    return player->getPlayerName();
}

Territory::Territory(const string &territoryName) {
    this->territoryName = territoryName;
    this->player = nullptr;
}

Territory::Territory(const Territory &territory) {
    this->player = territory.player;
    this->territoryName = territory.territoryName;
}

Map::Map() {
    this->continentName = "";
    this->territories = vector<Territory*>();
}

Map::Map(Territory* territory) {
    this->continentName = "";
    this->territories = vector<Territory*>();
    this->territories.push_back(territory);
}

Map::~Map() {
    for (auto* territory : territories) {
        delete territory;
    }
}

Map::Map(const Map &map) {
    this->continentName = map.continentName;
    this->territories = map.territories;
}

bool Map::validate() {
    return false;
}

string Map::getContinentName() {
    return continentName;
}
