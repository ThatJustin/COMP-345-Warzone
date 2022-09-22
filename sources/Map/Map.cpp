#include <iostream>
#include <fstream>
#include "Map.h"

Continent::Continent() {
    this->map_continent_id = 0;
    this->continent_name = "";
    this->territories = vector<Territory*>();
}

Continent::Continent(int map_continent_id, const string& continent_name) {
    this->map_continent_id = map_continent_id;
    this->continent_name = continent_name;
    this->territories = vector<Territory*>();
}

Continent::~Continent() {
    for (auto& territory: this->territories) {
        delete territory;
        territory = nullptr;
    }
    this->territories.clear();
}

Continent::Continent(const Continent& continent) {
    this->map_continent_id = continent.map_continent_id;
    this->continent_name = continent.continent_name;
    this->territories = vector<Territory*>(continent.territories.size());
    for (auto& temp: continent.territories) {
        this->territories.push_back(new Territory(*temp));
    }
}

Continent& Continent::operator=(const Continent& continent) {
    this->map_continent_id = continent.map_continent_id;
    this->continent_name = continent.continent_name;
    this->territories = continent.territories;
    return *this;
}

bool Continent::operator==(const Continent& continent) const {
    return this->map_continent_id == continent.map_continent_id;
}

ostream& operator<<(ostream& outs, const Continent& continent) {
    outs << "Continent Name: " << continent.continent_name << endl <<
         "Number of Territories:\n";

    for (auto territory: continent.territories) {
        outs << territory->getTerritoryName() << endl;
    }
    return outs;
}

int Continent::getMapContinentId() {
    return this->map_continent_id;
}

string Continent::getContinentName() {
    return this->continent_name;
}

vector<Territory*> Continent::getTerritories() {
    return this->territories;
}

void Continent::addTerritory(Territory* territory) {
    for (auto a_territory: this->territories) {
        if (territory->getTerritoryName() == a_territory->getTerritoryName()) {
            return;
        }
    }
    this->territories.push_back(territory);
}

Territory::Territory() {
    this->map_territory_id = 0;
    this->number_of_armies = 0;
    this->territory_name = "";
    this->continent = new Continent();
    this->player = nullptr;
    this->adjacent_territories = vector<Territory*>();
}

Territory::Territory(int map_territory_id, const string& territory_name, Player* player) {
    this->map_territory_id = map_territory_id;
    this->number_of_armies = 0;
    this->territory_name = territory_name;
    this->continent = new Continent();
    this->player = player;
    this->adjacent_territories = vector<Territory*>();
}

Territory::Territory(int map_territory_id, string territory_name, Continent* continent) {
    this->map_territory_id = map_territory_id;
    this->number_of_armies = 0;
    this->territory_name = territory_name;
    this->continent = continent;
    this->player = nullptr;
    this->adjacent_territories = vector<Territory*>();
}

Territory::~Territory() {
    delete this->continent;
    this->continent = nullptr;
    delete this->player;
    this->player = nullptr;

    for (auto& adjacent_territory: this->adjacent_territories) {
        delete adjacent_territory;
        adjacent_territory = nullptr;
    }
    this->adjacent_territories.clear();
}

Territory::Territory(const Territory& territory) {
    this->map_territory_id = territory.map_territory_id;
    this->number_of_armies = territory.number_of_armies;
    this->territory_name = territory.territory_name;
    this->continent = new Continent(*territory.continent);
    this->player = new Player(*territory.player);
    this->adjacent_territories = vector<Territory*>(territory.adjacent_territories.size());
    for (auto& temp: territory.adjacent_territories) {
        this->adjacent_territories.push_back(new Territory(*temp));
    }
}

Territory& Territory::operator=(const Territory& territory) {
    this->map_territory_id = territory.map_territory_id;
    this->number_of_armies = territory.number_of_armies;
    this->territory_name = territory.territory_name;
    this->continent = territory.continent;
    this->player = territory.player;
    this->adjacent_territories = territory.adjacent_territories;
    return *this;
}

bool Territory::operator==(const Territory& territory) const {
    return this->map_territory_id == territory.map_territory_id;
}

ostream& operator<<(ostream& outs, const Territory& territory) {
    outs << "Territory Name: " << territory.territory_name << endl <<
         "Number of Armies: " << territory.number_of_armies << endl <<
         "Within Continent: " << territory.continent->getContinentName() << endl <<
         "Owned by player: " << territory.player->getPlayerName() << endl;
    return outs;
}

