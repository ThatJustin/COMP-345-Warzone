#include "GameEngine.h"
#include <iostream>
#include <string>
#include <algorithm>

/**
 * Constructor of GameEngine
 */
GameEngine::GameEngine() {

    //Create the states which will be used throughout the game
    start = new Start(this);
    loadMap = new MapLoaded(this);
    mapValidated = new MapValidated(this);
    playersAdded = new PlayersAdded(this);
    assignReinforcement = new AssignReinforcement(this);
    issueOrders = new IssueOrders(this);
    executeOrders = new ExecuteOrders(this);
    win = new class Win(this);

    //Keep track of the current game state
    currentGameState = nullptr;
}

/**
 * Destructor of GameEngine
 */
GameEngine::~GameEngine() {
    if (start != nullptr) {
        delete start;
        start = nullptr;
    }
    if (loadMap != nullptr) {
        delete loadMap;
        loadMap = nullptr;
    }
    if (mapValidated != nullptr) {
        delete mapValidated;
        mapValidated = nullptr;
    }
    if (playersAdded != nullptr) {
        delete playersAdded;
        playersAdded = nullptr;
    }
    if (assignReinforcement != nullptr) {
        delete assignReinforcement;
        assignReinforcement = nullptr;
    }
    if (issueOrders != nullptr) {
        delete issueOrders;
        issueOrders = nullptr;
    }
    if (executeOrders != nullptr) {
        delete executeOrders;
        executeOrders = nullptr;
    }
    if (win != nullptr) {
        delete win;
        win = nullptr;
    }
    if (currentGameState != nullptr) {
        currentGameState = nullptr;
    }
}

/**
 * Changes the state of the game based on the transitionn given.hanging state for the game.
 * @param transition
 */
void GameEngine::changeStateByTransition(int transition) {
    //First time running the game
    if (currentGameState == nullptr) {
        this->currentGameState = getStateFromTransition(transition);
        this->getCurrentGameState()->enterState();
    } else {
        // isValidTransition will be used for when we need to validate that the transition is okay in the future
        // for now it's always true
        if (this->getCurrentGameState()->isValidTransition()) {
            this->currentGameState = getStateFromTransition(transition);
            this->getCurrentGameState()->enterState();
        }
    }
}

/**
 * Returns the state of the game engine.
 * @return currentGameState
 */
GameState* GameEngine::getCurrentGameState() {
    return currentGameState;
}

/**
 * Returns the state after a given transition.
 * @param transition
 * @return state
 */
GameState* GameEngine::getStateFromTransition(int transition) {
    if (transition == StartGame || transition == Play) {
        return start;
    } else if (transition == LoadMap) {
        return loadMap;
    } else if (transition == ValidateMap) {
        return mapValidated;
    } else if (transition == AddPlayer) {
        return playersAdded;
    } else if (transition == AssignCountries || transition == Endexecorders) {
        return assignReinforcement;
    } else if (transition == IssueOrder) {
        return issueOrders;
    } else if (transition == EndIssueOrders || transition == Execorder) {
        return executeOrders;
    } else if (transition == Win) {
        return win;
    } else {
        cout << "An invalid transition has been encountered." << endl;
        return nullptr;
    }
}

/**
 * Copy constructor
 */
GameEngine::GameEngine(const GameEngine& gameEngine) {
    start = dynamic_cast<Start*>(gameEngine.start->copy());
    loadMap = dynamic_cast<MapLoaded*>(gameEngine.loadMap->copy());
    mapValidated = dynamic_cast<MapValidated*>(gameEngine.mapValidated->copy());
    playersAdded = dynamic_cast<PlayersAdded*>(gameEngine.playersAdded->copy());
    assignReinforcement = dynamic_cast<AssignReinforcement*>(gameEngine.assignReinforcement->copy());
    issueOrders = dynamic_cast<IssueOrders*>(gameEngine.issueOrders->copy());
    executeOrders = dynamic_cast<ExecuteOrders*>(gameEngine.executeOrders->copy());
    win = dynamic_cast<class Win*>(gameEngine.win->copy());
    currentGameState = gameEngine.currentGameState->copy();
}

/**
 * Override the = operator for GameEngine.
 */
GameEngine& GameEngine::operator=(const GameEngine& gameEngine) {
    if (this == &gameEngine) {
        return *this;
    }
    this->start = gameEngine.start;
    this->loadMap = gameEngine.loadMap;
    this->mapValidated = gameEngine.mapValidated;
    this->playersAdded = gameEngine.playersAdded;
    this->assignReinforcement = gameEngine.assignReinforcement;
    this->issueOrders = gameEngine.issueOrders;
    this->executeOrders = gameEngine.executeOrders;
    this->win = gameEngine.win;
    this->currentGameState = gameEngine.currentGameState;
    return *this;
}

/**
 * Override the << operator for GameEngine.
 */
ostream& operator<<(ostream& stream, const GameEngine& gameEngine) {
    stream << "Current GameEngine State : " << gameEngine.currentGameState->name << endl;
    return stream;
}

/**
 * Default constructor
 */
