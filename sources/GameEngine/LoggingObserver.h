//
// Created by Alex De Luca on 2022-11-03.
//

#ifndef COMP_345_WARZONE_LOGGINGOBSERVER_H
#define COMP_345_WARZONE_LOGGINGOBSERVER_H

#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <list>
using namespace std;
class ILoggable{
public:
    virtual void stringToLog()=0;
};


class Observer{
public:
    ~Observer();
    virtual void update(ILoggable* ilog) = 0;
protected:
    Observer();
};

class Subject{
public:
    Subject();
    ~Subject();
    virtual void attach(Observer *obs) = 0;
    virtual void detach(Observer *obs) = 0;
    virtual void notify(ILoggable *ilog)=0;
private:
    list<*Observer> *observers;
};

class LogObserver : public Observer{
public:
    LogObserver();
    ~LogObserver();

    string getGameLog();
    void setGameLog(string gamelog);

    void update(ILoggable *ilog);
private:
    string GameLog;
};
#endif //COMP_345_WARZONE_LOGGINGOBSERVER_H
