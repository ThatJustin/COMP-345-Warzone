#include "../Player/Player.h"
#include "Map.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::find;
using std::find_if;

/**
 * Default constructor for the Continent class
 */
Continent::Continent() {
    this->map_continent_id = 0;
    this->continent_name = "";
    this->territories = vector<Territory*>();
}

/**
 * Parameterized constructor for the Continent class
 * @param map_continent_id The id of the continent
 * @param continent_name The name of the continent
 */
Continent::Continent(int map_continent_id, const string& continent_name) {
    this->map_continent_id = map_continent_id;
    this->continent_name = continent_name;
    this->territories = vector<Territory*>();
}

/**
 * Copy constructor for the Continent class
 * @param continent The Continent object to copy
 */
Continent::Continent(const Continent& continent) {
    this->map_continent_id = continent.map_continent_id;
    this->continent_name = continent.continent_name;
    this->territories = continent.territories;
}

/**
 * Destructor for the Continent class
 */
Continent::~Continent() {
    for (auto& territory: this->territories) {
        territory = nullptr;
    }
}

/**
 * Assignment operator overload for the Continent class
 * @param continent The Continent object to assign
 * @return A reference to the Continent object assigned
 */
Continent& Continent::operator=(const Continent& continent) {
    this->map_continent_id = continent.map_continent_id;
    this->continent_name = continent.continent_name;
    this->territories = continent.territories;
    return *this;
}

/**
 * Stream insertion operator overload for the Continent class
 * @param outs The output stream
 * @param continent The Continent object to output
 * @return A reference to the output stream which contains class information about the Continent object
 */
ostream& operator<<(ostream& outs, const Continent& continent) {
    outs << "Continent Name: " << continent.continent_name << endl <<
         "Number of Territories: " << continent.territories.size() << endl <<
         "They are:" << endl;
    for (auto territory: continent.territories) {
        outs << territory->getTerritoryName() << endl;
    }
    return outs;
}

/**
 * Accessor for the continent name
 * @return The name of the continent
 */
string Continent::getContinentName() {
    return this->continent_name;
}

/**
 * Accessor for the territories found on the continent
 * @return A vector of territory pointers that point to the territories found on the continent
 */
vector<Territory*> Continent::getTerritories() {
    return this->territories;
}

/**
 * Adds a territory to the continent if it is not already on the continent
 * @param territory The pointer to the territory to add to the continent
 */
void Continent::addTerritory(Territory* territory) {
    for (auto a_territory: this->territories) {
        if (territory->getTerritoryName() == a_territory->getTerritoryName()) {
            return;
        }
    }
    this->territories.push_back(territory);
}

/**
 * Default constructor for the Territory class
 */
Territory::Territory() {
    this->map_territory_id = 0;
    this->number_of_armies = 0;
    this->territory_name = "";
    this->continent = new Continent();
    this->player = nullptr;
    this->adjacent_territories = vector<Territory*>();
}

/**
 * Parameterized constructor for the Territory class
 * @param map_territory_id The id of the territory
 * @param territory_name The name of the territory
 * @param player The pointer to the player that owns the territory
 */
Territory::Territory(int map_territory_id, const string& territory_name, Player* player) {
    this->map_territory_id = map_territory_id;
    this->number_of_armies = 0;
    this->territory_name = territory_name;
    this->continent = new Continent();
    this->player = player;
    this->adjacent_territories = vector<Territory*>();
}

/**
 * Parameterized constructor for the Territory class
 * @param map_territory_id The id of the territory
 * @param territory_name The name of the territory
 * @param continent The pointer to the continent the territory is on/part of
 */
Territory::Territory(int map_territory_id, string territory_name, Continent* continent) {
    this->map_territory_id = map_territory_id;
    this->number_of_armies = 0;
    this->territory_name = std::move(territory_name);
    this->continent = continent;
    this->player = nullptr;
    this->adjacent_territories = vector<Territory*>();
}

