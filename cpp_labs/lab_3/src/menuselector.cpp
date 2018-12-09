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
        {Option::Controls, "Controls"},
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
    auto difficulty = _model->game_data().get_difficulty();
    if (++difficulty == Difficulty::Total) {
        difficulty = Difficulty::Easy;
    }
    _model->game_data()._set_difficulty(difficulty);
    _model->notify_views();
}

void MenuSelector::prev_difficulty() {
    auto difficulty = _model->game_data().get_difficulty();
    if (--difficulty == Difficulty::Total) {
        --difficulty;
    }
    _model->game_data()._set_difficulty(difficulty);
    _model->notify_views();
}

void MenuSelector::next_gamemode() {
    auto gameMode = _model->game_data().get_gamemode();
    if (++gameMode == GameMode::Total) {
        gameMode = GameMode::Player_vs_Bot;
    }
    _model->game_data()._set_gamemode(gameMode);
    _set_inactive_options();
    _model->notify_views();
}

void MenuSelector::prev_gamemode() {
    auto gameMode = _model->game_data().get_gamemode();
    if (--gameMode == GameMode::Total) {
        --gameMode;
    }
    _model->game_data()._set_gamemode(gameMode);
    _set_inactive_options();
    _model->notify_views();
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
    auto aiLevel1 = _model->game_data().get_AI_level(PlayerNumber::Player1);
    if (++aiLevel1 == Difficulty::Total) {
        aiLevel1 = Difficulty::Easy;
    }
    _model->game_data()._set_AI_level(PlayerNumber::Player1, aiLevel1);
    _model->notify_views();
}

void MenuSelector::prev_AI_level_first() {
    auto aiLevel1 = _model->game_data().get_AI_level(PlayerNumber::Player1);
    if (--aiLevel1 == Difficulty::Total) {
        --aiLevel1;
    }
    _model->game_data()._set_AI_level(PlayerNumber::Player1, aiLevel1);
    _model->notify_views();
}

void MenuSelector::next_AI_level_second() {
    auto aiLevel2 = _model->game_data().get_AI_level(PlayerNumber::Player2);
    if (++aiLevel2 == Difficulty::Total) {
        aiLevel2 = Difficulty::Easy;
    }
    _model->game_data()._set_AI_level(PlayerNumber::Player2, aiLevel2);
    _model->notify_views();
}

void MenuSelector::prev_AI_level_second() {
    auto aiLevel2 = _model->game_data().get_AI_level(PlayerNumber::Player2);
    if (--aiLevel2 == Difficulty::Total) {
        --aiLevel2;
    }
    _model->game_data()._set_AI_level(PlayerNumber::Player2, aiLevel2);
    _model->notify_views();
}

std::string MenuSelector::get_option_name(Option option) {
    auto &gameData = _model->game_data();
    switch (option) {
    case Option::Difficulty:
        return _optionNames[option] + _difficultyNames[gameData.get_difficulty()];
    case Option::GameMode:
        return _optionNames[option] + _gamemodeNames[gameData.get_gamemode()];
    case Option::AI_level_1:
        return _optionNames[option] + _difficultyNames[gameData.get_AI_level(PlayerNumber::Player1)];
    case Option::AI_level_2:
        return _optionNames[option] + _difficultyNames[gameData.get_AI_level(PlayerNumber::Player2)];
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
