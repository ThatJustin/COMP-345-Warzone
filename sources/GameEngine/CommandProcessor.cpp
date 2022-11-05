#include <iostream>
#include <utility>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "CommandProcessor.h"
#include <algorithm>

using namespace std;

Command::Command(const string& inputCommand) {
    this->command = inputCommand;
}

Command::~Command() {

}

void Command::saveEffect(const std::string& commandEffect) {
    this->effect = commandEffect;
    //Might need to notify observer in part 5
}

std::ostream& operator<<(ostream& stream, const Command& command) {
    stream << "Name - " << command.command << endl;
    stream << "Effect - " << command.effect << endl;
    return stream;
}

std::string Command::getParam() {
    std::string param;
    if (command.find_first_of(' ') != string::npos) {
        param = command.substr(command.find_first_of(" \t") + 1);
    }
    return param;
}

std::string Command::getTransitionName() {
    return command.substr(0, command.find(' '));
}

CommandProcessor::CommandProcessor() {
    this->inputFileName = "";
    this->isUsingConsole = false;
    this->commands = vector<Command*>();
}

CommandProcessor::CommandProcessor(bool isUsingConsole, string inputFileName) {
    this->isUsingConsole = isUsingConsole;
    this->inputFileName = std::move(inputFileName);
    this->commands = vector<Command*>();
}

CommandProcessor::~CommandProcessor() {
    for (auto cmd: this->commands) {
        delete cmd;
    }
}

CommandProcessor::CommandProcessor(const std::vector<Command*>& commands) : commands(commands) {

}

string CommandProcessor::readCommand() {
    string command;
    cout << "Please enter a command: " << endl;
    //TODO maybe show valid commands in this state
    cin >> std::ws;
    getline(cin, command);

    return command;
}

void CommandProcessor::saveCommand(Command* command) {
    this->commands.push_back(command);
}

Command* CommandProcessor::getCommand(const string& currentState) {
    string inputCommand = readCommand();
    Command* command = new Command(inputCommand);
    bool isValidTransition = validate(command, currentState);
    saveCommand(command);
    return isValidTransition == 0 ? nullptr : command;
}

std::ostream& operator<<(ostream& stream, const CommandProcessor& commandProcessor) {
    stream << "Using console : ";
    if (commandProcessor.isUsingConsole == 0) {
        stream << "false";
    } else {
        stream << "true";
    }
    stream << "    Input File Name : " << commandProcessor.inputFileName << endl;
    stream << "CommandProcessor contains these commands: " << endl;
    for (auto cmd: commandProcessor.commands) {
        stream << *cmd << endl;
    }
    return stream;
}

/**
 * Validates if the command is valid in the current state.
 * @param pCommand
 * @param currentState
 * @return
 */
bool CommandProcessor::validate(Command* pCommand, const std::string& currentState) {
    string com = pCommand->getTransitionName();
    bool success = false;
    // Check if the transition is valid from the current state
    if (com == "loadmap") {
        if (pCommand->getParam().empty()) {
            pCommand->saveEffect("[INVALID COMMAND] Missing map name for loadmap.");
            return false;
        }
        if (currentState == "start" || currentState == "maploaded") {
            success = true;
        }
    } else if (com == "validatemap") {
        if (currentState == "maploaded") {
            success = true;
        }
    } else if (com == "addplayer") {
        if (pCommand->getParam().empty()) {
            pCommand->saveEffect("[INVALID COMMAND] Missing player name for addplayer");
            return false;
        }
        if (currentState == "mapvalidated" || currentState == "playersadded") {
            success = true;
        }
    } else if (com == "gamestart") {
        if (currentState == "playersadded") {
            success = true;
        }
    } else if (com == "replay" || com == "quit") {
        if (currentState == "win") {
            success = true;
        }
    }
    if (success) {
        pCommand->saveEffect("[VALID COMMAND] " + com + " used in valid state " + currentState + ".");
        return true;
    }
    pCommand->saveEffect("[INVALID COMMAND] Tried to use command " + com + " while in state " + currentState + ".");
    return false;
}

string FileCommandProcessorAdapter::readCommand() {
    std::string command = fileLineReader->readLineFromFile(inputFileName);
    return command;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string& inputFileName)
        : CommandProcessor(false, inputFileName) {
    this->fileLineReader = new FileLineReader();
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileLineReader;
}

string FileLineReader::readLineFromFile(const std::string& fileName) {

    std::string path = std::filesystem::current_path().string() + "\\Games\\" + fileName;
    std::ifstream file(path);
    if (!file.is_open()) {
        cout << "Game with file name " << fileName << " doesn't exist." << endl;
        exit(-1);
    }
    std::string lineText;
    skipReplayEmpty:
    //find the text on the specific line
    for (int i = 1; i <= filelinePosition; i++) {
        if (std::find(replayPositions.begin(), replayPositions.end(), filelinePosition) == replayPositions.end()) {
            std::getline(file, lineText);
        }
    }

    filelinePosition++;
    // if it's empty, it most of been a replay we already did, skip it
    if (lineText.empty()) {
        goto skipReplayEmpty;
    }
    return lineText;
}

FileLineReader::FileLineReader() {
    filelinePosition = 1;
}

FileLineReader::~FileLineReader() {}
