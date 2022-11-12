//
// Created by Alex De Luca on 2022-11-03.
//

#include "LoggingObserver.h"

#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::ofstream;

/**
 *
 * Observer class
 *
 */

/**
 * Observer constructor
 */
Observer::Observer() {}

Observer::~Observer() {}

Subject::Subject() {
    observers = new list<Observer*>;
}

Subject::~Subject() {
    delete observers;
}

void Subject::attach(Observer* obs) {
    observers->push_back(obs);
}

void Subject::detach(Observer* obs) {
    observers->remove(obs);
}

void Subject::notify(ILoggable* i_loggable) {
    for (auto& observer : *observers) {
        observer->update(i_loggable);
    }
}

LogObserver::LogObserver() {
}

LogObserver::~LogObserver() {

}

void LogObserver::update(ILoggable* i_loggable) {
    ofstream game_log_file("./Logs/gamelog.txt", ofstream::app);
    if (game_log_file.is_open()){
        game_log_file << i_loggable->stringToLog() << endl;
        game_log_file.close();
    } else {
        cout << "Unable to open the game log text file. Please try again." << endl;
    }
}