int Territory::getMapTerritoryId() {
    return this->map_territory_id;
}

int Territory::getNumberOfArmies() {
    return this->number_of_armies;
}

void Territory::setNumberOfArmies(int army_number) {
    this->number_of_armies = army_number;
}

string Territory::getTerritoryName() {
    return this->territory_name;
}

vector<Territory*> Territory::getAdjacentTerritories() {
    return this->adjacent_territories;
}

Continent* Territory::getContinent() {
    return this->continent;
}

Player* Territory::getTerritoryOwner() {
    return this->player;
}

void Territory::setTerritoryOwner(Player* territory_owner) {
    this->player = territory_owner;
}

string Territory::getPlayerName() {
    return this->player->getPlayerName();
}

Map::Map() {
    this->territories = vector<Territory*>();
    this->continents = vector<Continent*>();
}

Map::Map(vector<Territory*> territories, vector<Continent*> continents) {
    this->territories = territories;
    this->continents = continents;
}

Map::~Map() {
    for (auto& territory: this->territories) {
        delete territory;
        territory = nullptr;
    }
    for (auto& continent: this->continents) {
        delete continent;
        continent = nullptr;
    }
    this->territories.clear();
    this->continents.clear();
}

Map::Map(const Map& map) {
    this->territories = vector<Territory*>(map.territories.size());
    for (auto& temp: map.territories) {
        this->territories.push_back(new Territory(*temp));
    }
    this->territories = vector<Territory*>(map.continents.size());
    for (auto& temp: map.continents) {
        this->continents.push_back(new Continent(*temp));
    }
}

Map& Map::operator=(const Map& map) {
    this->territories = map.territories;
    this->continents = map.continents;
    return *this;
}

ostream& operator<<(ostream& outs, const Map& map) {
    outs << "Map contains the following number of territories:\n" << map.territories.size() << endl <<
         "They are:" << endl;
    for (auto& territory: map.territories) {
        outs << *territory << endl;
    }

    outs << "Map contains the following number of continents: " << map.continents.size() << endl <<
         "They are:" << endl;
    for (auto& continent: map.continents) {
        outs << *continent << endl;
    }
    return outs;
}

vector<Territory*> Map::getTerritories() {
    return this->territories;
}

vector<Continent*> Map::getContinents() {
    return this->continents;
}

Territory* Map::getTerritory(int map_territory_id) {
    for (auto& territory: this->territories) {
        if (territory->getMapTerritoryId() == map_territory_id) {
            return territory;
        }
    }
    return nullptr;
}

Territory* Map::getTerritoryInContinent(int map_territory_id, int map_continent_id) {
    Continent* continent = this->getContinent(map_continent_id);
    if (continent != nullptr) {
        for (auto& territory: continent->getTerritories()) {
            if (territory->getMapTerritoryId() == map_territory_id) {
                return territory;
            }
        }
    }
    return nullptr;
}

Continent* Map::getContinent(int map_continent_id) {
    for (auto& continent: this->continents) {
        if (continent->getMapContinentId() == map_continent_id) {
            return continent;
        }
    }
    return nullptr;
}

//void Map::depthFirstSearch(int starting_territory_id, vector<Territory *> &visited_territories) {
//    Territory* starting_territory = this->getTerritory(starting_territory_id);
//    if(starting_territory != nullptr) {
//        visited_territories.push_back(starting_territory);
//        for(auto &adjacent_territory : starting_territory->getAdjacentTerritories()) {
//            if(find(visited_territories.begin(), visited_territories.end(), adjacent_territory) == visited_territories.end()) {
//                this->depthFirstSearch(adjacent_territory->getMapTerritoryId(), visited_territories);
//            }
//        }
//    }
//}
//
//void Map::depthFirstSearch(vector<Territory*> continent, int starting_territory_id, vector<Territory*> &visited_territories) {
//    Territory* starting_territory = this->getTerritoryInContinent(starting_territory_id, continent[0]->getContinent()->getMapContinentId());
//    if(starting_territory != nullptr) {
//        visited_territories.push_back(starting_territory);
//        for(auto &adjacent_territory : starting_territory->getAdjacentTerritories()) {
//            if(find(visited_territories.begin(), visited_territories.end(), adjacent_territory) == visited_territories.end()) {
//                this->depthFirstSearch(continent, adjacent_territory->getMapTerritoryId(), visited_territories);
//            }
//        }
//    }
//}

