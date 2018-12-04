#include "menuselector.h"
#include "gamemodel.h"

namespace  {

using Option = MenuSelector::Option;

}

MenuSelector::MenuSelector(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("MenuSelector::MenuSelector(..): model is NULL");
    }
    _optionNames = {
        {Option::StartGame, "Start Game"},
        {Option::GameMode, "Mode: "},
        {Option::Difficulty, "Difficulty: "},
        {Option::AI_level_1, "AI level 1: "},
        {Option::AI_level_2, "AI level 2: "},
        {Option::Options, "Options"},
        {Option::Quit, "Quit"}
    };
    _difficultyNames = {
        {Difficulty::Easy, "Easy"},
        {Difficulty::Normal, "Normal"},
        {Difficulty::Hard, "Hard"}
    };
    _gamemodeNames = {
        {GameMode::Player_vs_Bot, "Player-Bot"},
        {GameMode::Player_vs_Player, "Player-Player"},
        {GameMode::Bot_vs_Bot, "Bot-Bot"}
    };
    _activeOptions = {
        {Option::StartGame, true},
        {Option::GameMode, true},
        {Option::Difficulty, true},
        {Option::AI_level_1, true},
        {Option::AI_level_2, true},
        {Option::Options, true},
        {Option::Quit, true}
    };
    _set_inactive_options();
}

void MenuSelector::reset() {
    _option = Option::StartGame;
}

void MenuSelector::next_option() {
    if (++_option == Option::Total) {
        _option = Option::StartGame;
    }
    if (!_activeOptions[_option]) {
        next_option();
    }
    _model->notify_views();
}

void MenuSelector::prev_option() {
    if (--_option == Option::Total) {
        --_option;
    }
    if (!_activeOptions[_option]) {
        prev_option();
    }
    _model->notify_views();
}

void MenuSelector::next_difficulty() {
    if (++_difficulty == Difficulty::Total) {
        _difficulty = Difficulty::Easy;
    }
    _model->notify_views();
}

void MenuSelector::prev_difficulty() {
    if (--_difficulty == Difficulty::Total) {
        --_difficulty;
    }
    _model->notify_views();
}

void MenuSelector::next_gamemode() {
    if (++_gamemode == GameMode::Total) {
        _gamemode = GameMode::Player_vs_Bot;
    }
    _set_inactive_options();
    _model->notify_views();
}

void MenuSelector::prev_gamemode() {
    if (--_gamemode == GameMode::Total) {
        --_gamemode;
    }
    _set_inactive_options();
    _model->notify_views();
}

void MenuSelector::_set_inactive_options() {
    switch (_gamemode) {
    case GameMode::Player_vs_Bot:
        _activeOptions[Option::Difficulty] = true;
        _activeOptions[Option::AI_level_1] = false;
        _activeOptions[Option::AI_level_2] = false;
        break;
    case GameMode::Player_vs_Player:
        _activeOptions[Option::Difficulty] = false;
        _activeOptions[Option::AI_level_1] = false;
        _activeOptions[Option::AI_level_2] = false;
        break;
    case GameMode::Bot_vs_Bot:
        _activeOptions[Option::Difficulty] = false;
        _activeOptions[Option::AI_level_1] = true;
        _activeOptions[Option::AI_level_2] = true;
        break;
    default:
        break;
    }
}

void MenuSelector::next_AI_level_first() {
    if (++_aiLevel1 == Difficulty::Total) {
        _aiLevel1 = Difficulty::Easy;
    }
    _model->notify_views();
}

void MenuSelector::prev_AI_level_first() {
    if (--_aiLevel1 == Difficulty::Total) {
        --_aiLevel1;
    }
    _model->notify_views();
}

void MenuSelector::next_AI_level_second() {
    if (++_aiLevel2 == Difficulty::Total) {
        _aiLevel2 = Difficulty::Easy;
    }
    _model->notify_views();
}

void MenuSelector::prev_AI_level_second() {
    if (--_aiLevel2 == Difficulty::Total) {
        --_aiLevel2;
    }
    _model->notify_views();
}

template<typename T>
void MenuSelector::_next_option_mode(T &option) {

}

template<typename T>
void MenuSelector::_prev_option_mode(T &option) {

}

std::string MenuSelector::get_option_name(Option option) {
    switch (option) {
    case Option::Difficulty:
        return _optionNames[option] + _difficultyNames[_difficulty];
    case Option::GameMode:
        return _optionNames[option] + _gamemodeNames[_gamemode];
    case Option::AI_level_1:
        return _optionNames[option] + _difficultyNames[_aiLevel1];
    case Option::AI_level_2:
        return _optionNames[option] + _difficultyNames[_aiLevel2];
    default:
        break;
    }
    return _optionNames[option];
}

Option &operator++(Option &option) {
    if (option == Option::Total) {
        option = Option::StartGame;
    }
    else {
        option = static_cast<Option>(static_cast<int>(option) + 1);
    }
    return option;
}

Option &operator--(Option &option) {
    if (option == Option::StartGame) {
        option = Option::Total;
    }
    else {
        option = static_cast<Option>(static_cast<int>(option) - 1);
    }
    return option;
}

Difficulty &operator++(Difficulty &difficulty) {
    if (difficulty == Difficulty::Total) {
        difficulty = Difficulty::Easy;
    }
    else {
        difficulty = static_cast<Difficulty>(static_cast<int>(difficulty) + 1);
    }
    return difficulty;
}

Difficulty &operator--(Difficulty &difficulty) {
    if (difficulty == Difficulty::Easy) {
        difficulty = Difficulty::Total;
    }
    else {
        difficulty = static_cast<Difficulty>(static_cast<int>(difficulty) - 1);
    }
    return difficulty;
}

GameMode &operator++(GameMode &gamemode) {
    if (gamemode == GameMode::Total) {
        gamemode = GameMode::Player_vs_Bot;
    }
    else {
        gamemode = static_cast<GameMode>(static_cast<int>(gamemode) + 1);
    }
    return gamemode;
}

GameMode &operator--(GameMode &gamemode) {
    if (gamemode == GameMode::Player_vs_Bot) {
        gamemode = GameMode::Total;
    }
    else {
        gamemode = static_cast<GameMode>(static_cast<int>(gamemode) - 1);
    }
    return gamemode;
}
