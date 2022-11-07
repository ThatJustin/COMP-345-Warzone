#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Drivers/headers/CommandProcessingDriver.h"
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"

using namespace std::literals;

void testCommandProcessor(int argc, char** argv) {
    if (argc < 2 || (argv[1] != "-console"sv && argv[1] != "-file"sv)) {
        cout << "Please run the program using either -console or -file filename arguments." << endl;
        exit(0);
    }
    bool isUsingConsole = true;
    string mapFileName;
    if (argv[1] == "-file"sv) {
        if (argc < 3) {
            cout << "Please include a file name for this argument like -file filename." << endl;
            exit(0);
        }
        mapFileName = argv[2];
        isUsingConsole = false;
    }

    CommandProcessor* commandProcessor;
    if (isUsingConsole) {
        commandProcessor = new CommandProcessor(isUsingConsole, mapFileName);
    } else {
        commandProcessor = new FileCommandProcessorAdapter(mapFileName);
    }
    GameEngine* gameEngine = new GameEngine();
    gameEngine->setCommandProcessor(commandProcessor);
    gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::StartGame);
    cout << "Entering StartGame state." << endl;

    while (true) {
        string curStateName = gameEngine->getCurrentGameState()->name;
        Command* c = commandProcessor->getCommand(curStateName);
        if (c == nullptr) {
            cout << "This command is not valid for this state." << endl;
            continue;
        }
        gameEngine->commandTransitionName = c->getTransitionName();
        if (c->getTransitionName() == "loadmap") {
            gameEngine->commandParam = c->getParam();
            gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::LoadMap);
            cout << "Entering Loadmap state." << endl;
        } else if (c->getTransitionName() == "validatemap") {
            gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::ValidateMap);
            cout << "Entering ValidateMap state." << endl;
        } else if (c->getTransitionName() == "addplayer") {
            gameEngine->commandParam = c->getParam();
            gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::AddPlayer);
            cout << "Entering AddPlayer state." << endl;
        } else if (c->getTransitionName() == "gamestart") {
            cout << "Skipping main game loop for demo. Simulating a win transition.\r\n" << endl;
            gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::Win);
            cout << "Entering Win state." << endl;
        } else if (c->getTransitionName() == "replay") {
            gameEngine->prepareForReplay();
            gameEngine->currentGameState = gameEngine->getStateFromTransition(GameEngine::Play);
            cout << "Entering replay state." << endl;
        } else if (c->getTransitionName() == "quit") {
            cout << "Thanks for playing!" << endl;
            break;
        }
    }
    delete gameEngine;
}