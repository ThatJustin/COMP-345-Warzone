#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "sources/Map//Map.h"

void testLoadMaps() {
//    cout << "Map Continents:" << endl;
//    for(const string& continent : continents) {
//        cout << continent << endl;
//    }
//
    cout << "\nMap Territories:" << endl;
    for(Territory *territory : territories) {
        cout << territory->getTerritoryName() << endl;
    }
//
//    cout << "\nMap Adjacencies:" << endl;
//    for(const string& adjacency : adjacencies) {
//        cout << adjacency << endl;
//    }
}

int main() {
    testLoadMaps();
    return 0;
}