#include "GameEngine.h"
#include "../Map/Map.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "math.h"
#include "CommandProcessor.h"
#include "sources/Map/Map.h"
#include "sources/Player/Player.h"
#include "sources/Cards/Cards.h"
#include "sources/Orders/Orders.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "sources/Player/PlayerStrategy.h"

/**
 * Constructor of GameEngine
 */

GameEngine::GameEngine(LogObserver* obs) {
    attach(obs);
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
    this->commandTransitionName = "";
    this->gameMap = nullptr;
    this->gamePlayers = std::vector<Player*>();
    this->deck = new Deck();
    this->neutral = new Player("NeutralPlayer");
    this->isTournamentMode = false;
    this->tournamentNumberOfGames = 0;
    this->result = map<string, string>();
    this->tournamentMapName = "";
    this->hasTournamentEnded = false;
    this->turnNumber == 0;
}

/**
 * Destructor of GameEngine
 */
GameEngine::~GameEngine() {
    detach(observer);
    if (win != nullptr) {
        delete win;
        win = nullptr;
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
    if (commandProcessor != nullptr) {
        delete commandProcessor;
        commandProcessor = nullptr;
    }
    if (gameMap != nullptr) {
        delete gameMap;
        gameMap = nullptr;
    }
    if (deck != nullptr) {
        delete deck;
        deck = nullptr;
    }
    if (neutral != nullptr) {
        delete neutral;
        neutral = nullptr;
    }
    if (!gamePlayers.empty()) {
        for (auto p: gamePlayers) {
            delete p;
        }
        gamePlayers.clear();
    }
}

void GameEngine::initializeTournament(string ListOfMapFiles, string ListOfPlayerStrategies, int NumberOfGames,
                                      int MaxNumberOfTurns) {
    std::cout << "Starting tournament" << std::endl;
    this->isTournamentMode = true;
    this->tournamentNumberOfGames = NumberOfGames;
    vector<string> MapFiles;
    vector<string> PlayerStrategies;
    string MapFile;
    string PlayerStrategy;
    std::stringstream MapFileStream(ListOfMapFiles);
    std::stringstream PlayerStrategyStream(ListOfPlayerStrategies);

    while (std::getline(MapFileStream, MapFile, ',')) {
        MapFiles.push_back(MapFile);
    }
    while (std::getline(PlayerStrategyStream, PlayerStrategy, ',')) {
        PlayerStrategies.push_back(PlayerStrategy);
    }
    int index = 0;
    for (auto& iteratorMapFile: MapFiles) {
        cout << "creating map files" << endl;
        ofstream File("Tournaments/map" + std::to_string(index) + ".txt");
        File << "loadmap " << iteratorMapFile << endl;
        File << "validatemap" << endl;
        for (auto& iteratorPlayerStrategy: PlayerStrategies) {
            File << "addplayer " << iteratorPlayerStrategy << endl;
        }
        File << "gamestart" << endl;
        for (int i = 0; i < NumberOfGames; i++) {
            File << "replay" << endl;
        }
        File << "quit" << endl;
        File.close();
        index++;
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
        notify(this);
        this->getCurrentGameState()->enterState();
    } else {
        // isValidToTransitionAway will be used for when we need to validate that the transition is okay in the future
        // for now it's always true
        if (this->getCurrentGameState()->isValidToTransitionAway()) {
            this->currentGameState = getStateFromTransition(transition);
            notify(this);
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

    while (true) {
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
            //Maingameloop is called after gameStart
            gameStart();
        } else if (c->getTransitionName() == "replay") {
            prepareForReplay();
            changeStateByTransition(StartGame);
        } else if (c->getTransitionName() == "quit") {
            cout << "Thanks for playing!" << endl;
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
    //When this function is called, you are already in the AssignReinforcement state
    while (!hasWinner) {
        changeStateByTransition(GameEngine::GameStart); // goes to assign reinforcement state

        changeStateByTransition(GameEngine::IssueOrder); // goes to issue order state
        cout << endl;
        changeStateByTransition(GameEngine::IssueOrdersEnd); // goes to execute orders state
    }
}

/**
 * Prepares the game engine to replay the game.
 */
void GameEngine::prepareForReplay() {
    //Delete and reinitialize objects from part 2 like map, players, etc
    delete gameMap;
    for (auto p: gamePlayers) {
        delete p;
    }
    gamePlayers.clear();
    delete deck;
    delete neutral;
    this->hasWinner = false;
    this->commandParam = "";
    this->commandTransitionName = "";
    this->gameMap = nullptr;
    this->gamePlayers = std::vector<Player*>();
    this->deck = new Deck();
    this->neutral = new Player("NeutralPlayer");
    this->getPlayer = new Player();
    this->turnNumber == 0;

    // If it's being read from a file
    if (!this->isUsingConsole()) {
        FileCommandProcessorAdapter* processorAdapter = dynamic_cast<FileCommandProcessorAdapter*>(this->commandProcessor);
        if (processorAdapter != nullptr) {
            FileLineReader* fileLineReader = processorAdapter->fileLineReader;
            if (fileLineReader != nullptr) {
                //Save the position of replay to skip it on its next file read
                fileLineReader->replayPositions.push_back(fileLineReader->filelinePosition - 1);

                //Reset back to the beginning
                fileLineReader->filelinePosition = 1;
            }
        }
    }
}

/**
 * Returns true if the game is using the console for input or false for reading from a file.
 * @return
 */
bool GameEngine::isUsingConsole() {
    return dynamic_cast<FileCommandProcessorAdapter*>(this->commandProcessor) == nullptr;
}

/**
 * Returns the command processor object.
 * @return
 */
CommandProcessor* GameEngine::getCommandProcessor() const {
    return commandProcessor;
}

void GameEngine::addPlayer(Player* pPlayer) {
    pPlayer->getOrdersList()->attach(observer);
    cout << endl;
    gamePlayers.push_back(pPlayer);
    cout << endl;

}

vector<Player*> GameEngine::getGamePlayers() {
    return gamePlayers;
}

bool GameEngine::isPlayerAdded(Player* pPlayer) {
    for (auto& p: this->getGamePlayers()) {
        if (p->getPlayerName() == pPlayer->getPlayerName()) {
            cout << "Player already exists!" << endl;
            return true;
        }
    }
    return false;
}

/**
fairly distribute all the territories to the players
determine randomly the order of play of the players in the game
give 50 initial army units to the players, which are placed in their respective reinforcement pool
let each player draw 2 initial cards from the deck using the deck’s draw() method
switch the game to the play phase
 */
void GameEngine::gameStart() {
    std::random_device rd;
    default_random_engine randomEngine(rd());

    int playerCount = this->getGamePlayers().size();
    int totalTerritoryCount = this->gameMap->getTerritories().size();

    vector<Territory*> shuffledTerritories = gameMap->getShuffledTerritories();

    div_t divisionResult;
    divisionResult = div(totalTerritoryCount, playerCount);

    vector<Player*> shuffledPlayers = getGamePlayers();

    //b) determine randomly the order of play of the players in the game
    shuffle(shuffledPlayers.begin(), shuffledPlayers.end(), randomEngine);


    int individualTerritoryCount = divisionResult.quot;
    int individualTerritoryStartIndex = 0;
    for (auto player: this->getGamePlayers()) {

        // Assign the territories to the player
        for (int i = individualTerritoryStartIndex; i < individualTerritoryCount; i++) {
            Territory* territory = shuffledTerritories.at(i);
            territory->setTerritoryOwner(player);
            player->addTerritory(territory);
        }

        //c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
        player->setReinforcementPool(50);

        //d) let each player draw 2 initial cards from the deck using the deck’s draw() method
        for (int i = 0; i < 2; i++) {
            deck->draw(player);
        }
        individualTerritoryStartIndex += divisionResult.quot;
        individualTerritoryCount += divisionResult.quot;
    }
    // Assign the leftover territories to the neutral player
    if (divisionResult.rem != 0) {
        for (int i = individualTerritoryStartIndex; i < individualTerritoryStartIndex + divisionResult.rem; i++) {
            Territory* territory = shuffledTerritories.at(i);
            territory->setTerritoryOwner(neutral);
            neutral->addTerritory(territory);
        }
    }
    //Once mainGameLoop is called, the game will run by itself until it gets to the win state
    if (!bIsDemo) {
        mainGameLoop();
    }
}

/**
 * Gets the deck used for the game.
 * @return
 */
Deck* GameEngine::getDeck() const {
    return deck;
}

/**
 * Gets the neutral player.
 * @return
 */
Player* GameEngine::getNeutralPlayer() const {
    return neutral;
}

void GameEngine::attach(Observer* obs) {
    Subject::attach(obs);
    observer = obs;
}

void GameEngine::detach(Observer* obs) {
    Subject::detach(obs);
    obs = nullptr;
}

string GameEngine::stringToLog() {
    string log = "[State Change] Game has changed to state [" + this->currentGameState->name + "].";
    if (isTournamentMode && hasTournamentEnded) {
        //loop in results map
        for (auto& r: result) {
            log += "Map: " + r.first;
            log += "Winner: " + r.second;
        }
    }
    return log;
}

void GameEngine::setGameMap(Map* pMap) {
    this->gameMap = pMap;
}

void GameEngine::removePlayer(Player* pPlayer) {
    int pos = -1;
    for (int i = 0; i < this->gamePlayers.size(); i++) {
        Player* player = this->gamePlayers.at(i);
        if (pPlayer->getPlayerName() == player->getPlayerName()) {
            pos = i;
            break;
        }
    }
    if (pos >= 0) {
        cout << endl;
        gamePlayers.erase(gamePlayers.begin() + pos);
        cout << endl;
    }
}

void GameEngine::setDemo(bool b) {
    bIsDemo = b;
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
bool GameState::isValidToTransitionAway() {
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
bool Start::isValidToTransitionAway() {
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
    cout << "Entering " << *this;
    auto* mapLoader = new MapLoader();
    Map* map = mapLoader->loadMap("./Map Files/" + this->gameEngine->commandParam);

    if (map != nullptr) {
        //Map is good
        if (this->gameEngine->gameMap != nullptr) {
            delete this->gameEngine->gameMap;
            this->gameEngine->gameMap = nullptr;
            cout << "The previously loaded map has been successfully replaced." << endl;
        }
        this->gameEngine->gameMap = map;
        cout << "Map " << this->gameEngine->commandParam << " successfully loaded." << endl;
    }
    cout << endl;
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool MapLoaded::isValidToTransitionAway() {
    //If they re-enter the loadmap command transition, it's a valid transition
    if (this->gameEngine->commandTransitionName == "loadmap") {
        return true;
    }
    return gameEngine->gameMap != nullptr;
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
    Map* map = gameEngine->gameMap;
    if (map->validate()) {
        cout << "Map is valid." << endl;
    } else {
        //Invalid Map
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool MapValidated::isValidToTransitionAway() {
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
    cout << endl;
    vector<Player*> players = this->gameEngine->getGamePlayers();
    cout << "Entering " << *this;
    cout << "Current players added: [";
    for (auto& p: players) {
        if (p != players.back()) {
            cout << p->getPlayerName() << ",";
        } else {
            cout << p->getPlayerName();
        }
    }
    cout << "]" << endl << endl;
    Player* player = new Player(this->gameEngine->commandParam);
    if (gameEngine->getGamePlayers().size() == 6) {
        cout << "There is already the maximum amount of players 6. Please go to the next state." << endl;
        return;
    }
    if (player->getPlayerName() == "Human") {
        player->setPlayerStrategy(new HumanPlayerStrategy(player));
    } else if (player->getPlayerName() == "Benevolent") {
        player->setPlayerStrategy(new BenevolentPlayerStrategy(player));
    } else if (player->getPlayerName() == "Cheater") {
        player->setPlayerStrategy(new CheaterPlayerStrategy(player));
    } else if (player->getPlayerName() == "Neutral") {
        player->setPlayerStrategy(new NeutralPlayerStrategy(player));
    } else if (player->getPlayerName() == "Aggressive") {
        player->setPlayerStrategy(new AggressivePlayerStrategy(player));
    }
    if (players.empty()) {
        this->gameEngine->addPlayer(player);
        cout << "Player " << this->gameEngine->commandParam << " successfully added." << endl;
    } else {
        if (!this->gameEngine->isPlayerAdded(player)) {
            this->gameEngine->addPlayer(player);
            cout << "Player " << this->gameEngine->commandParam << " successfully added." << endl;
        }
    }
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool PlayersAdded::isValidToTransitionAway() {
    if (gameEngine->getGamePlayers().size() < 2 && this->gameEngine->commandTransitionName != "addplayer") {
        cout << "A minimum of 2 players are required to play this game." << endl;
        return false;
    }
    if (this->gameEngine->commandTransitionName == "addplayer") {
        return true;
    }
    return gameEngine->getGamePlayers().size() >= 2 && gameEngine->getGamePlayers().size() <= 6;
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
 * Phase to give player army:
 * 1.Players are given a number of army units that depends on the number of territories they own, (# of territories owned divided by 3, rounded down)._/
 * 2.If the player owns all the territories of an entire continent, the player is given a number of army units corresponding to the continent’s control bonus value.
 * 3.In any case, the minimal number of reinforcement army units per turn for any player is 3._/
 * 4.These army units are placed in the player’s reinforcement pool.-
 * 5.This must be implemented in a function/method named reinforcementPhase() in the game engine. _/
 */
void AssignReinforcement::enterState() {
    cout << "Entering " << *this << endl;

    //Increment the turn, this increments each time this state is entered (full circle achieved in maingameloop)
    this->gameEngine->turnNumber++;

    for (Player* player: this->gameEngine->getGamePlayers()) {
        cout << endl << "AssignReinforcement for player " << player->getPlayerName() << " " << endl;
        //Players are given a number of army units that depends on the number of territories they own, (# of territories owned divided by 3, rounded down).
        int countToAdd = 0;
        if (!player->getTerritories().empty()) {
            int terrReinf = (int) std::floor((player->getTerritories().size()) / 3);
            countToAdd = countToAdd + terrReinf;
            cout << "Units from territories " << countToAdd << endl;
        }
        // If the player owns all the territories of an entire continent, the player is given a number of army units corresponding to the continent’s control bonus value.
        for (Continent* continent: gameEngine->gameMap->getContinents()) {
            if (continent->isContinentOwner(player->getPlayerName())) {
                countToAdd = countToAdd + continent->getContinentControlBonusValue();
                cout << "Player " << player->getPlayerName() << " owns all territories on continent "
                     << continent->getContinentName() << " bonus of " << continent->getContinentControlBonusValue()
                     << " added." << endl;
            }
        }
        //In any case, the minimal number of reinforcement army units per turn for any player is 3.
        if (countToAdd < 3) {
            countToAdd = 3;
        }
        player->setReinforcementPool(player->getReinforcementPool() + countToAdd);
        cout << endl << "Total new reinforcement units " << countToAdd << "." << endl;
    }
    cout << endl;
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool AssignReinforcement::isValidToTransitionAway() {
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
 * Phase that Player issue order:
 * 1.Players issue orders and place them in their order list through a call to the Player::issueOrder() method._/
 * 2.This method is called in round-robin fashion across all players by the game engine._/
 * 3.This phase ends when all players have signified that they don’t have any more orders to issue for this turn.
 * 4.This must be implemented in a function/method named issueOrdersPhase() in the game engine. -
 */
void IssueOrders::enterState() {
    cout << "Entering " << *this << endl;
    //Reset anything required to start a neww issue order phase
    for (Player* player: this->gameEngine->getGamePlayers()) {
        player->setNegotiationWith(nullptr);
    }

    //call the player issue order method to add order in their order list
    for (Player* player: this->gameEngine->getGamePlayers()) {
        if (!player->getTerritories().empty()) {
            player->issueOrder(gameEngine);
        }
    }
    cout << endl;
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool IssueOrders::isValidToTransitionAway() {
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
 * Phase to execute player's order:
 * 1.Once all the players have signified in the same turn that they are not issuing one more order,_/
 * the game engine proceeds to execute the top order on the list of orders of each player in a round-robin fashion (i.e. the “Order Execution Phase”—see below).
 * 2.Once all the players’ orders have been executed, the main game loop goes back to the reinforcement phase._/
 * 3.This must be implemented in a function/method named executeOrdersPhase() in the game engine._/
 */
void ExecuteOrders::enterState() {
    cout << "Entering " << *this << endl;
    if (gameEngine->bIsDemo) {
        cout << "Skipping using orders for demo purpose." << endl;
    } else {
        for (Player* player: gameEngine->getGamePlayers()) {
            player->useOrders();
        }
    }
    cout << endl;
    vector<Player*> playersToRemove = vector<Player*>();
    // Check if anyone owns 0 territories
    for (Player* player: gameEngine->getGamePlayers()) {
        if (player->getTerritories().empty()) {
            playersToRemove.push_back(player);
            cout << endl;
        }
    }
    //remove them
    for (Player* player: playersToRemove) {
        cout << player->getPlayerName() << " has no more territories and is removed from the game." << endl;
        gameEngine->removePlayer(player);
        cout << endl;
    }
    playersToRemove.clear();
    cout << endl;
    //Check if there's a winner (1 player left)
    if (gameEngine->getGamePlayers().size() == 1) {
        gameEngine->hasWinner = true;
        this->gameEngine->changeStateByTransition(GameEngine::Win);
        if (gameEngine->isTournamentMode) {
            gameEngine->result.insert(
                    {gameEngine->tournamentMapName, gameEngine->getGamePlayers()[0]->getPlayerName()});
        }
    }
    cout << endl;
}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool ExecuteOrders::isValidToTransitionAway() {
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

    cout << "Player " << gameEngine->getGamePlayers().at(0)->getPlayerName() << " has won the game!" << endl;

}

/**
 * Returns true if the state is allowed to make this transition.
 * @return
 */
bool Win::isValidToTransitionAway() {
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