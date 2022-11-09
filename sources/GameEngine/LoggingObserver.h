//
// Created by Alex De Luca on 2022-11-03.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

class ILoggable {
public:
    virtual string stringToLog() = 0;
};

class Observer {
public:
    Observer();

    ~Observer();

    virtual void update(ILoggable* iLoggable) = 0;
};

class Subject {
public:
    Subject();

    ~Subject();

    virtual void attach(Observer* obs);

    virtual void detach(Observer* obs);

    virtual void notify(ILoggable* ilog);

protected:
    list<Observer*>* observers;
};

class LogObserver : public Observer {
public:
    LogObserver();

    ~LogObserver();

    string getGameLog();

    void setGameLog(string gamelog);

    void update(ILoggable* ilog) override;

private:
    string GameLog;
};
