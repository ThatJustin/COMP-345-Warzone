#pragma once

#include <string>
#include <vector>
#include "sources/LogObserver/LoggingObserver.h"

class Command  : public ILoggable, public Subject {
public:
    Command();

    explicit Command(const std::string& inputCommand);

    Command(const Command& command);

    ~Command();

    std::string stringToLog() override;

    std::string command;

    std::string effect;

    void saveEffect(const std::string& commandEffect);

    friend std::ostream& operator<<(std::ostream& stream, const Command& command);

    Command& operator=(const Command& com);

    std::string getParam() const;

    std::string getTransitionName() const;
};

class CommandProcessor : public ILoggable, public Subject {
public:
    CommandProcessor();

    CommandProcessor(const CommandProcessor& commandProcessor);

    CommandProcessor(bool isUsingConsole, std::string inputFileName, Observer* obs);

    virtual ~CommandProcessor();

    void attach(Observer* o) override;

    void detach(Observer* o) override;

    std::string stringToLog() override;

    std::vector<Command*> commands;

    bool isUsingConsole;

    std::string inputFileName;

    Command* getCommand(const std::string& currentState);

    friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& commandProcessor);

    CommandProcessor& operator=(const CommandProcessor& commandProcessor);

private:
    virtual std::string readCommand();

    void saveCommand(Command* command);

    bool validate(Command* pCommand, const std::string& currentState);

    Observer* observer;
};

class FileLineReader {
public:
    FileLineReader();

    FileLineReader(const FileLineReader& fileLineReader);

    ~FileLineReader();

    friend std::ostream& operator<<(std::ostream& stream, const FileLineReader& fileLineReader);

    FileLineReader& operator=(const FileLineReader& fileLineReader);

    std::string readLineFromFile(const std::string& fileName);

    int filelinePosition;

    //Stores the positions of the replay commands
    std::vector<int> replayPositions;
};

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    FileCommandProcessorAdapter();

    explicit FileCommandProcessorAdapter(const std::string& inputFileName, Observer* obs);

    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

    ~FileCommandProcessorAdapter() override;

    friend std::ostream&
    operator<<(std::ostream& stream, const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

    FileLineReader* fileLineReader;

private:
    std::string readCommand() override;
};