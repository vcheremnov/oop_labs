#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <utility>
#include "observer_pattern.h"
#include "field.h"

class GameModel: public Subject {
public:
    // constructor
    GameModel() = default;
private:
    std::pair<std::pair<Field, Field>, std::pair<Field, Field>> _fields;
};

#endif // GAMEMODEL_H
