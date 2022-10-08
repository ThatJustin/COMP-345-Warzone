//#include <filesystem>
#include "sources/Map//Map.h"
#include <filesystem>

using std::cout;
using std::endl;
using std::vector;
using std::string;

/**
 * This function tests the loadMaps() and validate() functions/methods in the MapLoader and Map classes, respectively.
 */
void testLoadMaps() {
    auto* mapLoader = new MapLoader();
    vector<Map*> maps;
    vector<string> map_file_names;
    auto path = std::filesystem::path("./Map Files");

    // Here, I am checking if the folder path does not exist
    if (!exists(path)) {
        cout << "The " << path.string().substr(2) << " folder does not exist" << endl;
        return;
    }

    cout << "Map file names:" << endl;
    // Here, I am looping through all the map files in the Map Files folder and loading them one by one
    for (auto& map_file: std::filesystem::directory_iterator(path)) {
        if (map_file.path().string().substr(12) == ".DS_Store") {
            continue;
        }
        cout << "\tMap file: " << map_file.path().string().substr(12) << endl;
        map_file_names.push_back(map_file.path().string().substr(12));

        // If the map file is not loadable, then it should be returning a null Map object, hence the check here
        if (mapLoader->loadMap(map_file.path().string()) == nullptr) {
            cout << "\tThe map " << map_file.path().string().substr(12) << " cannot be loaded." << endl;
            map_file_names.pop_back();
            continue;
        }
        maps.push_back(mapLoader->loadMap(map_file.path().string()));
    }

    // Here, I am making sure that I can't load a map file that does not even exist (this should return a null Map object)
    mapLoader->loadMap("./Map Files/Map_File_That_Does_Not_Exist.map");

    // Here, I am looping through all the generated/loaded maps from the Map Files folder and validating them one by one
    cout << "\nValidation of map files:" << endl;
    for (int i = 0; i < maps.size(); i++) {
        if (maps[i]->validate()) {
            cout << "\t" << map_file_names[i] << " is a valid map file" << endl;
        } else {
            cout << "\t" << map_file_names[i] << " is an invalid map file" << endl;
        }
    }
    // I make sure to delete the MapLoader object here before the program/process is finished, which deletes the Map objects that were loaded as well
    delete mapLoader;
}