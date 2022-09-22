#include "PlayerDriver.cpp"
#include "MapDriver.cpp"
#include "OrdersDriver.cpp"
#include "GameEngineDriver.cpp"
#include "CardsDriver.cpp"


int main() {
    // If you're debugging your stuff, just comment out the others when testing yours

    //Test Maps
    std::cout << "Testing Maps" << std::endl;
    //  testLoadMaps();

    //Test Player
    std::cout << "Testing Player" << std::endl;
    testPlayers();

    //Test OrderList
    std::cout << "Testing OrderList" << std::endl;
    //  testOrdersLists();

    //Test Cards deck/hand
    std::cout << "Testing Cards deck/hand" << std::endl;
    // testCards();

    //Test GameEngine
    std::cout << "Testing GameEngine" << std::endl;
    // testGameStates();
    return 0;
}