#pragma once

#include <list>

class Observer;

class Subject {
public:
    virtual ~Subject() = default;
    void notify_observers();
    void attach_observer(Observer*);
    void detach_observer(Observer*);
private:
    std::list<Observer*> _observers;
};

class Observer {
public:
    Observer(Subject *subject):
        _subject(subject) {}
    virtual ~Observer() = default;
    void update(Subject*);
protected:
    bool _is_updated()
        { return _isUpdated; }
    void _reset_update()
        { _isUpdated = false; }
private:
    Subject *_subject = nullptr;
    bool _isUpdated = false;
};
