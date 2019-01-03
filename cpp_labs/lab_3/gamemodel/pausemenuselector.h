#pragma once

#include <vector>
#include <string>
#include <map>
#include "game/gamestate.h"

class GameModel;

class PauseMenuSelector {
public:
    enum class Option {
        Continue, GotoMainMenu, Quit, Total
    };
    PauseMenuSelector(GameModel*);
    void reset();
    void next_option();
    void prev_option();
    Option get_option() const
        { return _option; }
    std::string get_option_name(Option option) const
        { return _optionNames.at(option); }
private:
    GameModel *_model = nullptr;
    Option _option = Option::Continue;
    std::map<Option, std::string> _optionNames;
};

PauseMenuSelector::Option &operator++(PauseMenuSelector::Option&);
PauseMenuSelector::Option &operator--(PauseMenuSelector::Option&);
