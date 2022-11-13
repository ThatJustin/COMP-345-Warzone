//
// Created by Alex De Luca on 2022-11-03.
//

#pragma once

#include <iostream>
#include <list>

class ILoggable {
public:
    virtual std::string stringToLog() = 0;
};

class Observer {
public:
    Observer();

    ~Observer();

    virtual void update(ILoggable* i_loggable) = 0;
};

class Subject {
public:
    Subject();

    ~Subject();

    virtual void attach(Observer* obs);

    virtual void detach(Observer* obs);

    virtual void notify(ILoggable* i_loggable);

protected:
    std::list<Observer*>* observers;
};

class LogObserver : public Observer {
public:
    LogObserver();

    ~LogObserver();

    void update(ILoggable* i_loggable) override;
};