/**
 * Copy constructor for the Territory class
 * @param territory The Territory object to copy
 */
Territory::Territory(const Territory& territory) {
    this->map_territory_id = territory.map_territory_id;
    this->number_of_armies = territory.number_of_armies;
    this->territory_name = territory.territory_name;
    this->continent = territory.continent;
    this->player = territory.player;
    this->adjacent_territories = territory.adjacent_territories;
}

/**
 * Destructor for the Territory class
 */
Territory::~Territory() {
    if (continent != nullptr) {
        this->continent = nullptr;
    }
    if (player != nullptr) {
        this->player = nullptr;
    }
    for (auto& adjacent_territory: this->adjacent_territories) {
        adjacent_territory = nullptr;
    }
}

/**
 * Assignment operator overload for the Territory class
 * @param territory The Territory object to assign
 * @return A reference to the Territory object assigned
 */
Territory& Territory::operator=(const Territory& territory) {
    this->map_territory_id = territory.map_territory_id;
    this->number_of_armies = territory.number_of_armies;
    this->territory_name = territory.territory_name;
    this->continent = territory.continent;
    this->player = territory.player;
    this->adjacent_territories = territory.adjacent_territories;
    return *this;
}

/**
 * Stream insertion operator overload for the Territory class
 * @param outs The output stream
 * @param territory The Territory object to output
 * @return A reference to the output stream which contains class information about the Territory object
 */
ostream& operator<<(ostream& outs, const Territory& territory) {
    outs << "Territory Name: " << territory.territory_name << endl <<
         "Number of Armies: " << territory.number_of_armies << endl <<
         "Within Continent: " << territory.continent->getContinentName() << endl;
    if (territory.player == nullptr || territory.player->getPlayerName().empty()) {
        outs << "Owned by player: No player" << endl;
    } else {
        outs << "Owned by player: " << territory.player->getPlayerName() << endl;
    }
    return outs;
}

/**
 * Accessor for the territory id
 * @return The id of the territory
 */
int Territory::getMapTerritoryId() {
    return this->map_territory_id;
}

/**
 * Mutator for the territory id
 * @param territory_id The id of the territory to set
 */
void Territory::setMapTerritoryId(int territory_id) {
    this->map_territory_id = territory_id;

}

/**
 * Accessor for the number of armies on the territory
 * @return The number of armies on the territory
 */
int Territory::getNumberOfArmies() {
    return this->number_of_armies;
}

/**
 * Mutator for the number of armies on the territory
 * @param army_number The number of armies to set on the territory
 */
void Territory::setNumberOfArmies(int army_number) {
    this->number_of_armies = army_number;
}

/**
 * Accessor for the territory name
 * @return The name of the territory
 */
string Territory::getTerritoryName() {
    return this->territory_name;
}

/**
 * Accessor for the vector of territory pointers that point to the adjacent territories of any specific territory object on the map
 * @return The vector of territory pointers that point to the adjacent territories of any specific territory object on the map
 */
vector<Territory*> Territory::getAdjacentTerritories() {
    return this->adjacent_territories;
}

/**
 * Accessor for the pointer to the continent the territory belongs to
 * @return The pointer to the continent the territory belongs to
 */
Continent* Territory::getContinent() {
    return this->continent;
}

/**
 * Accessor for the pointer to the player that owns the territory
 * @return The pointer to the player that owns the territory
 */
Player* Territory::getTerritoryOwner() {
    return this->player;
}

/**
 * Mutator for the pointer to the player that owns/will own the territory
 * @param player The pointer to the player that owns/will own the territory
 */
void Territory::setTerritoryOwner(Player* territory_owner) {
    this->player = territory_owner;
}

/**
 * Accessor for the name of the player that owns the territory
 * @return The name of the player that owns the territory
 */
string Territory::getPlayerName() {
    return this->player->getPlayerName();
}

