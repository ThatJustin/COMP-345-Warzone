#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Orders/Orders.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include <fstream>

void testLoggingObserver(int argc, char** argv) {
    //Used to show only subclass, not to be used in actual test
    auto* command = new Command();
    auto* deploy_order = new Deploy();
    auto* orders_list = new OrdersList();
    auto* commandProcessor_test = new CommandProcessor();
    auto* gameEngine_test = new GameEngine(nullptr);
    // End

    cout << "Testing they are a subclass of Subject and ILoggable classes." << endl;
    // this checks if the Command, CommandProcessor, Orders, OrdersList and GameEngine classes are
    // all subclasses of the Subject and ILoggable classes
    if (dynamic_cast<Subject*>(command) != nullptr && dynamic_cast<ILoggable*>(command) != nullptr) {
        cout << "The Command class is a subclass of the Subject and ILoggable classes.\n" << endl;
    } else {
        cout << "The Command class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(commandProcessor_test) != nullptr &&
        dynamic_cast<ILoggable*>(commandProcessor_test) != nullptr) {
        cout << "The CommandProcessor class is a subclass of the Subject and ILoggable classes.\n" << endl;
    } else {
        cout << "The CommandProcessor class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    // this uses the deploy order as an example to show that the Orders class is a subclass of the Subject and ILoggable classes
    // since the deploy order is of Orders class type
    if (dynamic_cast<Subject*>(deploy_order) != nullptr && dynamic_cast<ILoggable*>(deploy_order) != nullptr) {
        cout << "The Orders class is a subclass of the Subject and ILoggable classes.\n" << endl;
    } else {
        cout << "The Orders class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(orders_list) != nullptr && dynamic_cast<ILoggable*>(orders_list) != nullptr) {
        cout << "The OrdersList class is a subclass of the Subject and ILoggable classes.\n" << endl;
    } else {
        cout << "The OrdersList class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(gameEngine_test) != nullptr && dynamic_cast<ILoggable*>(gameEngine_test) != nullptr) {
        cout << "The GameEngine class is a subclass of the Subject and ILoggable classes.\n" << endl;
    } else {
        cout << "The GameEngine class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    cout
            << "Testing GameEngine::transition(), CommandProcessor::saveCommand(), Command::saveEffect() logging \r\nusing part 2 test func since it uses them anyway."
            << endl;

    //We wil test most functionality using basically part 2's test func since it uses a lot of what we have to show
    // GameEngine::transition, CommandProcessor::saveCommand(), Command::saveEffect()
    if (argc < 2 || (argv[1] != "-console"sv && argv[1] != "-file"sv)) {
        cout << "Please run the program using either -console or -file filename arguments." << endl;
        exit(0);
    }
    bool isUsingConsole = true;
    string mapFileName;
    if (argv[1] == "-file"sv) {
        if (argc < 3) {
            cout << "Please include a file name for this argument like -file filename." << endl;
            exit(0);
        }
        mapFileName = argv[2];
        isUsingConsole = false;
    }
    LogObserver* logObserver = new LogObserver();
    CommandProcessor* commandProcessor;
    if (isUsingConsole) {
        commandProcessor = new CommandProcessor(isUsingConsole, mapFileName, logObserver);
    } else {
        commandProcessor = new FileCommandProcessorAdapter(mapFileName, logObserver);
    }

    GameEngine* gameEngine = new GameEngine(logObserver);
    gameEngine->setCommandProcessor(commandProcessor);
    gameEngine->changeStateByTransition(GameEngine::StartGame);

    while (true) {
        string curStateName = gameEngine->getCurrentGameState()->name;
        Command* c = commandProcessor->getCommand(curStateName);
        if (c == nullptr) {
            cout << "This command is not valid for this state." << endl;
            continue;
        }
        gameEngine->commandTransitionName = c->getTransitionName();
        if (c->getTransitionName() == "loadmap") {
            gameEngine->commandParam = c->getParam();
            gameEngine->changeStateByTransition(GameEngine::LoadMap);
        } else if (c->getTransitionName() == "validatemap") {
            gameEngine->changeStateByTransition(GameEngine::ValidateMap);
        } else if (c->getTransitionName() == "addplayer") {
            gameEngine->commandParam = c->getParam();
            gameEngine->changeStateByTransition(GameEngine::AddPlayer);
        } else if (c->getTransitionName() == "gamestart") {
            gameEngine->gameStart();
            cout << "Displaying the outcome of gameStart being called." << endl;
            for (auto p: gameEngine->getGamePlayers()) {
                cout << *p << endl;
            }
            cout << "Simulating a win transition for demo purpose." << endl;
            gameEngine->changeStateByTransition(GameEngine::Win);
        } else if (c->getTransitionName() == "replay") {
            gameEngine->prepareForReplay();
            gameEngine->changeStateByTransition(GameEngine::Play);
        } else if (c->getTransitionName() == "quit") {
            cout << "Thanks for playing!" << endl;
            break;
        }
    }

    cout << "Testing OrderList addOrder logging." << endl;
    //  Just grab the last player added and use them as a basis to test orderlist/order logging
    Player* test = gameEngine->getGamePlayers().back();
    Territory* t1_1 = new Territory(0, "t1", test);

    test->getOrdersList()->addOrder(new Deploy(test, 0, t1_1));

    test->getOrdersList()->getOrdersList().at(0)->execute();

}