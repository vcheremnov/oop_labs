#pragma once

#include "gameview.h"
#include "gamestate.h"
#include "eventlistener.h"
#include "player.h"
#include <memory>

class GameModel;

class GameController {
public:
    virtual ~GameController() = default;
    virtual void switch_listener(GameState) = 0;
    void bind_to_human_player(HumanPlayer *player)
        { _humanPlayer = player; }
    void bind_to_bot_player(BotPlayer *player)
        { _botPlayer = player; }
protected:
    HumanPlayer *_get_human_player()
        { return _humanPlayer; }
    BotPlayer *_get_bot_player()
        { return _botPlayer; }
private:
    HumanPlayer *_humanPlayer = nullptr;
    BotPlayer *_botPlayer = nullptr;
};

class ConsoleController: public GameController {
public:
    ConsoleController(GameModel*);
    void switch_listener(GameState) override;
private:
    // handlers registry
    std::map<GameState, std::unique_ptr<EventListener>> _listeners;
};