bool Map::validate() {
    if (territories.empty() || continents.empty()) {
        return false;
    }

//    for(auto &territory : this->territories) {
//        vector<Territory*> visited_territories;
//        this->depthFirstSearch(territory->getMapTerritoryId(), visited_territories);
//        if(visited_territories.size() != this->territories.size()) {
//            return false;
//        }
//    }
//
//    for(auto &continent : this->continents) {
//        vector<Territory*> visited_territories;
//        this->depthFirstSearch(continent->getTerritories(), continent->getTerritories()[0]->getMapTerritoryId(), visited_territories);
//        if(visited_territories.size() != continent->getTerritories().size()) {
//            return false;
//        }
//    }

    for (auto& territory: this->territories) {
        int territory_in_continent_count = 0;
        for (auto& continent: this->continents) {
            for (auto& territory_in_continent: continent->getTerritories()) {
                if (territory_in_continent->getMapTerritoryId() == territory->getMapTerritoryId()) {
                    territory_in_continent_count++;
                }
            }
        }
        if (territory_in_continent_count != 1) {
            return false;
        }
    }
    return true;
}

MapLoader::MapLoader() {
    this->map_file_directory = "";
    this->map = new Map();
}

MapLoader::~MapLoader() {
    delete this->map;
    this->map = nullptr;
}

MapLoader::MapLoader(const MapLoader& map_loader) {
    this->map_file_directory = map_loader.map_file_directory;
    this->map = new Map(*map_loader.map);
}

Map* MapLoader::loadMap(string map_file_path) {
    vector<Territory*> territories;
    vector<Continent*> continents;
    string map_file_line;
    vector<string> map_file_lines;
    int continents_section_count = 0;
    int map_continent_id = 0;
    int territories_section_count = 0;
    int map_territory_id = 0;

    ifstream map_file(map_file_path);
    if (!map_file) {
        cout << "Error: Unable to open this map file wih the following directory: " << map_file_directory << endl;
        return nullptr;
    }

    while (getline(map_file, map_file_line)) {
        map_file_lines.push_back(map_file_line);
        if (map_file_line == "[Continents]") {
            continents_section_count++;
            while (getline(map_file, map_file_line)) {
                if (map_file_line == "[Territories]" || map_file_line.empty()) {
                    continue;
                }
                map_continent_id++;
                continents.push_back(
                        new Continent(map_continent_id, map_file_line.substr(0, map_file_line.size() - 2)));
            }
        }
        if (continents_section_count > 1) {
            cout << "Error: Invalid map file format for continents." << endl;
            return nullptr;
        }
        if (continents_section_count == 0) {
            cout << "Error: Invalid map file format since it is missing the [Continents] section." << endl;
            return nullptr;
        }
        if (map_file_line == "[Territories]") {
            territories_section_count++;
            while (getline(map_file, map_file_line)) {
                if (map_file_line.empty()) {
                    continue;
                }
                Continent* continent = continents.at(map_territory_id);
                map_territory_id++;
                territories.push_back(
                        new Territory(map_territory_id, map_file_line.substr(0, map_file_line.find(',')), continent));


//                for(const string& continent : continents) {
//                    if(map_file_line.find(continent) != string::npos) {
//                        if(continent == map_file_line.substr(map_file_line.find(continent),continent.size() + 1)) {
//                            aTerritory->setContinent(continent);
//                            cout << aTerritory->getContinentName() << endl;
//                            cout << map_file_line.find(continent) + continent.size() << endl;
//                        }
//                    }
//                }
//                map_file_line = map_file_line.substr(map_file_line.find(','), map_file_line.size());
//                adjacencies.push_back(map_file_line);
            }
        }
        if (territories_section_count > 1) {
            cout << "Error: Invalid map file format for territories." << endl;
            return nullptr;
        }
        if (territories_section_count == 0) {
            cout << "Error: Invalid map file format since it is missing the [Territories] section." << endl;
            return nullptr;
        }
    }
    map_file.close();

    if (map_file_lines.empty()) {
        cout << "Error: This map file is empty." << endl;
        return nullptr;
    }
    if (continents.empty()) {
        cout << "Error: This map file is missing continents." << endl;
        return nullptr;
    }
    if (territories.empty()) {
        cout << "Error: This map file is missing territories." << endl;
        return nullptr;
    }

    map = new Map(territories, continents);
    return map;
}