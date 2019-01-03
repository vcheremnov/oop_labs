#pragma once

#include <map>
#include <set>
#include <vector>
#include <memory>
#include "field.h"
#include "gamestate.h"

class GameController;

// base player

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
};

// human player factory

class BaseHumanPlayerCreator {
public:
    virtual ~BaseHumanPlayerCreator() = default;
    virtual HumanPlayer *get_human_player(GameController*) = 0;
};

template<typename ConcreteHumanPlayer>
class HumanPlayerCreator: public BaseHumanPlayerCreator {
public:
    static_assert(std::is_base_of<HumanPlayer, ConcreteHumanPlayer>::value,
                  "Class ConcreteHumanPlayer has to inherit from HumanPlayer");
    HumanPlayer *get_human_player(GameController *controller) override
        { return new ConcreteHumanPlayer(controller); }
};

class HumanPlayerFactory {
public:
    // singleton access
    static HumanPlayerFactory &instance() {
        static HumanPlayerFactory obj;
        return obj;
    }
    // registration
    template<typename ConcreteHumanPlayer>
    void register_human_player(GameType gameType) {
         _registry[gameType].reset(new HumanPlayerCreator<ConcreteHumanPlayer>());
    }
    // factory methods
    HumanPlayer *get_human_player(GameType gameType, GameController *controller)
        { return _registry[gameType]->get_human_player(controller); }
private:
    HumanPlayerFactory() = default;
    HumanPlayerFactory(const HumanPlayerFactory&) = delete;
    HumanPlayerFactory &operator=(const HumanPlayerFactory&) = delete;
    // registry
    std::map<GameType, std::unique_ptr<BaseHumanPlayerCreator>> _registry;
};

// bot player

class BotPlayer: public Player {
public:
    using Player::Player;
protected:
    virtual void _make_move() = 0;
};

// bot player factory

class BaseBotPlayerCreator {
public:
    virtual ~BaseBotPlayerCreator() = default;
    virtual BotPlayer *get_bot_player(GameController*) = 0;
};

template<typename ConcreteBotPlayer>
class BotPlayerCreator: public BaseBotPlayerCreator {
public:
    static_assert(std::is_base_of<BotPlayer, ConcreteBotPlayer>::value,
                  "Class ConcreteBotPlayer has to inherit from BotPlayer");
    BotPlayer *get_bot_player(GameController *controller) override
        { return new ConcreteBotPlayer(controller); }
};

class BotPlayerFactory {
public:
    // singleton access
    static BotPlayerFactory &instance() {
        static BotPlayerFactory obj;
        return obj;
    }
    // registration
    template<typename ConcreteBot>
    void register_bot_player(const std::string &difficulty, const std::string &botName) {
         _registry[difficulty].reset(new BotPlayerCreator<ConcreteBot>());
         _botNames[difficulty] = botName;
         _difficulties.insert(difficulty);
    }
    // factory methods
    BotPlayer *get_bot_player(const std::string &difficulty, GameController *controller)
        { return _registry.at(difficulty)->get_bot_player(controller); }
    const std::string& get_bot_name(const std::string &difficulty) const
        { return _botNames.at(difficulty); }
    std::vector<std::string> get_difficulties() const
        { return std::vector<std::string>(_difficulties.begin(), _difficulties.end()); }
private:
    BotPlayerFactory() = default;
    BotPlayerFactory(const BotPlayerFactory&) = delete;
    BotPlayerFactory &operator=(const BotPlayerFactory&) = delete;
    // registry
    std::map<std::string, std::unique_ptr<BaseBotPlayerCreator>> _registry;
    std::map<std::string, std::string> _botNames;
    std::set<std::string> _difficulties;
};
