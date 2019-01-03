#pragma once

class GameModel;

class Object {
public:
    virtual ~Object() = default;
    virtual void render(GameModel*) = 0;
};
