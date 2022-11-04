#include <iostream>
#include "sources/GameEngine/GameEngine.h"

void testGameStates() {
    GameEngine* gameEngine = new GameEngine;

    // Show the transitions for demo by going to the start game first
    gameEngine->changeStateByTransition(GameEngine::StartGame);
    delete gameEngine;
}

//free function for the main game loop
void testMainGameLoop(){

    //add test cases in a round robin fashion by part 2
    //assignReinforcement();

    //executeOrdersPhase();

    //issueOrdersPhase();

    cout<<"The winner is: addwinnername variable" << endl;
}
