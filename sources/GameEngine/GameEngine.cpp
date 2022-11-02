#include "GameEngine.h"
#include "../Player/Player.h"
#include "sources/Map/Map.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "math.h"

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
    this->name = "loadmap";
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

/**
 * Phase to give player army:
 * 1.Players are given a number of army units that depends on the number of territories they own, (# of territories owned divided by 3, rounded down)._/
 * 2.If the player owns all the territories of an entire continent, the player is given a number of army units corresponding to the continent’s control bonus value.
 * 3.In any case, the minimal number of reinforcement army units per turn for any player is 3._/
 * 4.These army units are placed in the player’s reinforcement pool.-
 * 5.This must be implemented in a function/method named reinforcementPhase() in the game engine. -
 */
void MainGameLoop::reinforcementPhase(Player* player){ //potentially take parameter

    //if check the amount of territory own for each player and give army accordingly
    if(player->getTerritories().size() != 0){

        //round down the amount of ary based on the amount of territory owned by the player
        player->setArmy(std::round((player->getTerritories().size())/3));
    }

    //if player own all territory given a number of army units corresponding to the continent control bonus value

    for(Continent* continent: map->getContinents()){
        for(Territory* territory: continent->getTerritories()){
            //verify if the player is the one that own the territory
            if(territory->getTerritoryOwner() != player){
                goto end;//continue the external loop
            }
        }
        player->setArmy(controlbonus);//need to change controlbonus to get continent value
        end:;//continue from this
    }

    //if minimum reinforcement per turn is 3
    player->setArmy(+3);
}

/**
 * Phase that Player issue order:
 * 1.Players issue orders and place them in their order list through a call to the Player::issueOrder() method.
 * 2.This method is called in round-robin fashion across all players by the game engine.
 * 3.This phase ends when all players have signified that they don’t have any more orders to issue for this turn.
 * 4.This must be implemented in a function/method named issueOrdersPhase() in the game engine. -
 */
void MainGameLoop::issueOrdersPhase(){

    //call the player issue order method to add order in their order list

    //implement in a round robin fashion

    //phase end when all player have no more order to issue for their turn

}

/**
 * Phase to execute player's order:
 * 1.Once all the players have signified in the same turn that they are not issuing one more order,
 * the game engine proceeds to execute the top order on the list of orders of each player in a round-robin fashion (i.e. the “Order Execution Phase”—see below).
 * 2.Once all the players’ orders have been executed, the main game loop goes back to the reinforcement phase.
 * 3.This must be implemented in a function/method named executeOrdersPhase() in the game engine.
 */
void MainGameLoop::executeOrdersPhase(){

    //once no more order, execute the top order on the list in a round robin fashion

    //execute all order, then go back to the reinforcement phase

}
