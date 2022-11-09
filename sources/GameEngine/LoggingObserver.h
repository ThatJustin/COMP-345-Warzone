//
// Created by Alex De Luca on 2022-11-03.
//

#ifndef COMP_345_WARZONE_LOGGINGOBSERVER_H
#define COMP_345_WARZONE_LOGGINGOBSERVER_H

#include "CommandProcessor.h"
#include <string>
#include <string.h>
#include <list>
using namespace std;
class ILoggable{
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
    virtual void notify()=0;
private:
    list<*Observer> *observers;
};

class LogObserver : public Observer{
public:
    LogObserver();
    ~LogObserver();

    string getFilename();
    void setFilename(string filename);

    void update(ILoggable *ilog);
private:
    string FILENAME;


};
#endif //COMP_345_WARZONE_LOGGINGOBSERVER_H
