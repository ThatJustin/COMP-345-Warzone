#include "Map.h";
#include <iostream>
#include <fstream>
using std::cout;

ifstream fin("/MapFiles/World.map");

if(fin.fail( )) {
    cout << "Error - Failed to open " << endl;
    exit( -1 );
}

cout << "Success - Opened " << endl;

