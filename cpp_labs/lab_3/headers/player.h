#pragma once

#include "gamecontroller.h"
#include <map>
#include <vector>
#include "field.h"

class Player {
public:
    Player(GameController*);
    virtual ~Player() = default;
    virtual void wait_event() = 0;
protected:
    GameController *_get_controller()
        { return _controller; }
private:
    GameController *_controller = nullptr;
};

// human player

class HumanPlayer: public Player {
public:
    using Player::Player;
    void wait_event() override;
};

// bot player

class BotPlayer: public Player {
public:
    using Player::Player;
    void wait_event() override;
protected:
    virtual void _init_shooting_sequence() = 0;
    virtual void _make_move() = 0;
};

// bot player factory

class BaseBotCreator {
public:
    virtual ~BaseBotCreator() = default;
    virtual BotPlayer *get_bot_player(GameController*) = 0;
};

template<typename ConcreteBot>
class ConcreteBotCreator: public BaseBotCreator {
public:
    static_assert(std::is_base_of<BotPlayer, ConcreteBot>::value,
                  "Class ConcreteBot has to inherit from BotPlayer");
    BotPlayer *get_bot_player(GameController *controller) override
        { return new ConcreteBot(controller); }
};

class BotFactory {
public:
    // singleton access
    static BotFactory &instance() {
        static BotFactory obj;
        return obj;
    }
    // registration
    template<typename ConcreteBot>
    void register_bot_player(Difficulty difficulty) {
         _registry[difficulty].reset(new ConcreteBotCreator<ConcreteBot>());
    }
    // factory methods
    BotPlayer *get_bot_player(Difficulty difficulty, GameController *controller)
        { return _registry[difficulty]->get_bot_player(controller); }
private:
    BotFactory() = default;
    BotFactory(const BotFactory&) = delete;
    BotFactory &operator=(const BotFactory&) = delete;
    // registry
    std::map<Difficulty, std::unique_ptr<BaseBotCreator>> _registry;
};
