#ifndef MAP_H
#define MAP_H

#pragma once
#include "../Player/Player.h"
#include <sstream>

class Player;

class Continent {
public:
    Continent();
    Continent(int map_continent_id, const string &continent_name);
    ~Continent();
    Continent(const Continent &continent);
    Continent &operator = (const Continent &continent);
    bool operator == (const Continent &continent) const;
    friend ostream &operator << (ostream &outs, const Continent &continent);
    int getMapContinentId();
    string getContinentName();
    vector<Territory*> getTerritories();
    void addTerritory(Territory *territory);

private:
    int map_continent_id;
    string continent_name;
    vector<Territory*> territories;
};

class Territory {
public:
    Territory();
    Territory(int map_territory_id, string territory_name, Continent *continent);
    ~Territory();
    Territory(const Territory &territory);
    Territory &operator = (const Territory &territory);
    bool operator == (const Territory &territory) const;
    friend ostream &operator << (ostream &outs, const Territory &territory);
    int getMapTerritoryId();
    int getNumberOfArmies();
    void setNumberOfArmies(int number_of_armies);
    string getTerritoryName();
    vector<Territory*> getAdjacentTerritories();
    Continent* getContinent();
    Player* getTerritoryOwner();
    void setTerritoryOwner(Player *player);
    string getPlayerName();
    void addAdjacentTerritory(Territory* territory);

private:
    int map_territory_id, number_of_armies;
    string territory_name;
    vector<Territory*> adjacent_territories;
    Continent* continent;
    Player* player;
};

class Map {
public:
    Map();
    Map(vector<Territory*> territories, vector<Continent*> continents);
    ~Map();
    Map(const Map &map);
    Map &operator = (const Map &map);
    friend ostream &operator << (ostream &outs, const Map &map);
    vector<Territory*> getTerritories();
    vector<Continent*> getContinents();
    Territory* getTerritory(int map_territory_id);
    Territory* getTerritoryInContinent(int map_territory_id, int map_continent_id);
    Continent* getContinent(int map_continent_id);
    void depthFirstSearch(int starting_territory_id, vector<Territory*> &visited_territories);
    void depthFirstSearch(vector<Territory*> continent, int starting_territory_id, vector<Territory*> &visited_territories);
    bool validate();


private:
    vector<Territory*> territories;
    vector<Continent*> continents;
};

class MapLoader {
public:
    MapLoader();
    ~MapLoader();
    MapLoader(const MapLoader &map_loader);
    Map* loadMap(string map_file_directory);

private:
    string map_file_directory;
    Map* map;
};

#endif //MAP_H