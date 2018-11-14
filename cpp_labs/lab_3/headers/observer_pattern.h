#ifndef OBSERVER_PATTERN_H
#define OBSERVER_PATTERN_H

#include <list>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    void update(Subject*);
private:
    Subject *_subject = nullptr;
    bool _isUpdated = false;
};

class Subject {
public:
    virtual ~Subject() = default;
    void attach(Observer*);
    void detach(Observer*);
protected:
    void notify();
private:
    std::list<Observer*> _observers;
};

#endif // OBSERVER_PATTERN_H
