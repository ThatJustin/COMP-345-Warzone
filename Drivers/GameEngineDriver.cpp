
#include "sources/GameEngine/GameEngine.h"
#include <iostream>
#include <string>

void testGameStates() {

    bool flag = false;
    GameEngine* GE = new GameEngine;

    while (!flag) {
        printf("currect state:");
        std::cout << GE->getState()->Name;
        printf("\n");
        GE->Handle();
        printf("\n");
        if (GE->getState()->Name == "End") {
            flag = true;
        }
    }
    delete GE;
}

int main() {
    testGameStates();
}