/**
 * Adds an adjacent territory to the specified territory, if it is not already adjacent to it, by passing the pointer to the adjacent territory as a parameter
 * @param territory The pointer to the adjacent territory to add
 */
void Territory::addAdjacentTerritory(Territory* territory) {
    for (auto adjacent_territory: this->adjacent_territories) {
        if (territory->getTerritoryName() == adjacent_territory->getTerritoryName()) {
            return;
        }
    }
    this->adjacent_territories.push_back(territory);
}

/**
 * Default constructor for the Map class
 */
Map::Map() {
    this->territories = vector<Territory*>();
    this->continents = vector<Continent*>();
}

/**
 * Parameterized constructor for the Map class
 * @param territories The vector of territory pointers that point to all territories found on the map
 * @param continents The vector of continent pointers that point to all continents found on the map
 */
Map::Map(vector<Territory*> territories, vector<Continent*> continents) {
    this->territories = std::move(territories);
    this->continents = std::move(continents);
}

/**
 * Copy constructor for the Map class
 * @param map The Map object to copy
 */
Map::Map(const Map& map) {
    this->territories = map.territories;
    this->continents = map.continents;
}

/**
 * Destructor for the Map class
 */
Map::~Map() {
    for (auto& territory: this->territories) {
        delete territory;
        territory = nullptr;
    }
    for (auto& continent: this->continents) {
        delete continent;
        continent = nullptr;
    }
}

/**
 * Assignment operator overload for the Map class
 * @param map The Map object to assign
 * @return A reference to the Map object assigned
 */
Map& Map::operator=(const Map& map) {
    this->territories = map.territories;
    this->continents = map.continents;
    return *this;
}

/**
 * Stream insertion operator overload for the Map class
 * @param outs The output stream
 * @param map The Map object to output
 * @return A reference to the output stream which contains class information about the Map object
 */
