//
// Created by Alex De Luca on 2022-11-03.
//

#include "LoggingObserver.h"

#include <utility>
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

LogObserver::LogObserver() {
}

LogObserver::~LogObserver() {

}

/*void LogObserver::setGameLog(string gamelog) {
    this->GameLog = std::move(gamelog);
}

string LogObserver::getGameLog() {
    return GameLog;
}*/

void LogObserver::update(ILoggable* ilog) {
    ofstream myFile("gamelog.txt");
    if (myFile.is_open()){
        myFile<<ilog->stringToLog();
        myFile.close();
    }
}

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

void Subject::notify(ILoggable* ilog) {
    auto i = observers->begin();
    for (; i != observers->end(); ++i) {
        (*i)->update(ilog);
    }
}




