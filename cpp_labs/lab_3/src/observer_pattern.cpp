#include "observer_pattern.h"
#include <stdexcept>

// observer

void Observer::update(Subject *subject) {
    if (subject == _subject) {
        _isUpdated = true;
    }
}

// subject

void Subject::attach(Observer *obs) {
    if (obs == nullptr) {
        throw std::runtime_error("OBSERVER IS NULL...");
    }
    _observers.push_back(obs);
}

void Subject::detach(Observer *obs) {
    _observers.remove(obs);
}

void Subject::notify() {
    for (auto &obs: _observers) {
        obs->update(this);
    }
}
