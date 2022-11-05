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
        cout<< "Please run the program using either -console or -file filename arguments." << endl;
        exit(0);
    }
    bool isUsingConsole = true;
    string mapFileName;
    if (argv[1] == "-file"sv) {
        if (argc < 3) {
            cout<< "Please include a file name for this argument like -file filename." << endl;
            exit(0);
        }
        mapFileName = argv[2];
        isUsingConsole = false;
    }

    CommandProcessor* commandProcessor;
    if (isUsingConsole) {
        commandProcessor  = new CommandProcessor(isUsingConsole, mapFileName);
    } else {
        commandProcessor = new FileCommandProcessorAdapter(mapFileName);
    }
    //TODO for testing, I will emulate the actual code here and not call the GameEngine
    GameEngine* gameEngine = new GameEngine();
    gameEngine->setCommandProcessor(commandProcessor);

    gameEngine->startupPhase();

    delete commandProcessor;
}