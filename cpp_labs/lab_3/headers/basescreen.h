#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <map>
#include <memory>
#include "consolewindow.h"
#include "gamestate.h"
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

class ConsoleScreen: public BaseScreen {
protected:
    void _load_background(const std::string &filename);
    const std::string &_get_background()
        { return _background; }
private:
    std::string _background;
};

// screen factory

class BaseScreenCreator {
public:
    virtual ~BaseScreenCreator() = default;
    virtual ConsoleScreen *get_screen() = 0;
};

template<typename Screen>
class ConsoleScreenCreator: public BaseScreenCreator {
public:
    static_assert(std::is_base_of<ConsoleScreen, Screen>::value,
                  "Class Screen has to inherit from ConsoleScreen");
    ConsoleScreen *get_screen() override
        { return new Screen(); }
};

class ScreenFactory {
public:
    // singleton access
    static ScreenFactory &instance() {
        static ScreenFactory obj;
        return obj;
    }
    // registration
    template<typename Screen>
    void register_screen(GameState gameState) {
         _registry[gameState].reset(new ConsoleScreenCreator<Screen>());
    }
    // factory methods
    ConsoleScreen *get_screen(GameState gameState)
        { return _registry[gameState]->get_screen(); }
    void get_all_screens(std::map<GameState, std::unique_ptr<ConsoleScreen>> &screens) {
        for (auto &regItem: _registry) {
            screens[regItem.first].reset(get_screen(regItem.first));
        }
    }
private:
    ScreenFactory() = default;
    ScreenFactory(const ScreenFactory&) = delete;
    ScreenFactory &operator=(const ScreenFactory&) = delete;
    // registry
    std::map<GameState, std::unique_ptr<BaseScreenCreator>> _registry;
};

