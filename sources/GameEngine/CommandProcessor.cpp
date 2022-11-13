#include <iostream>
#include <utility>
#include <filesystem>
#include <fstream>
#include "CommandProcessor.h"
#include <algorithm>

using namespace std;

/**
 * Default constructor.
 */
Command::Command() {
    this->command = "";
}

/**
 * Parameterized constructor.
 * @param inputCommand
 */
Command::Command(const string& inputCommand) {
    this->command = inputCommand;
}

/**
 * Destructor
 */
Command::~Command() {}


/**
 * Saves the effect command and notifies the observer.
 * @param commandEffect
 */
void Command::saveEffect(const std::string& commandEffect) {
    this->effect = commandEffect;
    notify(this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param command
 * @return
 */
std::ostream& operator<<(ostream& stream, const Command& command) {
    stream << "Name - " << command.command << endl;
    stream << "Effect - " << command.effect << endl;
    return stream;
}

/**
 * Returns the parameter part of the command.
 * @return
 */
std::string Command::getParam() const {
    std::string param;
    if (command.find_first_of(' ') != string::npos) {
        param = command.substr(command.find_first_of(" \t") + 1);
    }
    return param;
}

/**
 * Returns the transition name part of the command.
 * @return
 */
std::string Command::getTransitionName() const {
    return command.substr(0, command.find(' '));
}

/**
 * Assignment operator overload for the Command class
 * @param com
 * @return
 */
Command& Command::operator=(const Command& com) {
    if (this == &com) {
        return *this;
    }
    this->command = com.command;
    this->effect = com.effect;
    return *this;
}

/**
 * Copy Constructor.
 * @param command
 */
Command::Command(const Command& command) {
    this->command = command.command;
    this->effect = command.effect;
}

string Command::stringToLog() {
    return "[Command Effect] " + this->effect;
}

/**
 * Default constructor.
 */
CommandProcessor::CommandProcessor() {
    this->inputFileName = "";
    this->isUsingConsole = false;
    this->commands = vector<Command*>();
}

/**
 * Parameterized constructor.
 * @param isUsingConsole
 * @param inputFileName
 */
CommandProcessor::CommandProcessor(bool isUsingConsole, string inputFileName, Observer* obs) {
    this->isUsingConsole = isUsingConsole;
    this->inputFileName = std::move(inputFileName);
    this->commands = vector<Command*>();
    attach(obs);
}

/**
 * Destructor.
 */
CommandProcessor::~CommandProcessor() {
    for (auto cmd: this->commands) {
        cmd->detach(observer);
        delete cmd;
        cmd = nullptr;
    }
    this->commands.clear();
    detach(observer);
}


string CommandProcessor::stringToLog() {
    return "[CommandProcessor] Command [" + this->commands.back()->command + "] has been saved to command collection. ";
}

/**
 * Reads input as the command and returns it.
 * @return
 */
string CommandProcessor::readCommand() {
    string command;
    cout << "Please enter a command: " << endl;
    //TODO maybe show valid commands in this state
    cin >> std::ws;
    getline(cin, command);

    return command;
}

/**
 * Saves the command to the command vector.
 * @param command
 */
void CommandProcessor::saveCommand(Command* command) {
    this->commands.push_back(command);
    notify(this);
}

/**
 * Gets the next command read from user input, saves it and returns the command if it's valid.
 * @param currentState
 * @return
 */
Command* CommandProcessor::getCommand(const string& currentState) {
    string inputCommand = readCommand();
    Command* command = new Command(inputCommand);
    command->attach(observer);
    saveCommand(command);
    return validate(command, currentState) == 0 ? nullptr : command;
}

/**
 * Stream data for CommandProcessor class.
 * @param stream
 * @param commandProcessor
 * @return
 */
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
 * @param pCommand command being used.
 * @param currentState current state of the game.
 * @return true if it's valid, false otherwise
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
            pCommand->saveEffect("[Invalid Command] Missing player name for addplayer");
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
        pCommand->saveEffect("[Valid Command] " + com + " used in valid state " + currentState + ".");
        return true;
    }
    pCommand->saveEffect("[Invalid Command] Tried to use command " + com + " while in state " + currentState + ".");
    return false;
}

