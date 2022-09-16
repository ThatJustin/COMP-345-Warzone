#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "sources/Map//Map.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::cerr;
using std::istringstream;

void testLoadMaps() {
    ifstream mapFile("./Map Files/World.map");
    vector<string> continents;
    vector<Territory*> territories;
    string line;
    string continentName;
    Territory *aTerritory;

    if (!mapFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    while(getline(mapFile, line)) {
        if(line == "[Continents]"){
            while(getline(mapFile, line)) {
                if(line == "[Territories]" || line.empty()) {
                    break;
                }
                continentName = line.substr(0, line.size() - 2);
                continents.push_back(continentName);
            }
        }
        if(line == "[Territories]") {
            while(getline(mapFile, line)) {
                if(line.empty()) {
                    continue;
                }
                aTerritory = new Territory(line.substr(0, line.find(',')));
                territories.push_back(aTerritory);
            }
        }
    }

    mapFile.close();
    cout << "Map Continents:" << endl;
    for(const string& continent : continents) {
        cout << continent << endl;
    }

    cout << "\nMap Territories:" << endl;
    for(Territory *territory : territories) {
        cout << territory->getTerritoryName() << endl;
    }
    delete aTerritory;
}

int main() {
    testLoadMaps();
    return 0;
}



