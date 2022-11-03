#include <iostream>
#include "headers/PlayerDriver.h"
#include "headers/MapDriver.h"
#include "headers/OrdersDriver.h"
#include "headers/GameEngineDriver.h"
#include "headers/CardsDriver.h"
#include "headers/CommandProcessingDriver.h"

using namespace std;

int main(int argc, char** argv) {
    //Testing the functionality for Assignment 2.

    cout << "Testing CommandProcessor" << endl;
    testCommandProcessor(argc, argv);


    return 0;
}