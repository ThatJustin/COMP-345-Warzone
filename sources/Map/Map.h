#ifndef MAP_H
#define MAP_H

#include <string>
#include <set>
#include "../Player/Player.h"

class Player;
class Territory{
public:
    Territory(Player *player, int numberOfArmies);
    Territory();
    ~Territory();
    Territory(const Territory &territory);

    std::string territory;
    string getPlayerName();
private:
    std::set<std::string> adjacencies;
    Player* player = nullptr;
};

class Map{
public:
    Map(Territory *territory);
    Map();
    Map(const Map &map);
    void traverseMap(int i, bool visited[]);
    bool validate();

private:
    std::set<Territory> *territories;
    bool visited[];

};
class MapLoader{
public:
    MapLoader();
    MapLoader(const MapLoader &mapLoader);

private:
};

#endif //MAP_H