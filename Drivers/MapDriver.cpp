#include <filesystem>

using std::cout;
using std::endl;

void testLoadMaps() {
    auto *mapLoader = new MapLoader();
    vector<Map*> maps;
    vector<string> map_file_names;
    auto path = filesystem::path("./Map Files");

    cout << "Map file names:" << endl;
    for(auto &map_file : filesystem::directory_iterator(path)) {
        if(map_file.path().string().substr(12) == ".DS_Store") {
            continue;
        }
        cout << "\tMap file: " << map_file.path().string().substr(12) << endl;
        map_file_names.push_back(map_file.path().string().substr(12));
        if(mapLoader->loadMap(map_file.path().string()) == nullptr) {
            map_file_names.pop_back();
            continue;
        }
        maps.push_back(mapLoader->loadMap(map_file.path().string()));
    }

    cout << "\nValidation of map files:" << endl;
    for(int i = 0; i < maps.size(); i++) {
        if(maps[i]->validate()) {
            cout << "\t" << map_file_names[i] << " is a valid map file" << endl;
        } else {
            cout << "\t" << map_file_names[i] << " is an invalid map file" << endl;
        }
    }
    delete mapLoader;
}