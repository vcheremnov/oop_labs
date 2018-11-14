#ifndef PLAYER_H
#define PLAYER_H

#include "gamecontroller.h"

class Action {
public:
    virtual ~Action() = default;
};

class Player {
public:
    virtual ~Player() = default;
    virtual Action get_action(GameController &) = 0;
};

// player types
enum class PlayerType {
    HumanPlayer, BotPlayer
};

// human player

class HumanPlayer: public Player {
public:

};

class ConsolePlayer: public HumanPlayer {
public:

};

class GuiPlayer: public HumanPlayer {
public:

};

// bot player


// AI level
enum class AILevel {
    Easy, Normal, Hard
};

class BotPlayer: public Player {
public:

};

class EasyBotPlayer: public BotPlayer {
public:

};

class NormalBotPlayer: public BotPlayer {
public:

};

class HardBotPlayer: public BotPlayer {
public:

};


#endif // PLAYER_H
