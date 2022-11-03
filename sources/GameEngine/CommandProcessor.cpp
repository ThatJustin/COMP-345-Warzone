#include <iostream>
#include <utility>
#include "CommandProcessor.h"

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
    cout << "Enter command: " << endl;
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
    string com = pCommand->command;
    bool success = false;
    // Check if the transition is valid from the current state
    if (com == "loadmap") {
        if (currentState == "start" || currentState == "maploaded") {
            success = true;
        }
    } else if (com == "validatemap") {
        if (currentState == "maploaded") {
            success = true;
        }
    } else if (com == "addplayer") {
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
        pCommand->saveEffect("[VALID COMMAND]" + com + " used in valid state " + currentState);
        return true;
    }
    pCommand->saveEffect("[INVALID COMMAND] Tried to use command " + com + " while in state " + currentState);
    return false;
}