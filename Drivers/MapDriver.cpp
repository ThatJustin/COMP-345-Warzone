#include <iostream>
#include <string>
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
    MapLoader mapLoader;
    mapLoader.loadMap("./Map Files/World.map");
}

int main() {
    testLoadMaps();
    return 0;
}