GameState::GameState() {
    this->name = "none";
    this->gameEngine = nullptr;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
GameState::GameState(GameEngine* gameEngine) {
    this->name = "none";
    this->gameEngine = gameEngine;
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool GameState::isValidTransition() {
    return true;
}

/**
 * Destructor
 */
GameState::~GameState() {

}

/**
 * Copy constructor
 * @param start gamestate to copy
 */
GameState::GameState(const GameState& start) {
    this->name = start.name;
    this->gameEngine = start.gameEngine;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
Start::Start(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "start";
    this->gameEngine = gameEngine;
}

Start::Start(const Start& start) : GameState(start) {
    this->name = start.name;
    this->gameEngine = start.gameEngine;
}

/**
 * Destructor
 */
Start::~Start() {
}

/**
 * Handles what happens when entering a specific state.
 */
void Start::enterState() {
    cout << "Entering start state" << endl;

    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::LoadMap);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool Start::isValidTransition() {
    return true;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* Start::copy() {
    return new Start(*this);
}


/**
 * Constructor
 * @param name
 * @param gameEngine
 */

MapLoaded::MapLoaded(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "loadmap";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
MapLoaded::~MapLoaded() {
}

/**
 * Handles what happens when entering a specific state.
 */
void MapLoaded::enterState() {
    cout << "Entering Loadmap state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::LoadMap);
            validInput = true;
        } else if (input == VALID_COMMANDS[1]) {
            gameEngine->changeStateByTransition(GameEngine::ValidateMap);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool MapLoaded::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param mapLoaded
 */
MapLoaded::MapLoaded(const MapLoaded& mapLoaded) : GameState(mapLoaded) {
    this->name = mapLoaded.name;
    this->gameEngine = mapLoaded.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* MapLoaded::copy() {
    return new MapLoaded(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
MapValidated::MapValidated(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "mapvalidated";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
MapValidated::~MapValidated() {
}

/**
 * Handles what happens when entering a specific state.
 */
void MapValidated::enterState() {
    cout << "Entering MapValidated state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::AddPlayer);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool MapValidated::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param mapValidated
 */
MapValidated::MapValidated(const MapValidated& mapValidated) : GameState(mapValidated) {
    this->name = mapValidated.name;
    this->gameEngine = mapValidated.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* MapValidated::copy() {
    return new MapValidated(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
PlayersAdded::PlayersAdded(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "playersadded";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
PlayersAdded::~PlayersAdded() {
}

/**
 * Handles what happens when entering a specific state.
 */
void PlayersAdded::enterState() {
    cout << "Entering PlayersAdded state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::AddPlayer);
            validInput = true;
        } else if (input == VALID_COMMANDS[1]) {
            gameEngine->changeStateByTransition(GameEngine::AssignCountries);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool PlayersAdded::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param playersAdded
 */
PlayersAdded::PlayersAdded(const PlayersAdded& playersAdded) : GameState(playersAdded) {
    this->name = playersAdded.name;
    this->gameEngine = playersAdded.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* PlayersAdded::copy() {
    return new PlayersAdded(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
AssignReinforcement::AssignReinforcement(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "assignreinforcement";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
AssignReinforcement::~AssignReinforcement() {
}

/**
 * Handles what happens when entering a specific state.
 */
void AssignReinforcement::enterState() {
    cout << "Entering AssignReinforcement state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::IssueOrder);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool AssignReinforcement::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param assignReinforcement
 */
AssignReinforcement::AssignReinforcement(const AssignReinforcement& assignReinforcement) : GameState(
        assignReinforcement) {
    this->name = assignReinforcement.name;
    this->gameEngine = assignReinforcement.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* AssignReinforcement::copy() {
    return new AssignReinforcement(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
IssueOrders::IssueOrders(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "issueorders";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
IssueOrders::~IssueOrders() {
}

/**
 * Handles what happens when entering a specific state.
 */
void IssueOrders::enterState() {
    cout << "Entering IssueOrders state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::IssueOrder);
            validInput = true;
        } else if (input == VALID_COMMANDS[1]) {
            gameEngine->changeStateByTransition(GameEngine::EndIssueOrders);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool IssueOrders::isValidTransition() {
    return true;
}

IssueOrders::IssueOrders(const IssueOrders& issueOrders) : GameState(issueOrders) {
    this->name = issueOrders.name;
    this->gameEngine = issueOrders.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* IssueOrders::copy() {
    return new IssueOrders(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
ExecuteOrders::ExecuteOrders(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "executeorders";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
ExecuteOrders::~ExecuteOrders() {
}

/**
 * Handles what happens when entering a specific state.
 */
void ExecuteOrders::enterState() {
    cout << "Entering ExecuteOrders state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::Execorder);
            validInput = true;
        } else if (input == VALID_COMMANDS[1]) {
            gameEngine->changeStateByTransition(GameEngine::Endexecorders);
            validInput = true;
        } else if (input == VALID_COMMANDS[2]) {
            gameEngine->changeStateByTransition(GameEngine::Win);
            validInput = true;
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool ExecuteOrders::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param executeOrders
 */
ExecuteOrders::ExecuteOrders(const ExecuteOrders& executeOrders) : GameState(executeOrders) {
    this->name = executeOrders.name;
    this->gameEngine = executeOrders.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* ExecuteOrders::copy() {
    return new ExecuteOrders(*this);
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
Win::Win(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "win";
    this->gameEngine = gameEngine;
}

/**
 * Destructor
 */
Win::~Win() {
}

/**
 * Handles what happens when entering a specific state.
 */
void Win::enterState() {
    cout << "Entering Win state" << endl;


    cout << "What state would you like to transition to?" << endl;
    bool validInput = false;
    while (!validInput) {
        cout << "Valid transitions : ";
        for (auto const& value: VALID_COMMANDS) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == VALID_COMMANDS[0]) {
            gameEngine->changeStateByTransition(GameEngine::Play);
            validInput = true;
        } else if (input == VALID_COMMANDS[1]) {
            validInput = true;
            exit(0);
        } else {
            cout << "An invalid transition was entered. Please try again." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool Win::isValidTransition() {
    return true;
}

/**
 * Copy constructor.
 * @param win
 */
Win::Win(const Win& win) : GameState(win) {
    this->name = win.name;
    this->gameEngine = win.gameEngine;
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* Win::copy() {
    return new Win(*this);
}
