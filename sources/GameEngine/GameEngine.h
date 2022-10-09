#pragma once

#include <string>

using namespace std;

class GameState;

class Start;

class MapLoaded;

class MapValidated;

class PlayersAdded;

class AssignReinforcement;

class IssueOrders;

class ExecuteOrders;

class Win;


class GameEngine {
private:

    //each state
    Start* start;
    MapLoaded* loadMap;
    MapValidated* mapValidated;
    PlayersAdded* playersAdded;
    AssignReinforcement* assignReinforcement;
    IssueOrders* issueOrders;
    ExecuteOrders* executeOrders;
    Win* win;

    // Keep track of the current state
    GameState* currentGameState;
public:
    GameEngine();

    GameEngine(const GameEngine& gameEngine);

    ~GameEngine();

    friend ostream& operator<<(ostream& stream, const GameEngine& player);

    GameEngine& operator=(const GameEngine& gameEngine);

    GameState* getCurrentGameState();

    //All valid transitions
    static const int StartGame = 0,
            LoadMap = 1,
            ValidateMap = 2,
            AddPlayer = 3,
            AssignCountries = 4,
            IssueOrder = 5,
            EndIssueOrders = 6,
            Execorder = 7,
            Endexecorders = 8,
            Win = 9,
            Play = 10,
            End = 11;

    //Handles changing states by a transition
    void changeStateByTransition(int transition);

    GameState* getStateFromTransition(int transition);
};

class GameState {
public:
    GameEngine* gameEngine;

    GameState();

    explicit GameState(GameEngine* gameEngine);

    GameState(const GameState& gameState);

    //Name of the state in case we need it
    std::string name;

    //Virtual functions to override
    virtual void enterState() = 0;

    virtual GameState* copy() = 0;

    virtual bool isValidTransition();

    virtual ~GameState();

    GameState& operator=(const GameState& gameState);
};

class Start : public GameState {
private:
public:
    explicit Start(GameEngine* gameEngine);

    Start(const Start& start);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[1] = {GameEngine::LoadMap};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[1]{"loadmap"};

    ~Start() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const Start& start);

    Start& operator=(const Start& start);
};

class MapLoaded : public GameState {
private:
public:

    explicit MapLoaded(GameEngine* gameEngine);

    MapLoaded(const MapLoaded& mapLoaded);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::LoadMap, GameEngine::ValidateMap};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"loadmap", "validatemap"};

    ~MapLoaded() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const MapLoaded& mapLoaded);

    MapLoaded& operator=(const MapLoaded& mapLoaded);
};

class MapValidated : public GameState {
private:
public:

    explicit MapValidated(GameEngine* gameEngine);

    MapValidated(const MapValidated& mapValidated);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[1] = {GameEngine::AddPlayer};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[1]{"addplayer"};

    ~MapValidated() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const MapValidated& mapValidated);

    MapValidated& operator=(const MapValidated& mapValidated);
};

class PlayersAdded : public GameState {
private:
public:

    explicit PlayersAdded(GameEngine* gameEngine);

    PlayersAdded(const PlayersAdded& playersAdded);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::AddPlayer, GameEngine::AssignCountries};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"addplayer", "assigncountries"};

    ~PlayersAdded() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const PlayersAdded& playersAdded);

    PlayersAdded& operator=(const PlayersAdded& playersAdded);
};

class AssignReinforcement : public GameState {
private:
public:

    explicit AssignReinforcement(GameEngine* gameEngine);

    AssignReinforcement(const AssignReinforcement& assignReinforcement);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[1] = {GameEngine::IssueOrder};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[1]{"issueorders"};

    ~AssignReinforcement() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const AssignReinforcement& assignReinforcement);

    AssignReinforcement& operator=(const AssignReinforcement& assignReinforcement);
};

class IssueOrders : public GameState {
private:
public:

    explicit IssueOrders(GameEngine* gameEngine);

    IssueOrders(const IssueOrders& issueOrders);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::IssueOrder, GameEngine::EndIssueOrders};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"issueorders", "endissueorders"};

    ~IssueOrders() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const IssueOrders& issueOrders);

    IssueOrders& operator=(const IssueOrders& issueOrders);
};

class ExecuteOrders : public GameState {
private:
public:

    explicit ExecuteOrders(GameEngine* gameEngine);

    ExecuteOrders(const ExecuteOrders& executeOrders);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[3] = {GameEngine::Execorder, GameEngine::Endexecorders, GameEngine::Win};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[3]{"execorder", "endexecorders", "win"};

    ~ExecuteOrders() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const ExecuteOrders& executeOrders);

    ExecuteOrders& operator=(const ExecuteOrders& executeOrders);
};

class Win : public GameState {
private:
public:

    explicit Win(GameEngine* gameEngine);

    Win(const Win& win);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::Play, GameEngine::End};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"play", "end"};

    ~Win() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const Win& win);

    Win& operator=(const Win& win);
};