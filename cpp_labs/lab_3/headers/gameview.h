#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "observer_pattern.h"

class GameView: public Observer {
public:
};

class ConsoleView: public GameView {

};

class GuiView: public GameView {

};

#endif // GAMEVIEW_H