ostream& operator<<(ostream& outs, const Map& map) {
    outs << "Map contains the following number of territories: " << map.territories.size() << endl <<
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

/**
 * Accessor for the vector of territory pointers that point to all territories found on the map
 * @return The vector of territory pointers that point to all territories found on the map
 */
vector<Territory*> Map::getTerritories() {
    return this->territories;
}

/**
 * Accessor for the vector of continent pointers that point to all continents found on the map
 * @return The vector of continent pointers that point to all continents found on the map
 */
vector<Continent*> Map::getContinents() {
    return this->continents;
}

/**
 * Accessor for the pointer to a specific territory given its respective territory id as a parameter
 * @param map_territory_id The id of the territory to find/get
 * @return The pointer to the territory found/gotten with the specified id if it exists inside the vector of territory pointers, otherwise it returns a null territory pointer
 */
Territory* Map::getTerritory(int map_territory_id) {
    for (auto& territory: this->getTerritories()) {
        if (territory->getMapTerritoryId() == map_territory_id) {
            return territory;
        }
    }
    return nullptr;
}

/**
 * DFS method used for traversing the map/graph in order to know if it is connected or not
 * @param starting_territory_id The id of the territory to start the traversal from
 * @param visited_territories The vector of territory pointers that point to the territories that have been visited during the traversal so far
 */
void Map::depthFirstSearch(int starting_territory_id, vector<Territory*>& visited_territories) {

    // If the territory pointer points to a territory that has already been visited with that specific territory id, then return the recursive call
    if (find(visited_territories.begin(), visited_territories.end(), this->getTerritory(starting_territory_id)) !=
        visited_territories.end()) {
        return;
    }

    // If the territory pointer, that points to a territory that has not been visited with that specific territory id,
    // is not a null territory pointer, then add it to the vector of visited territory pointers that point to visited territories
    if (this->getTerritory(starting_territory_id) != nullptr) {
        visited_territories.push_back(this->getTerritory(starting_territory_id));

        // Here, I loop through all the adjacent territories of the territory being pointed to by the territory pointer
        // with that specific territory id and checking if those adjacent territories have been visited or not.
        // If they have not, then I call the DFS method recursively on them
        for (auto& adjacent_territory: this->getTerritory(starting_territory_id)->getAdjacentTerritories()) {
            if (find(visited_territories.begin(), visited_territories.end(), adjacent_territory) ==
                visited_territories.end()) {
                this->depthFirstSearch(adjacent_territory->getMapTerritoryId(), visited_territories);
            }
        }
    }
}

/**
 * DFS method used for traversing the continents/subgraph in order to know if they each are connected or not
 * @param continent The continent which needs to be checked during the traversal of all its territories (the continent is a subgraph of the map/graph)
 * @param starting_territory_id The id of the first territory inside the specific continent to start the traversal from
 * @param visited_territories The vector of territory pointers that point to the territories that have been visited during the traversal of the specific continent so far
 */
void Map::depthFirstSearch(Continent* continent, int starting_territory_id, vector<Territory*>& visited_territories) {

    // If the territory pointer points to a territory that has already been visited with that specific territory id
    // inside that specific continent, then return the recursive call
    if (find(visited_territories.begin(), visited_territories.end(), this->territories.at(starting_territory_id)) !=
        visited_territories.end()) {
        return;
    }

    // If the territory pointer, that points to a territory that has not been visited with that specific territory id inside that specific continent,
    // is not a null territory pointer, then add it to the vector of visited territory pointers that point to visited territories
    if (this->territories.at(starting_territory_id) != nullptr) {
        visited_territories.push_back(this->territories.at(starting_territory_id));

        // Here, I loop through all the territories inside the specific continent and their adjacent territories and
        // checking if those territories exist in the map/graph. If they do, then I make sure to only call the DFS method recursively
        // on adjacent territories that are inside the specific continent and have not been visited yet
        for (auto& territory: continent->getTerritories()) {
            for (auto& adjacent_territory: territory->getAdjacentTerritories()) {
                auto iterator = find_if(this->territories.begin(), this->territories.end(), [&](Territory* territory) {
                    return territory->getTerritoryName() == adjacent_territory->getTerritoryName();
                });
                if (iterator != this->territories.end()) {
                    if ((this->territories.at(
                            adjacent_territory->getMapTerritoryId())->getContinent()->getContinentName() !=
                         continent->getContinentName() ||
                         (adjacent_territory->getTerritoryName() == territory->getTerritoryName()))) {
                        continue;
                    }
                    if (find(visited_territories.begin(), visited_territories.end(), adjacent_territory) ==
                        visited_territories.end()) {

                        // Here, I need an extra check in case the territory with this adjacent territory has not been visited yet by
                        // adding it to the vector of visited territory pointers that point to visited territories
                        if (find(visited_territories.begin(), visited_territories.end(), territory) ==
                            visited_territories.end()) {
                            visited_territories.push_back(territory);
                        }
                        this->depthFirstSearch(continent, adjacent_territory->getMapTerritoryId(), visited_territories);
                    }
                }
            }
        }
    }
}

/**
 * Method used for validating the maps after they are loaded from the specific map files
 * @return True if the map is valid, otherwise it returns false if it is not valid
 */
bool Map::validate() {
    vector<Territory*> visited_territories;

    // Here, I am looping through all the territories and comparing each to each other to see
    // if there exists a territory pointer in the vector of territory pointers that points to a territory with two different continents
    for (auto& territory: this->getTerritories()) {
        for (auto& comparing_territory: this->getTerritories()) {
            if (territory->getTerritoryName() == comparing_territory->getTerritoryName() &&
                territory->getContinent()->getContinentName() !=
                comparing_territory->getContinent()->getContinentName()) {
                cout << "\tMap is not valid because the territory \"" << territory->getTerritoryName()
                     << "\" is in two different continents." << endl;
                return false;
            }
        }
    }

    // Here, I am using the DFS method to traverse the map/graph and check if it is connected or not.
    // If the map/graph is not connected, then the map is not valid (the DFS method will not visit all
    // territories found in the map/graph as some will be unreachable)
    this->depthFirstSearch(this->getTerritories()[0]->getMapTerritoryId(), visited_territories);
    if (visited_territories.size() != this->getTerritories().size()) {
        cout << "\tMap is not valid because it is not a connected graph." << endl;
        return false;
    }

    // Here, I am looping through all the continents and using the DFS method to traverse each continent/subgraph one at a time
    // in order to check if each continent/subgraph is connected or not. If the continent/subgraph is not connected, then the map is not valid
    // (the DFS method will not visit all territories found in the continent/subgraph as some will be unreachable by the other territories within the same continent)
    for (auto& continent: this->getContinents()) {
        if (continent->getTerritories().empty()) {
            cout << "\tMap is not valid because the continent \"" << continent->getContinentName()
                 << "\" contains no territories." << endl;
            return false;
        }
        visited_territories.clear();
        this->depthFirstSearch(continent, continent->getTerritories()[0]->getMapTerritoryId(), visited_territories);
        if (visited_territories.size() != continent->getTerritories().size()) {
            cout << "\tMap is not valid because the continent \"" << continent->getContinentName()
                 << "\" is not a connected subgraph." << endl;
            return false;
        }
    }
    return true;
}

/**
 * Default constructor for the MapLoader class
 */
MapLoader::MapLoader() {
    this->map_file_directory = "";
    this->map = new Map();
}

/**
 * Copy constructor for the MapLoader class
 * @param map_loader The MapLoader object to copy
 */
MapLoader::MapLoader(const MapLoader& map_loader) {
    this->map_file_directory = map_loader.map_file_directory;
    this->map = map_loader.map;
}

/**
 * Destructor for the MapLoader class
 */
MapLoader::~MapLoader() {
    delete this->map;
    this->map = nullptr;
}

/**
 * Assignment operator for the MapLoader class
 * @param map_loader The MapLoader object to assign
 * @return The MapLoader object that was assigned
 */
MapLoader& MapLoader::operator=(const MapLoader& map_loader) {
    this->map_file_directory = map_loader.map_file_directory;
    this->map = map_loader.map;
    return *this;
}

/**
 * Stream insertion operator overload for the MapLoader class
 * @param outs The output stream
 * @param map The MapLoader object to output
 * @return A reference to the output stream which contains class information about the MapLoader object
 */
ostream& operator<<(ostream& outs, const MapLoader& map_loader) {
    outs << "Map file directory: " << map_loader.map_file_directory << endl;
    outs << *map_loader.map;
    return outs;
}

/**
 * Method used for loading the map from specific map files
 * @param map_file_path The directory/path of the map file to load
 * @return The pointer to the map that was loaded from the map file if the map file was loaded successfully, otherwise it returns a null map pointer
 */
Map* MapLoader::loadMap(const string& map_file_path) {
    this->map_file_directory = map_file_path;
    vector<Territory*> territories;
    vector<Continent*> continents;
    vector<string> map_file_lines;
    string map_file_line;
    int continents_section_count = 0;
    int map_continent_id = 0;
    int territories_section_count = 0;
    int map_territory_id = 0;
    int map_file_line_first_territory_id = 0;

    ifstream map_file(map_file_path);

    // If the map file doesn't exist with this directory/path or this specific map file name, then return a null map pointer
    if (!map_file) {
        cout << "\tError: Unable to open/load the map file " << map_file_path.substr(12)
             << " wih the following directory: " << map_file_directory
             << ". Make sure that this map file exists within the \"" << map_file_directory.substr(2, 9) << "\" folder."
             << endl;
        return nullptr;
    }

    // Here, I am parsing each line of each map file in real time, separating this while loop into two sections:
    // One for the presence of the [Continents] section and one for the [Territories] section
    while (getline(map_file, map_file_line)) {

        // I am storing the map file lines into a vector of strings in order to use them later on to check if the
        // map file is actually empty after closing it and print an error message before retuning a null map pointer
        map_file_lines.push_back(map_file_line);
        if (map_file_line.empty() || map_file_line == "\r" || map_file_line == "\n" || map_file_line == " ") {
            continue;
        }
        // If the map file line is the [Continents] section, then I am
        // incrementing the continents_section_count variable for error checking later on
        if (map_file_line == "[Continents]" || map_file_line == "[Continents]\r") {
            continents_section_count++;
            while (getline(map_file, map_file_line)) {
                if (map_file_line == "[Continents]" || map_file_line == "[Continents]\r") {
                    continents_section_count++;
                    break;
                }
                if (map_file_line == "[Territories]" || map_file_line == "[Territories]\r" || map_file_line.empty()) {
                    break;
                }

                // I take a substring of the map file line which corresponds to the continent name then
                // adding a new continent to the vector of continents with that continent name
                continents.push_back(new Continent(map_continent_id, map_file_line.substr(0, map_file_line.find('='))));
                map_continent_id++;
            }
        }

        map_continent_id = 0;

        // If the map file line is the [Territories] section, then I am incrementing the
        // territories_section_count variable for error checking later on
        if (map_file_line == "[Territories]" || map_file_line == "[Territories]\r") {
            territories_section_count++;
            while (getline(map_file, map_file_line)) {
                if (map_file_line == "[Continents]" || map_file_line == "[Continents]\r") {
                    if (continents_section_count == 0) {
                        cout << "\tError: Invalid map file format for continents in the map file "
                             << map_file_path.substr(12) << "." << endl;
                        return nullptr;
                    }
                    continents_section_count++;
                    break;
                }
                if (map_file_line == "[Territories]" || map_file_line == "[Territories]\r") {
                    cout << "\tError: Invalid map file format for territories in the map file "
                         << map_file_path.substr(12) << "." << endl;
                    return nullptr;
                }
                if (continents.empty() || map_file_line.empty() || map_file_line == " " || map_file_line == "\n" ||
                    map_file_line == "\r") {
                    continue;
                }

                // Here, I am assigning the continent pointer to an arbitrary continent pointer using 0 for the map_continent_id since I will
                // change it later on in the inner while loop anyway by making sure to change the newly added territory's
                // continent pointer to the continent pointer that corresponds to the continent name parsed from that specific part of the map file line
                Continent* continent = continents.at(map_continent_id);

                // I am turning the map file line into a string stream in order to parse the map file line more easily
                stringstream map_file_line_stream(map_file_line);

                // I am getting a substring of the map file line that ends before the first comma in the map file line
                // which corresponds to the territory name on each map file line that includes strings
                string territory_name = map_file_line.substr(0, map_file_line.find(','));
                int skipped_strings_index = 0;

                // Here, I am skipping the first 2 strings in the map file line which correspond to the territory coordinates
                // which are not relevant to the game or the project's implementation
                while (getline(map_file_line_stream, map_file_line, ',')) {
                    map_file_line = map_file_line.substr(map_file_line.find_first_not_of(' '),
                                                         map_file_line.find_last_not_of(' ') + 1);
                    skipped_strings_index++;

                    // When the map file line is equal to the continent name on each map file line, I make sure check
                    // that the continent actually exists in the vector of continent pointers that point to all the continents in the map using a Continent object iterator, and if it does,
                    // I create a new territory with the territory name and the continent pointer that corresponds to the continent name parsed from that specific part of the map file line
                    if (skipped_strings_index == 4) {
                        auto iterator = find_if(continents.begin(), continents.end(), [&](Continent* continent) {
                            return continent->getContinentName() == map_file_line;
                        });
                        if (iterator != continents.end()) {
                            continent = *iterator;
                            auto* map_file_line_first_territory = new Territory(map_territory_id, territory_name,
                                                                                continent);
                            territories.push_back(map_file_line_first_territory);
                            map_territory_id++;
                            continent->addTerritory(map_file_line_first_territory);
                            continue;
                        } else {
                            map_file_line_first_territory_id--;
                            break;
                        }
                    }
                    if (skipped_strings_index < 4) {
                        continue;
                    }

                    // Here, I am adding the adjacent territories to the newly added territory using the same territory id as the newly added territory
                    // which will be changed later on so that each adjacent territory actually holds the right territory id to reference the first time they
                    // are parsed at the beginning of a new map file line
                    territories.at(map_file_line_first_territory_id)->addAdjacentTerritory(
                            new Territory(map_territory_id, map_file_line, continent));
                }

                // This is the id that tracks the first territory  on each map file line
                map_file_line_first_territory_id++;
            }
        }
    }

    // I make sure to close the map file before returning the map pointer in order to avoid memory leaks
    map_file.close();

    // If the map file is empty, then an error is printed and a null map pointer is returned:
    // This is where the vector of strings that hold the map file lines is used to check if the map file is empty
    if (map_file_lines.empty()) {
        cout << "\tError: The map file " << map_file_path.substr(12) << " is empty." << endl;
        return nullptr;
    }

    // If "[Continents]" appears more than once in a map file, which is not normal, then an error is printed and a null map pointer is returned
    if (continents_section_count > 1) {
        cout << "\tError: Invalid map file format for continents in the map file " << map_file_path.substr(12) << "."
             << endl;
        return nullptr;
    }

    // If "[Territories]" never appears in a map file, which is not normal, then an error is printed and a null map pointer is returned
    if (territories_section_count == 0) {
        cout << "\tError: Invalid map file format in the map file " << map_file_path.substr(12)
             << " since it is missing the [Territories] section." << endl;
        return nullptr;
    }

    // If "[Continents]" never appears in a map file, which is not normal, then an error is printed and a null map pointer is returned
    if (continents_section_count == 0) {
        cout << "\tError: Invalid map file format in the map file " << map_file_path.substr(12)
             << " since it is missing the [Continents] section." << endl;
        return nullptr;
    }

    // If there are no continents in the map file, then an error is printed and a null map pointer is returned
    if (continents.empty()) {
        cout << "\tError: The map file " << map_file_path.substr(12) << " is missing continents." << endl;
        return nullptr;
    }

    // If there are no territories in the map file, then an error is printed and a null map pointer is returned
    if (territories.empty()) {
        cout << "\tError: The map file " << map_file_path.substr(12) << " is missing territories." << endl;
        return nullptr;
    }

    // Here, I am basically looping through every adjacent territory of every territory
    // in each continent and making sure the adjacent territory's territory id is the same as the
    // territory id of the territory that has the same territory name as the adjacent territory:
    // This is done using a Territory object iterator
    for (auto& continent: continents) {
        for (int i = 0; i < continent->getTerritories().size(); i++) {
            for (int j = 0; j < continent->getTerritories()[i]->getAdjacentTerritories().size(); j++) {
                if ((continent->getTerritories()[i]->getMapTerritoryId() + 1) ==
                    continent->getTerritories()[i]->getAdjacentTerritories()[j]->getMapTerritoryId()) {
                    auto iterator = find_if(territories.begin(), territories.end(), [&](Territory* territory) {
                        return territory->getTerritoryName() ==
                               continent->getTerritories()[i]->getAdjacentTerritories()[j]->getTerritoryName();
                    });
                    if (iterator != territories.end()) {
                        continent->getTerritories()[i]->getAdjacentTerritories()[j]->setMapTerritoryId(
                                territories.at(distance(territories.begin(), iterator))->getMapTerritoryId());
                    }
                }
            }
        }
    }

    // Before returning the map pointer, I make sure to create a new Map
    // object with the collection of gathered territories and continents from
    // the map file in their vector of pointers that point to territories and continents, respectively
    map = new Map(territories, continents);
    return map;
}