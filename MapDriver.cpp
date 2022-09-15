#include "Map.h";
#include <iostream>

void testLoadMaps(){
    MapLoader *mapLoader = new MapLoader("test.map");
    Map *map = new Map(mapLoader);
    map->validate();
}
