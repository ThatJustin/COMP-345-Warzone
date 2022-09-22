
#ifndef COMP_345_WARZONE_GAMEENGINE_H
#define COMP_345_WARZONE_GAMEENGINE_H

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
    friend ostream& operator<<(ostream& stream, const GameEngine &player);
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

    void changeStateByTransition(int transition);

    GameState* getStateFromTransition(int transition);
};

class GameState {
public:
    GameEngine* gameEngine;

    explicit GameState(GameEngine* gameEngine);

    //Name of the state in case we need it
    std::string name;

    //Virtual functions to override
    virtual void enterState() = 0;

    virtual bool isValidTransition();

    ~GameState();

};

class Start : public GameState {
private:
public:

    explicit Start(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[1] = {GameEngine::LoadMap};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[1]{"loadmap"};

    ~Start();

    void enterState() override;

    bool isValidTransition() override;
};

class MapLoaded : public GameState {
private:
public:

    explicit MapLoaded(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::LoadMap, GameEngine::ValidateMap};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"loadmap", "validatemap"};

    ~MapLoaded();

    void enterState() override;

    bool isValidTransition() override;
};

class MapValidated : public GameState {
private:
public:

    explicit MapValidated(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[1] = {GameEngine::AddPlayer};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[1]{"addplayer"};

    ~MapValidated();

    void enterState() override;

    bool isValidTransition() override;
};

class PlayersAdded : public GameState {
private:
public:

    explicit PlayersAdded(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::AddPlayer, GameEngine::AssignCountries};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"addplayer", "assigncountries"};

    ~PlayersAdded();

    void enterState() override;

    bool isValidTransition() override;
};

class AssignReinforcement : public GameState {
private:
public:

    explicit AssignReinforcement(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::IssueOrder};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"issueorders"};

    ~AssignReinforcement();

    void enterState() override;

    bool isValidTransition() override;
};

class IssueOrders : public GameState {
private:
public:

    explicit IssueOrders(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::IssueOrder, GameEngine::EndIssueOrders};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"issueorders", "endissueorders"};

    ~IssueOrders();

    void enterState() override;

    bool isValidTransition() override;
};

class ExecuteOrders : public GameState {
private:
public:

    explicit ExecuteOrders(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[3] = {GameEngine::Execorder, GameEngine::Endexecorders, GameEngine::Win};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[3]{"execorder", "endexecorders", "win"};

    ~ExecuteOrders();

    void enterState() override;

    bool isValidTransition() override;
};

class Win : public GameState {
private:
public:

    explicit Win(GameEngine* gameEngine);

    //These are the valid transitions from this state
    const int VALID_TRANSITIONS[2] = {GameEngine::Play, GameEngine::End};
    //These are the valid commands (input) for this state
    const string VALID_COMMANDS[2]{"play", "end"};

    ~Win();

    void enterState() override;

    bool isValidTransition() override;
};

#endif //COMP_345_WARZONE_GAMEENGINE_H
