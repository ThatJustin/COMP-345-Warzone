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
    start = gameEngine.start;
    loadMap = gameEngine.loadMap;
    mapValidated = gameEngine.mapValidated;
    playersAdded = gameEngine.playersAdded;
    assignReinforcement = gameEngine.assignReinforcement;
    issueOrders = gameEngine.issueOrders;
    executeOrders = gameEngine.executeOrders;
    win = gameEngine.win;
    currentGameState = gameEngine.currentGameState;
}

/**
 * Override the = operator for GameEngine.
 */
GameEngine& GameEngine::operator=(const GameEngine& gameEngine) {
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

ostream& operator<<(ostream& stream, const GameEngine& gameEngine) {
    stream << "Current GameEngine State " << gameEngine.currentGameState->name << endl;
    return stream;
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
 * Constructor
 * @param name
 * @param gameEngine
 */
Start::Start(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "start";
    this->gameEngine = gameEngine;
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