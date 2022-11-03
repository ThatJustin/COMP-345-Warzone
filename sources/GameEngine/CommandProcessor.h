#pragma once


#include <string>
#include <vector>

class Command {
public:
    explicit Command(const std::string& inputCommand);

    ~Command();

    std::string command;
    std::string effect;

    void saveEffect(const std::string& commandEffect);

    friend std::ostream& operator<<(std::ostream& stream, const Command& command);

    Command& operator=(const Command& command);


};

class CommandProcessor {

public:
    CommandProcessor();

    CommandProcessor(bool isUsingConsole, std::string  inputFileName);

    explicit CommandProcessor(const std::vector<Command*>& commands);

    ~CommandProcessor();

    std::vector<Command*> commands;
    bool isUsingConsole;
    std::string inputFileName;

    Command* getCommand(const std::string& currentState);

    friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& commandProcessor);

    CommandProcessor& operator=(const CommandProcessor& commandProcessor);

private:
    std::string readCommand();

    void saveCommand(Command* command);

    bool validate(Command* pCommand, const std::string& currentState);
};