/**
 * Assignment operator override.
 * @param commandProcessor
 * @return
 */
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor) {
    if (this == &commandProcessor) {
        return *this;
    }
    this->commands = commandProcessor.commands;
    this->inputFileName = commandProcessor.inputFileName;
    this->isUsingConsole = commandProcessor.isUsingConsole;
    return *this;
}

/**
 * Copy constructor.
 * @param commandProcessor
 */
CommandProcessor::CommandProcessor(const CommandProcessor& commandProcessor) {
    this->commands = commandProcessor.commands;
    this->inputFileName = commandProcessor.inputFileName;
    this->isUsingConsole = commandProcessor.isUsingConsole;
}

void CommandProcessor::attach(Observer* obs) {
    Subject::attach(obs);
    this->observer = obs;
}

void CommandProcessor::detach(Observer* obs) {
    Subject::detach(obs);
    this->observer = nullptr;
}

/**
 * Default constructor.
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
    this->fileLineReader = new FileLineReader();
    this->inputFileName = "";
    this->isUsingConsole = false;
}

/**
 * Parameterized Constructor.
 * @param inputFileName
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string& inputFileName, Observer* obs)
        : CommandProcessor(false, inputFileName, obs) {
    this->fileLineReader = new FileLineReader();
}

/**
 * Destructor.
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileLineReader;
}

/**
 * Reads the next line from a file.
 * @return
 */
string FileCommandProcessorAdapter::readCommand() {
    std::string command = fileLineReader->readLineFromFile(inputFileName);
    return command;
}

/**
 *
 * @param fileCommandProcessorAdapter
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(
        const FileCommandProcessorAdapter& fileCommandProcessorAdapter) : CommandProcessor(
        fileCommandProcessorAdapter) {
    this->inputFileName = fileCommandProcessorAdapter.inputFileName;
    this->fileLineReader = new FileLineReader(*fileCommandProcessorAdapter.fileLineReader);
    this->isUsingConsole = fileCommandProcessorAdapter.isUsingConsole;
}

std::ostream& operator<<(ostream& stream, const FileCommandProcessorAdapter& fileCommandProcessorAdapter) {
    stream << "File Name: " << fileCommandProcessorAdapter.inputFileName;
    stream << "Console: " << fileCommandProcessorAdapter.isUsingConsole;
    return stream;
}

/**
 * Assignment operator override.
 * @param fileCommandProcessorAdapter
 * @return
 */
FileCommandProcessorAdapter&
FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& fileCommandProcessorAdapter) {
    if (this == &fileCommandProcessorAdapter) {
        return *this;
    }
    this->inputFileName = fileCommandProcessorAdapter.inputFileName;
    this->fileLineReader = new FileLineReader(*fileCommandProcessorAdapter.fileLineReader);
    this->isUsingConsole = fileCommandProcessorAdapter.isUsingConsole;
    return *this;
}

/**
 * Parses the input file and reads the next line to be read.
 * @param fileName
 * @return
 */
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

/**
 * Default constructor,
 */
FileLineReader::FileLineReader() {
    filelinePosition = 1;
}

/**
 * Destructor.
 */
FileLineReader::~FileLineReader() {
    this->replayPositions.clear();
}

/**
 * Copy constructor.
 * @param fileLineReader
 */
FileLineReader::FileLineReader(const FileLineReader& fileLineReader) {
    this->filelinePosition = fileLineReader.filelinePosition;
    this->replayPositions = fileLineReader.replayPositions;
}

/**
 * Stream data for the class.
 * @param stream
 * @param fileLineReader
 * @return
 */
std::ostream& operator<<(ostream& stream, const FileLineReader& fileLineReader) {
    stream << "Line Position: " << fileLineReader.filelinePosition;
    stream << "Positions in file of replay commands: ";
    for (auto pos: fileLineReader.replayPositions) {
        stream << pos << " ";
    }
    return stream;
}

/**
 * Assignment operator overload for the FileLineReader class.
 * @param fileLineReader
 * @return
 */
FileLineReader& FileLineReader::operator=(const FileLineReader& fileLineReader) {
    if (this == &fileLineReader) {
        return *this;
    }
    this->filelinePosition = fileLineReader.filelinePosition;
    this->replayPositions = fileLineReader.replayPositions;
    return *this;
}
