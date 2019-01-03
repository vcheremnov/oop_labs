#pragma once

#include <map>
#include "gameview/basescreen.h"
#include "consolewindow.h"

class ConsoleScreen: public BaseScreen {
public:
    void render(GameModel*) override;
    void set_console_window(ConsoleWindow *win)
        { _win = win; }
protected:
    ConsoleWindow *_get_console_window()
        { return _win; }
    void _load_background(const std::string &filename);
    const std::string &_get_background()
        { return _background; }
private:
    ConsoleWindow *_win = nullptr;
    std::string _background;
};

// console screen factory

class BaseConsoleScreenCreator {
public:
    virtual ~BaseConsoleScreenCreator() = default;
    virtual ConsoleScreen *get_screen() = 0;
};

template<typename Screen>
class ConsoleScreenCreator: public BaseConsoleScreenCreator {
public:
    static_assert(std::is_base_of<ConsoleScreen, Screen>::value,
                  "Class Screen has to inherit from ConsoleScreen");
    ConsoleScreen *get_screen() override
        { return new Screen(); }
};

class ConsoleScreenFactory {
public:
    // singleton access
    static ConsoleScreenFactory &instance() {
        static ConsoleScreenFactory obj;
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
    ConsoleScreenFactory() = default;
    ConsoleScreenFactory(const ConsoleScreenFactory&) = delete;
    ConsoleScreenFactory &operator=(const ConsoleScreenFactory&) = delete;
    // registry
    std::map<GameState, std::unique_ptr<BaseConsoleScreenCreator>> _registry;
};
