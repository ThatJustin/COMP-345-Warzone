#include "sources/Game Engine/GameEngine.h"

void testGameStates() {
    GameEngine* gameEngine = new GameEngine;

    // Show the transitions for demo by going to the start game first
    gameEngine->changeStateByTransition(GameEngine::StartGame);

    delete gameEngine;
}
