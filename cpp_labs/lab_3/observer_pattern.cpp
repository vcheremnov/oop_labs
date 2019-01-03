#include "observer_pattern.h"
#include "stdexcept"

// subject

void Subject::notify_observers() {
    for (auto observer: _observers) {
        observer->update(this);
    }
}

void Subject::attach_observer(Observer *observer) {
    if (observer == nullptr) {
        throw std::runtime_error("Subject::attach_observer(..) error: observer is NULL");
    }
    _observers.push_back(observer);
}

void Subject::detach_observer(Observer *observer) {
    _observers.remove(observer);
}

// observer

void Observer::update(Subject *subject) {
    if (subject == _subject) {
        _isUpdated = true;
    }
}

