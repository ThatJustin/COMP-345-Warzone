#include <iostream>
#include "headers/PlayerDriver.h"
#include "headers/MapDriver.h"
#include "headers/OrdersDriver.h"
#include "headers/GameEngineDriver.h"
#include "headers/CardsDriver.h"


using namespace std;

int main() {
    //Testing the functionality for Assignment 1.

    //Test Maps
//     cout << "Testing Maps\n" << endl;
//     testLoadMaps();

    //Test Player
//     cout << "Testing Player" << endl;
//     testPlayers();

    //Test OrderList
    // cout << "Testing OrderList" << endl;
    // testOrdersLists();

    //Test Cards deck/hand
    //  cout << "Testing Cards deck/hand" << endl;
    // testCards();

    //Test GameEngine
      cout << "Testing GameEngine" << endl;
      testGameStates();
    return 0;
}