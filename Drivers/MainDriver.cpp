#include <iostream>
#include "headers/PlayerStrategiesDriver.h"
#include "headers/PlayerDriver.h"
#include "headers/MapDriver.h"
#include "headers/OrdersDriver.h"
#include "headers/GameEngineDriver.h"
#include "headers/CardsDriver.h"
#include "headers/CommandProcessingDriver.h"
#include "headers/LoggingObserverDriver.h"
#include <time.h>
#include <cmath>

using namespace std;

int main(int argc, char** argv) {
    //Testing the functionality for Assignment 3.

    cout << "Testing PlayerStrategiesDriver" << endl;
    testPlayerStrategies(argc, argv);

//    cout << "Testing TournamentDriver" << endl;
//    testTournament(argc, argv);
    return 0;
}