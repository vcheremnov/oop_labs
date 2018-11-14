#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

class GameController {
public:
    virtual ~GameController() = default;
};

class ConsoleController: public GameController {
public:

};

class GuiController: public GameController {
public:

};

#endif // GAMECONTROLLER_H
