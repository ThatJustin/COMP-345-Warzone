//
// Created by Alex De Luca on 2022-11-03.
//

#include "LoggingObserver.h"

#include <fstream>
#include <ctime>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::ofstream;


/**
 * Observer constructor
 */
Observer::Observer() {}

/**
 * Observer destructor
 */
Observer::~Observer() {}

/**
 * Subject constructor
 */
Subject::Subject() {
    observers = new list<Observer*>;
}

/**
 * Subject destructor
 */
Subject::~Subject() {
    delete observers;
}

/**
 * Adding an observer to the end of the observer list
 */
void Subject::attach(Observer* obs) {
    observers->push_back(obs);
}

/*
 * Removing an observer from the end of the observer list
 */
void Subject::detach(Observer* obs) {
    observers->remove(obs);
}

/*
 * Notifying the observer that a change has been made
 */
void Subject::notify(ILoggable* i_loggable) {
    for (auto& observer: *observers) {
        observer->update(i_loggable);
    }
}

/*
 * LogObserver constructor
 */
LogObserver::LogObserver() {
}

/*
 * LogObserver destructor
 */
LogObserver::~LogObserver() {

}

/*
 * Updates the gamelog file when changes have been made in the subjects
 */
void LogObserver::update(ILoggable* i_loggable) {
    ofstream game_log_file("./Logs/gamelog.txt", ofstream::app | ofstream::out);
    if (game_log_file.is_open()) {
        auto current_time = std::chrono::system_clock::now();
        auto time_inn_t = std::chrono::system_clock::to_time_t(current_time);
        string time = std::ctime(&time_inn_t);
        time.pop_back();
        game_log_file << "[" << time << "] " << i_loggable->stringToLog() << endl;
        game_log_file.close();
    } else {
        cout << "Unable to open the game log text file. Please try again." << endl;
    }
}