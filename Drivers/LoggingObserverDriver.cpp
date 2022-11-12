#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Orders/Orders.h"
#include <fstream>

void testLoggingObserver(){
    auto* command = new Command();
    auto* commandProcessor = new CommandProcessor();

    Orders* deploy_order = new Deploy();
    Orders* advance_order = new Advance();
    Orders* bomb_order = new Bomb();
    Orders* blockade_order = new Blockade();
    Orders* airlift_order = new Airlift();
    Orders* negotiate_order = new Negotiate();

    auto* orders_list = new OrdersList();

    auto* observer = new LogObserver();
    auto* gameEngine = new GameEngine(observer);

    // this checks if the Command, CommandProcessor, Orders, OrdersList and GameEngine classes are
    // all subclasses of the Subject and ILoggable classes
    if (dynamic_cast<Subject*>(command) != nullptr && dynamic_cast<ILoggable*>(command) != nullptr) {
        cout << "The Command class is a subclass of the Subject and ILoggable classes.\n" << endl;
    }
    else {
        cout << "The Command class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(commandProcessor) != nullptr && dynamic_cast<ILoggable*>(commandProcessor) != nullptr) {
        cout << "The CommandProcessor class is a subclass of the Subject and ILoggable classes.\n" << endl;
    }
    else {
        cout << "The CommandProcessor class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    // this uses the deploy order as an example to show that the Orders class is a subclass of the Subject and ILoggable classes
    // since the deploy order is of Orders class type
    if (dynamic_cast<Subject*>(deploy_order) != nullptr && dynamic_cast<ILoggable*>(deploy_order) != nullptr) {
        cout << "The Orders class is a subclass of the Subject and ILoggable classes.\n" << endl;
    }
    else {
        cout << "The Orders class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(orders_list) != nullptr && dynamic_cast<ILoggable*>(orders_list) != nullptr) {
        cout << "The OrdersList class is a subclass of the Subject and ILoggable classes.\n" << endl;
    }
    else {
        cout << "The OrdersList class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    if (dynamic_cast<Subject*>(gameEngine) != nullptr && dynamic_cast<ILoggable*>(gameEngine) != nullptr) {
        cout << "The GameEngine class is a subclass of the Subject and ILoggable classes.\n" << endl;
    }
    else {
        cout << "The GameEngine class is NOT a subclass of the Subject and ILoggable classes.\n" << endl;
    }

    // check if notify is called when execute is called
    deploy_order->execute();
    observer->update(deploy_order);
    // read contents of game log file
    ifstream game_log_file("./Logs/game_log.txt");
    string line;
    if (game_log_file.is_open()) {
        while (getline(game_log_file, line)) {
            cout << line << endl;
        }
        game_log_file.close();
    }
    else {
        cout << "\nUnable to open file" << endl;
    }

    gameEngine->changeStateByTransition(1);
    observer->update(gameEngine);
    // read contents of game log file
    if (game_log_file.is_open()) {
        while (getline(game_log_file, line)) {
            cout << line << endl;
        }
        game_log_file.close();
    }
    else {
        cout << "\nUnable to open file" << endl;
    }

    orders_list->addOrder(deploy_order);
    orders_list->addOrder(advance_order);
    orders_list->addOrder(bomb_order);
    orders_list->addOrder(blockade_order);
    orders_list->addOrder(airlift_order);
    orders_list->addOrder(negotiate_order);
}