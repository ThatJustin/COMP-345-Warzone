#ifndef MAP_H
#define MAP_H

class Map{
    public:
        Map(Territory *territory, );
        Map();
        Map(const Map &map);
        void traverseMap(int i, bool visited[]);
        bool validate();

    private:
        set<Territory> *territories;
        bool visited[];

};

class Territory{
    public:
        Territory(Player *player, int numberOfArmies);
        Territory();
        Territory(const Territory &territory);

    private:
        String territory;
        set<String> adjacencies;
};

class MapLoader{
    public:
        MapLoader();
        MapLoader(const MapLoader &mapLoader);

    private:
        set<File> *mapFiles;
};

#endif //MAP_H