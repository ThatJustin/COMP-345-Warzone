#include <iostream>
#include "headers/PlayerDriver.h"
#include "headers/MapDriver.h"
#include "headers/OrdersDriver.h"
#include "headers/GameEngineDriver.h"
#include "headers/CardsDriver.h"
#include "headers/CommandProcessingDriver.h"
#include "headers/LoggingObserverDriver.h"

using namespace std;

int main(int argc, char** argv) {
    //Testing the functionality for Assignment 2.

//    cout << "Testing CommandProcessor" << endl;
//    testCommandProcessor(argc, argv);

    //cout << "Testing GameEngineDriver StartupPhase" << endl;
    //testStartupPhase(argc, argv);


    cout<<"Testing LoggingObserverDriver:\n"<<endl;
    testLoggingObserver();
    return 0;
}