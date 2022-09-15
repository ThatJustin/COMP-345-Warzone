#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

int main(){
    string parsedLine;
    vector<string> parsedLines;
    ifstream mapFile("World.map");

    while(mapFile >> parsedLine){
        parsedLines.push_back(parsedLine);
    }

    for(string line : parsedLines){
        cout << parsedLine << endl;
    }

    return 0;
}