#include "GameEngine.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "CommandProcessor.h"

/**
 * Constructor of GameEngine
 */
GameEngine::GameEngine() {
    //Keep track of the current game state
    this->currentGameState = nullptr;

    /*Create the states which will be used throughout the game.
      We create them now, so we don't have to create them all over again when they are being transitioned to.*/
    this->start = new Start(this);
    this->loadMap = new MapLoaded(this);
    this->mapValidated = new MapValidated(this);
    this->playersAdded = new PlayersAdded(this);
    this->assignReinforcement = new AssignReinforcement(this);
    this->issueOrders = new IssueOrders(this);
    this->executeOrders = new ExecuteOrders(this);
    this->win = new class Win(this);

    this->commandProcessor = new CommandProcessor();
    this->commandParam = "";
}

/**
 * Destructor of GameEngine
 */
GameEngine::~GameEngine() {
    if (currentGameState != nullptr) {
        delete currentGameState;
        currentGameState = nullptr;
    }
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
    if (commandProcessor != nullptr) {
        delete commandProcessor;
        commandProcessor = nullptr;
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
    } else if (transition == GameStart || transition == Endexecorders) {
        return assignReinforcement;
    } else if (transition == IssueOrder) {
        return issueOrders;
    } else if (transition == IssueOrdersEnd || transition == Execorder) {
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
    commandProcessor = gameEngine.commandProcessor;
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
    this->commandProcessor = gameEngine.commandProcessor;
    return *this;
}

/**
 * Set the commandProcessor object for the game engine.
 * @param commandProcessor
 */
void GameEngine::setCommandProcessor(CommandProcessor* commandProc) {
    this->commandProcessor = commandProc;
}

/**
 * Stream data for the class.
 * @param stream
 * @param gameEngine
 * @return
 */
ostream& operator<<(ostream& stream, const GameEngine& gameEngine) {
    stream << "Current GameEngine State : " << gameEngine.currentGameState->name << endl;
    return stream;
}

/**
 * Startup Phase
 */
void GameEngine::startupPhase() {

    changeStateByTransition(StartGame);

    bool runningStartupPhase = true;
    while (runningStartupPhase) {
        string curStateName = this->getCurrentGameState()->name;
        Command* c = this->commandProcessor->getCommand(curStateName);
        if (c == nullptr) {
            cout << "This command is not valid for this state." << endl;
            continue;
        }
        if (c->getTransitionName() == "loadmap") {
            this->commandParam = c->getParam();
            changeStateByTransition(LoadMap);
        } else if (c->getTransitionName() == "validatemap") {
            changeStateByTransition(ValidateMap);
        } else if (c->getTransitionName() == "addplayer") {
            this->commandParam = c->getParam();
            changeStateByTransition(AddPlayer);
        } else if (c->getTransitionName() == "gamestart") {
            //Once mainGameLoop is called, the game will run by itself until it gets to the win state and then
            // will return here
            mainGameLoop();
        } else if (c->getTransitionName() == "replay") {
            changeStateByTransition(StartGame);
        } else if (c->getTransitionName() == "quit") {
            exit(0);
        }
    }
}

/**
 * The main game loop is handled here with no user interfering with it.
 * The valid states are AssignReinforcement, IssueOrders and ExecuteOrders.
 * The valid transitions are IssueOrder, IssueOrdersEnd, Execorder, Endexecorders, Win
 */
void GameEngine::mainGameLoop() {

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
 * Assignment operator for the class.
 * @param gameState
 * @return
 */
GameState& GameState::operator=(const GameState& gameState) {
    if (this == &gameState) {
        return *this;
    }
    this->name = gameState.name;
    this->gameEngine = gameState.gameEngine;
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
Start::Start(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "start";
}

Start::Start(const Start& start) : GameState(start) {
    this->name = start.name;
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
    cout << "Entering " << *this << endl;

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
 * Stream data for the class.
 * @param stream
 * @param start
 * @return
 */
ostream& operator<<(ostream& stream, const Start& start) {
    stream << "State: " << start.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param start
 * @return
 */
Start& Start::operator=(const Start& start) {
    if (this == &start) {
        return *this;
    }
    GameState::operator=(start);
    return *this;
}


/**
 * Constructor
 * @param name
 * @param gameEngine
 */

MapLoaded::MapLoaded(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "maploaded";
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
    cout << "Entering " << *this << endl;

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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* MapLoaded::copy() {
    return new MapLoaded(*this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param mapLoaded
 * @return
 */
ostream& operator<<(ostream& stream, const MapLoaded& mapLoaded) {
    stream << "State: " << mapLoaded.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param mapLoaded
 * @return
 */
MapLoaded& MapLoaded::operator=(const MapLoaded& mapLoaded) {
    if (this == &mapLoaded) {
        return *this;
    }
    GameState::operator=(mapLoaded);
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
MapValidated::MapValidated(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "mapvalidated";
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
    cout << "Entering " << *this << endl;

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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* MapValidated::copy() {
    return new MapValidated(*this);
}

/**
 *  * Stream data for the class.
 * @param stream
 * @param mapValidated
 * @return
 */
ostream& operator<<(ostream& stream, const MapValidated& mapValidated) {
    stream << "State: " << mapValidated.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param mapValidated
 * @return
 */
MapValidated& MapValidated::operator=(const MapValidated& mapValidated) {
    if (this == &mapValidated) {
        return *this;
    }
    GameState::operator=(mapValidated);
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
PlayersAdded::PlayersAdded(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "playersadded";
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
    cout << "Entering " << *this << endl;


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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* PlayersAdded::copy() {
    return new PlayersAdded(*this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param playersAdded
 * @return
 */
ostream& operator<<(ostream& stream, const PlayersAdded& playersAdded) {
    stream << "State: " << playersAdded.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param playersAdded
 * @return
 */
PlayersAdded& PlayersAdded::operator=(const PlayersAdded& playersAdded) {
    if (this == &playersAdded) {
        return *this;
    }
    GameState::operator=(playersAdded);
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
AssignReinforcement::AssignReinforcement(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "assignreinforcement";
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
    cout << "Entering " << *this << endl;


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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* AssignReinforcement::copy() {
    return new AssignReinforcement(*this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param assignReinforcement
 * @return
 */
ostream& operator<<(ostream& stream, const AssignReinforcement& assignReinforcement) {
    stream << "State: " << assignReinforcement.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param assignReinforcement
 * @return
 */
AssignReinforcement& AssignReinforcement::operator=(const AssignReinforcement& assignReinforcement) {
    if (this == &assignReinforcement) {
        return *this;
    }
    GameState::operator=(assignReinforcement);
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
IssueOrders::IssueOrders(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "issueorders";
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
    cout << "Entering " << *this << endl;


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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* IssueOrders::copy() {
    return new IssueOrders(*this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param issueOrders
 * @return
 */
ostream& operator<<(ostream& stream, const IssueOrders& issueOrders) {
    stream << "State: " << issueOrders.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param issueOrders
 * @return
 */
IssueOrders& IssueOrders::operator=(const IssueOrders& issueOrders) {
    if (this == &issueOrders) {
        return *this;
    }
    GameState::operator=(issueOrders);
    return *this;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
ExecuteOrders::ExecuteOrders(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "executeorders";
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
    cout << "Entering " << *this << endl;


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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* ExecuteOrders::copy() {
    return new ExecuteOrders(*this);
}

/**
 * Assignment operator for the class.
 * @param executeOrders
 * @return
 */
ExecuteOrders& ExecuteOrders::operator=(const ExecuteOrders& executeOrders) {
    if (this == &executeOrders) {
        return *this;
    }
    GameState::operator=(executeOrders);
    return *this;
}

/**
 * Stream data for the class.
 * @param stream
 * @param executeOrders
 * @return
 */
ostream& operator<<(ostream& stream, const ExecuteOrders& executeOrders) {
    stream << "State: " << executeOrders.name << endl;
    return stream;
}

/**
 * Constructor
 * @param name
 * @param gameEngine
 */
Win::Win(GameEngine* gameEngine) : GameState(gameEngine) {
    this->name = "win";
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
    cout << "Entering " << *this << endl;

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
}

/**
 * Creates a deep copy of the object.
 * @return a new copy
 */
GameState* Win::copy() {
    return new Win(*this);
}

/**
 * Stream data for the class.
 * @param stream
 * @param win
 * @return
 */
ostream& operator<<(ostream& stream, const Win& win) {
    stream << "State: " << win.name << endl;
    return stream;
}

/**
 * Assignment operator for the class.
 * @param win
 * @return
 */
Win& Win::operator=(const Win& win) {
    if (this == &win) {
        return *this;
    }
    GameState::operator=(win);
    return *this;
}
