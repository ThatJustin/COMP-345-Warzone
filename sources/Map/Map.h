// Make sure to include the Map header only once when compiling
#pragma once

#include <vector>
#include <iostream>

// Declare the classes used by the Map header before compiling it
class Player;

class Territory;

class Continent {
public:
    // Default Constructor
    Continent();

    // Parameterized Constructor
    Continent(int map_continent_id, const std::string& continent_name);

    // Copy Constructor
    Continent(const Continent& continent);

    // Destructor
    ~Continent();

    // Assignment operator overload
    Continent& operator=(const Continent& continent);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const Continent& continent);

    // Accessor for the continent name
    std::string getContinentName();

    // Accessor for the vector of territory pointers that point to territories found on the continent
    std::vector<Territory*> getTerritories();

    // Adds a territory to the continent
    void addTerritory(Territory* territory);

private:
    // An integer identifier representing the continent id
    int map_continent_id;

    // A string representing the continent name
    std::string continent_name;

    // Collection/list of territory pointers that point to territories found on the continent
    std::vector<Territory*> territories;
};

class Territory {
public:
    // Default Constructor
    Territory();

    // Parameterized constructor taking a player as one of its parameters
    Territory(int map_territory_id, const std::string& territory_name, Player* player);

    // Parameterized constructor taking a continent as one of its parameters
    Territory(int map_territory_id, std::string territory_name, Continent* continent);

    // Copy Constructor
    Territory(const Territory& territory);

    // Destructor
    ~Territory();

    // Assignment operator overload
    Territory& operator=(const Territory& territory);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const Territory& territory);

    // Accessor for the territory id
    int getMapTerritoryId();

    // Mutator for the territory id, mainly used for the adjacent territories
    void setMapTerritoryId(int map_territory_id);

    // Accessor for the number of armies on the territory
    int getNumberOfArmies();

    // Mutator for the number of armies on the territory
    void setNumberOfArmies(int number_of_armies);

    // Accessor for the territory name
    std::string getTerritoryName();

    // Accessor for the vector of territory pointers that point to the adjacent territories of any specific territory object on the map
    std::vector<Territory*> getAdjacentTerritories();

    // Accessor for the pointer to the continent the territory belongs to
    Continent* getContinent();

    // Accessor for the pointer to the player that owns the territory
    Player* getTerritoryOwner();

    // Mutator for the player that owns/will own the territory
    void setTerritoryOwner(Player* player);

    // Accessor for the name of the player that owns the territory
    std::string getPlayerName();

    // Adds an adjacent territory to the specified territory by passing the pointer to the adjacent territory as a parameter
    void addAdjacentTerritory(Territory* territory);

private:
    // An integer identifier representing the territory id and the number of armies on the territory
    int map_territory_id, number_of_armies;

    // A string representing the territory name
    std::string territory_name;

    // Collection/list of territory pointers that point to adjacent territories of each specific territory object on the map
    std::vector<Territory*> adjacent_territories;

    // A pointer to the continent the territory belongs to
    Continent* continent;

    // A pointer to the player that owns the territory
    Player* player;
};

class Map {
public:
    // Default Constructor
    Map();

    // Parameterized Constructor
    Map(std::vector<Territory*> territories, std::vector<Continent*> continents);

    // Copy Constructor
    Map(const Map& map);

    // Destructor
    ~Map();

    // Assignment operator overload
    Map& operator=(const Map& map);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const Map& map);

    // Accessor for the vector of territory pointers that point to all the territories found on the map
    std::vector<Territory*> getTerritories();

    // Accessor for the vector of continent pointers that point to all the continents found on the map
    std::vector<Continent*> getContinents();

    // Accessor for the pointer to a specific territory given its respective territory id as a parameter
    Territory* getTerritory(int map_territory_id);

    // DFS method used for traversing the map/graph in order to know if it is connected or not
    void depthFirstSearch(int starting_territory_id, std::vector<Territory*>& visited_territories);

    // DFS method used for traversing the continents/subgraph in order to know if they each are connected or not
    void
    depthFirstSearch(Continent* continent, int starting_territory_id, std::vector<Territory*>& visited_territories);

    // Method used for validating the maps after they are loaded from the specific map files
    bool validate();


private:
    // Collection/list of territory pointers that point to all territories found on the map
    std::vector<Territory*> territories;

    // Collection/list of continent pointers that point to all continents found on the map
    std::vector<Continent*> continents;
};

class MapLoader {
public:
    // Default Constructor
    MapLoader();

    // Copy Constructor
    MapLoader(const MapLoader& map_loader);

    // Destructor
    ~MapLoader();

    // Assignment operator overload
    MapLoader& operator=(const MapLoader& map_loader);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const MapLoader& map_loader);

    // Method used for loading the map from specific map files
    Map* loadMap(const std::string& map_file_directory);

private:
    // A string representing the specific map file directory to be loaded
    std::string map_file_directory;

    // A pointer to the map object that is to be returned after the specific map file is loaded
    Map* map;
};