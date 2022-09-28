#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::cerr;
using std::istringstream;

void testLoadMaps() {
    auto *mapLoader = new MapLoader();
    mapLoader->loadMap("./Map Files/World.map");

    delete mapLoader;
}