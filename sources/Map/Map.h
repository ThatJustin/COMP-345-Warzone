#ifndef MAP_H
#define MAP_H

#include <string>
#include "../Player/Player.h"

class Player;

class Territory {
public:
    Territory(Player *player, int numberOfArmies);
    Territory();
    ~Territory();
    Territory(const Territory &territory);
    explicit Territory(const string &territoryName);
    string getTerritoryName();
    string getPlayerName();

private:
    string territoryName;
    Player* player;
};

class Map {
public:
    explicit Map(Territory *territory);
    Map();
    ~Map();
    Map(const Map &map);
    bool validate();
    string getContinentName();

private:
    string continentName;
    vector<Territory*> territories;
};

class MapLoader {
public:
    MapLoader();
    ~MapLoader();
    MapLoader(const MapLoader &mapLoader);

private:
};

#endif //MAP_H