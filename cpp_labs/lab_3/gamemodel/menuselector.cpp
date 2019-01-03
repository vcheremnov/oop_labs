#include "menuselector.h"
#include "gamemodel.h"

namespace  {

using Option = MenuSelector::Option;

}

MenuSelector::MenuSelector(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("MenuSelector::MenuSelector(..): model is NULL");
    }
    _difficultiesNum = _model->game_data()._difficulties.size();
    if (_difficultiesNum == 0) {
        throw std::runtime_error("MenuSelector::MenuSelector(..): difficulties are absent");
    }
    _optionNames = {
        {Option::StartGame, "Start Game"},
        {Option::GameMode, "Mode: "},
        {Option::Difficulty, "Difficulty: "},
        {Option::AI_level_1, "AI level 1: "},
        {Option::AI_level_2, "AI level 2: "},
        {Option::Controls, "Controls"},
        {Option::Quit, "Quit"}
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
        {Option::Controls, true},
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
    _model->notify_observers();
}

void MenuSelector::prev_option() {
    if (--_option == Option::Total) {
        --_option;
    }
    if (!_activeOptions[_option]) {
        prev_option();
    }
    _model->notify_observers();
}

void MenuSelector::next_difficulty() {
    auto &difficultyIndex = _model->game_data()._difficultyIndex;
    difficultyIndex = (difficultyIndex + 1) % _difficultiesNum;
    _model->notify_observers();
}

void MenuSelector::prev_difficulty() {
    auto &difficultyIndex = _model->game_data()._difficultyIndex;
    difficultyIndex == 0 ? (difficultyIndex = _difficultiesNum - 1) : --difficultyIndex;
    _model->notify_observers();
}

void MenuSelector::next_gamemode() {
    auto gameMode = _model->game_data().get_gamemode();
    if (++gameMode == GameMode::Total) {
        gameMode = GameMode::Player_vs_Bot;
    }
    _model->game_data()._set_gamemode(gameMode);
    _set_inactive_options();
    _model->notify_observers();
}

void MenuSelector::prev_gamemode() {
    auto gameMode = _model->game_data().get_gamemode();
    if (--gameMode == GameMode::Total) {
        --gameMode;
    }
    _model->game_data()._set_gamemode(gameMode);
    _set_inactive_options();
    _model->notify_observers();
}

void MenuSelector::_set_inactive_options() {
    switch (_model->game_data().get_gamemode()) {
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
    auto &aiLevel1 = _model->game_data()._aiLevel1;
    aiLevel1 = (aiLevel1 + 1) % _difficultiesNum;
    _model->notify_observers();
}

void MenuSelector::prev_AI_level_first() {
    auto &aiLevel1 = _model->game_data()._aiLevel1;
    aiLevel1 == 0 ? (aiLevel1 = _difficultiesNum - 1) : --aiLevel1;
    _model->notify_observers();
}

void MenuSelector::next_AI_level_second() {
    auto &aiLevel2 = _model->game_data()._aiLevel2;
    aiLevel2 = (aiLevel2 + 1) % _difficultiesNum;
    _model->notify_observers();
}

void MenuSelector::prev_AI_level_second() {
    auto &aiLevel2 = _model->game_data()._aiLevel2;
    aiLevel2 == 0 ? (aiLevel2 = _difficultiesNum - 1) : --aiLevel2;
    _model->notify_observers();
}

std::string MenuSelector::get_option_name(Option option) {
    auto &gameData = _model->game_data();
    switch (option) {
    case Option::Difficulty:
        return _optionNames[option] + gameData.get_difficulty();
    case Option::GameMode:
        return _optionNames[option] + _gamemodeNames[gameData.get_gamemode()];
    case Option::AI_level_1:
        return _optionNames[option] + gameData.get_ai_level_1();
    case Option::AI_level_2:
        return _optionNames[option] + gameData.get_ai_level_2();
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
