#pragma once

#include <string>
#include "sources/Player/Player.h"
#include "sources/Orders/Orders.h"

using namespace std;

//for part 3
class Map;
class OrdersList;

class GameState;

class Start;

class MapLoaded;

class MapValidated;

class PlayersAdded;

class AssignReinforcement;

class IssueOrders;

class ExecuteOrders;

class Win;

class CommandProcessor;

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

    CommandProcessor* commandProcessor;
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
            GameStart = 4,
            IssueOrder = 5,
            IssueOrdersEnd = 6,
            Execorder = 7,
            Endexecorders = 8,
            Win = 9,
            Play = 10;

    //Handles changing states by a transition
    void changeStateByTransition(int transition);

    GameState* getStateFromTransition(int transition);

    void setCommandProcessor(CommandProcessor* commandProcessor);

    void startupPhase();

    void mainGameLoop();

    std::vector<Player*> players;

    std::vector<Territory*> territories;
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

    ~AssignReinforcement() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const AssignReinforcement& assignReinforcement);

    AssignReinforcement& operator=(const AssignReinforcement& assignReinforcement);

    //for maingameloop
    std::vector<Player*> players;

    //Map
    Map* map = NULL;

    int controlbonus;//need to privatise

};

class IssueOrders : public GameState {
private:
public:

    explicit IssueOrders(GameEngine* gameEngine);

    IssueOrders(const IssueOrders& issueOrders);

    ~IssueOrders() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const IssueOrders& issueOrders);

    IssueOrders& operator=(const IssueOrders& issueOrders);

    //for maingameloop
    std::vector<Player*> players;

    Map* map = NULL;

    Deck* deck = NULL;

    Hand* hand = NULL;
};

class ExecuteOrders : public GameState {
private:
public:

    explicit ExecuteOrders(GameEngine* gameEngine);

    ExecuteOrders(const ExecuteOrders& executeOrders);

    ~ExecuteOrders() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const ExecuteOrders& executeOrders);

    ExecuteOrders& operator=(const ExecuteOrders& executeOrders);

    std::vector<Player*> players;
};

class Win : public GameState {
private:
public:

    explicit Win(GameEngine* gameEngine);

    Win(const Win& win);

    ~Win() override;

    void enterState() override;

    bool isValidTransition() override;

    GameState* copy() override;

    friend ostream& operator<<(ostream& stream, const Win& win);

    Win& operator=(const Win& win);
};