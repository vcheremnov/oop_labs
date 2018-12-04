#pragma once

#include <vector>
#include <string>
#include <map>
#include "gamestate.h"

class GameModel;

class MenuSelector {
public:
    enum class Option {
        StartGame, GameMode, Difficulty, AI_level_1, AI_level_2, Options, Quit, Total
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
    Difficulty get_difficulty() const
        { return _difficulty; }
    Difficulty get_AI_level_first() const
        { return _aiLevel1; }
    Difficulty get_AI_level_second() const
        { return _aiLevel2; }
    GameMode get_gamemode() const
        { return _gamemode; }
    std::string get_option_name(Option);
private:
    template<typename T>
    void _next_option_mode(T&);
    template<typename T>
    void _prev_option_mode(T&);
    GameModel *_model = nullptr;
    Option _option = Option::StartGame;
    Difficulty _difficulty = Difficulty::Easy;
    Difficulty _aiLevel1 = Difficulty::Easy, _aiLevel2 = Difficulty::Easy;
    GameMode _gamemode = GameMode::Player_vs_Bot;
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


