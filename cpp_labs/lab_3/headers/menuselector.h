#pragma once

#include <vector>
#include <string>
#include <map>
#include "gamestate.h"

class GameModel;

class MenuSelector {
public:
    enum class Option {
        StartGame, GameMode, Difficulty,
        AI_level_1, AI_level_2, Controls, Quit, Total
    };
    MenuSelector(GameModel*);
    void reset();
    void next_option();
    void prev_option();
    void next_gamemode();
    void prev_gamemode();
    void next_difficulty();
    void prev_difficulty();
    void next_AI_level_first();
    void prev_AI_level_first();
    void next_AI_level_second();
    void prev_AI_level_second();
    Option get_option() const
        { return _option; }
    bool is_active_option(Option opt) const
        { return _activeOptions.at(opt); }
    std::string get_option_name(Option);
    // input mode
    void turn_input_mode_on()
        { _inputMode = true; }
    void turn_input_mode_off()
        { _inputMode = false; }
    bool input_mode_is_on()
        { return _inputMode; }
private:
    GameModel *_model = nullptr;
    bool _inputMode = false;
    Option _option = Option::StartGame;
    std::map<Option, std::string> _optionNames;
    std::map<Difficulty, std::string> _difficultyNames;
    std::map<GameMode, std::string> _gamemodeNames;
    std::map<Option, bool> _activeOptions;

    void _set_inactive_options();
};

MenuSelector::Option &operator++(MenuSelector::Option&);
MenuSelector::Option &operator--(MenuSelector::Option&);

Difficulty &operator++(Difficulty&);
Difficulty &operator--(Difficulty&);

GameMode &operator++(GameMode&);
GameMode &operator--(GameMode&);


