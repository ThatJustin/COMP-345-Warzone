//
// Created by Alex De Luca on 2022-11-03.
//

#include "LoggingObserver.h"
/**
 *
 * Observer class
 *
 */

/**
 * Observer constructor
 */
Observer::Observer() {}
Observer::~Observer(){}

LogObserver::LogObserver(){
    this->FILENAME="NULL";
}

LogObserver::~LogObserver(){

}

void LogObserver::setFilename(string filename){
    this->FILENAME=filename;
}

string LogObserver::getFilename(){
    return FILENAME;
}
void LogObserver::update(ILoggable *ilog){

}

Subject::Subject(){
    observers = new list<Observer*>;
}

Subject::~Subject(){
    for(int i=0;i<observers.size();i++){
        delete observers[i];
    }

    delete observers;
}


void Subject::attach(Observer *obs){
    observers->push_back(obs);
}

void Subject::detach(Observer *obs){
    observers->remove(obs);
}

void Subject::notify(){
    list<Observer *>::iterator i = observers->begin();
    for (; i != observers->end(); ++i){
        (*i)->update();
    }
}




