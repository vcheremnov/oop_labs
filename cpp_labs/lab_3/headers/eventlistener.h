#pragma once

#include "event.h"
#include "gamestate.h"
#include <memory>
#include <map>

class GameModel;

// listeners

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void event_occurred(const Event&) {}
    virtual void key_event_occurred(const KeyEvent&) {}
    virtual void mouse_event_occurred(const MouseEvent&) {}
    virtual void bot_move_occurred(const BotMoveEvent&) {}
    void bind_to_model(GameModel *model)
        { _model = model; }
protected:
    GameModel *_get_model()
        { return _model; }
private:
    GameModel *_model = nullptr;
};

// listener creator

class BaseListenerCreator {
public:
    virtual ~BaseListenerCreator() = default;
    virtual EventListener *get_listener() = 0;
};

template<typename Listener>
class ListenerCreator: public BaseListenerCreator {
public:
    static_assert(std::is_base_of<EventListener, Listener>::value,
                  "Class Listener has to inherit from PlayerListener");
    EventListener *get_listener()
        { return new Listener(); }
};

// listener factory

class ListenerFactory {
public:
    // singleton access
    static ListenerFactory &instance() {
        static ListenerFactory obj;
        return obj;
    }
    // registration
    template<typename Listener>
    void register_listener(GameState gameState) {
         _registry[gameState].reset(new ListenerCreator<Listener>());
    }
    // factory methods
    EventListener *get_listener(GameState gameState)
        { return _registry[gameState]->get_listener(); }
    void get_all_listeners(std::map<GameState, std::unique_ptr<EventListener>> &listeners) {
        for (auto &regItem: _registry) {
            listeners[regItem.first].reset(get_listener(regItem.first));
        }
    }
private:
    ListenerFactory() = default;
    ListenerFactory(const ListenerFactory&) = delete;
    ListenerFactory &operator=(const ListenerFactory&) = delete;
    // registry
    std::map<GameState, std::unique_ptr<BaseListenerCreator>> _registry;
};
