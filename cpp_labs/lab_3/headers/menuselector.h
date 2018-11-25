#pragma once

#include <vector>
#include <string>
#include <map>

class GameModel;

class MenuSelector {
public:
    enum class Option {
        StartGame, Difficulty, Options, Quit, Total
    };
    enum class Difficulty {
        Easy, Normal, Hard, Total
    };
    MenuSelector(GameModel*);
    void reset();
    void next_option();
    void prev_option();
    void next_difficulty();
    void prev_difficulty();
    Option get_option() const
        { return _option; }
    Difficulty get_difficulty() const
        { return _difficulty; }
    std::string get_option_name(Option);
private:
    GameModel *_model = nullptr;
    Option _option = Option::StartGame;
    Difficulty _difficulty = Difficulty::Easy;
    std::map<Option, std::string> _optionNames;
    std::map<Difficulty, std::string> _difficultyNames;
};

MenuSelector::Option &operator++(MenuSelector::Option&);
MenuSelector::Option &operator--(MenuSelector::Option&);

MenuSelector::Difficulty &operator++(MenuSelector::Difficulty&);
MenuSelector::Difficulty &operator--(MenuSelector::Difficulty&);

