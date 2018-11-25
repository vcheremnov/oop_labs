#include "headers/menuselector.h"

namespace  {

using Option = MenuSelector::Option;
using Difficulty = MenuSelector::Difficulty;

}

MenuSelector::MenuSelector() {
    _optionNames = {
        {Option::StartGame, "Start Game"},
        {Option::Difficulty, "Difficulty: "},
        {Option::Options, "Options"},
        {Option::Quit, "Quit"}
    };
    _difficultyNames = {
        {Difficulty::Easy, "Easy"},
        {Difficulty::Normal, "Normal"},
        {Difficulty::Hard, "Hard"}
    };
}

void MenuSelector::reset() {
    _option = Option::StartGame;
}

void MenuSelector::next_option() {
    if (++_option == Option::Total) {
        _option = Option::StartGame;
    }
}

void MenuSelector::prev_option() {
    if (--_option == Option::Total) {
        --_option;
    }
}

void MenuSelector::next_difficulty() {
    if (++_difficulty == Difficulty::Total) {
        _difficulty = Difficulty::Easy;
    }
}

void MenuSelector::prev_difficulty() {
    if (--_difficulty == Difficulty::Total) {
        --_difficulty;
    }
}

std::string MenuSelector::get_option_name(Option option) {
    if (option == Option::Difficulty) {
        return _optionNames[option] + _difficultyNames[_difficulty];
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
