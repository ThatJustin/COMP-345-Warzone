//
// Created by Amir Sadeqi on 2022-11-28.
//


#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "Drivers/headers/TournamentDriver.h"
#include "Drivers/headers/CommandProcessingDriver.h"
#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
using namespace std::literals;
using std::cout;
using std::endl;
using std::string;
using std::vector;
#include <algorithm>
void testTournament(int argc, char** argv) {
    if (argc < 2 || (argv[1] != "-console"sv && argv[1] != "-file"sv )) {
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
    LogObserver* logObserver = new LogObserver();
    CommandProcessor* commandProcessor;
    if (isUsingConsole) {
        commandProcessor = new CommandProcessor(isUsingConsole, mapFileName, logObserver);
    } else {
        commandProcessor = new FileCommandProcessorAdapter(mapFileName, logObserver);
    }

    GameEngine* gameEngine = new GameEngine(logObserver);
    gameEngine->setCommandProcessor(commandProcessor);
    gameEngine->changeStateByTransition(GameEngine::StartGame);
    int index= 0;
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
            gameEngine->changeStateByTransition(GameEngine::LoadMap);
        } else if (c->getTransitionName() == "validatemap") {
            gameEngine->changeStateByTransition(GameEngine::ValidateMap);
        } else if (c->getTransitionName() == "addplayer") {
            gameEngine->commandParam = c->getParam();
            gameEngine->changeStateByTransition(GameEngine::AddPlayer);
        } else if (c->getTransitionName() == "gamestart") {
            gameEngine->gameStart();
            cout << "Displaying the outcome of gameStart being called." << endl;
            for (auto p: gameEngine->getGamePlayers()) {
                cout << *p << endl;
            }
            cout << "Simulating a win transition for demo purpose." << endl;
            gameEngine->changeStateByTransition(GameEngine::Win);
        } else if (c->getTransitionName() == "replay") {
            gameEngine->prepareForReplay();
            gameEngine->changeStateByTransition(GameEngine::Play);
        }else if(c->getTransitionName() == "tournament"){
            cout << "tournament command received" << endl;
            string ListOfMapFiles;
            string ListOfPlayerStrategies;
            int NumberOfGames = 0;
            int MaxNumberOfTurns = 0;

            vector<string> Args;
            vector<string> In_Args;
            string arguments = c->getParam();
            string argument;
            string In_argument;
            std::stringstream ArgsStream(arguments);
            while (std::getline(ArgsStream, argument, '-')) {
                std::stringstream InArgsStream(argument);
                while (std::getline(InArgsStream, In_argument, ' ')) {
                    In_Args.push_back(In_argument);
                }
            }
            auto it_M = find(In_Args.begin(), In_Args.end(), "M");
            if (it_M != In_Args.end()) {
                int in_arg_index = -1;
                in_arg_index = it_M - In_Args.begin();
                ListOfMapFiles = In_Args[in_arg_index+1];
            }


            auto it_P = find(In_Args.begin(), In_Args.end(), "P");
            if (it_P != In_Args.end()) {
                int in_arg_index = -1;
                in_arg_index = it_P - In_Args.begin();
                ListOfPlayerStrategies = In_Args[in_arg_index+1];
            }

            auto it_G = find(In_Args.begin(), In_Args.end(), "G");
            if (it_G != In_Args.end()) {
                int in_arg_index = -1;
                in_arg_index = it_G - In_Args.begin();
                NumberOfGames = stoi(In_Args[in_arg_index+1]);
            }

            auto it_D = find(In_Args.begin(), In_Args.end(), "D");
            if (it_D != In_Args.end()) {
                int in_arg_index = -1;
                in_arg_index = it_D - In_Args.begin();
                MaxNumberOfTurns = stoi(In_Args[in_arg_index+1]);
            }
            cout << "initializing tournament" << endl;
            cout << MaxNumberOfTurns << endl;
            gameEngine->initializeTournament(ListOfMapFiles, ListOfPlayerStrategies, NumberOfGames, MaxNumberOfTurns);

            gameEngine->setCommandProcessor(commandProcessor);
            gameEngine->changeStateByTransition(GameEngine::StartGame);

        } else if (c->getTransitionName() == "quit") {
            index++;
            if(index != gameEngine->tournamentNumberOfGames-1){
                cout << "playing Tournaments/map"+std::to_string(index) << endl;
                commandProcessor = new FileCommandProcessorAdapter("Tournaments/map"+std::to_string(index), logObserver);
                gameEngine->setCommandProcessor(commandProcessor);
                gameEngine->tournamentMapName = "map"+std::to_string(index);
            }else{
                cout << "Thanks for playing!" << endl;
                break;
            }
        }
    }
    delete gameEngine;
}