#pragma once

#include <vector>
#include <memory>
#include "game/gamestate.h"
#include "object.h"

class GameModel;

class BaseScreen {
public:
    virtual ~BaseScreen() = default;
    virtual void render(GameModel*) = 0;
protected:
    void _add_object(Object *obj)
        { _objects.push_back(std::unique_ptr<Object>(obj)); }
    void _render_objects(GameModel *model)
        { for (auto &objPtr: _objects) objPtr->render(model); }
private:
    std::vector<std::unique_ptr<Object>> _objects;
};
