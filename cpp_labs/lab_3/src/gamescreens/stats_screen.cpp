#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include <sstream>

namespace {

// stats table

class StatsTable: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
    void bind_to_player(PlayerNumber player)
        { _player = player; }
protected:
    void _draw_object(GameModel*) override;
    void _draw_header(GameModel*);
    void _draw_result(GameModel*);
    void _draw_table_line(int line, const std::string &lineName, const std::string &lineValue);
private:
    const std::string _winResult = "*** WINNER ***";
    const std::string _loseResult = "~~~ LOSER ~~~";
    PlayerNumber _player = PlayerNumber::Player1;
    static const int _startY = 3;
};

void StatsTable::_draw_object(GameModel *model) {
    // header
    _draw_header(model);

    // draw table lines
    std::ostringstream strbuf;
    auto &playerStats = model->player_stats(_player);
    int line = _startY;
    // ships destroyed
    strbuf << playerStats.get_destroyed_ships() << " / " << ShipInitializer::SHIPS_TOTAL;
    _draw_table_line(line, "Ships destroyed", strbuf.str());
    line += 2;
    strbuf.str("");
    strbuf.clear();
    // hits
    strbuf << playerStats.get_hits();
    _draw_table_line(line, "Hits", strbuf.str());
    line += 2;
    strbuf.str("");
    strbuf.clear();
    // misses
    strbuf << playerStats.get_misses();
    _draw_table_line(line, "Misses", strbuf.str());
    line += 2;
    strbuf.str("");
    strbuf.clear();
    // accuracy
    strbuf << playerStats.get_accuracy() << "%%";
    _draw_table_line(line, "Accuracy", strbuf.str());

    // fix last vertical separator
    _get_window()->add_character_at(line + 1, _get_window()->get_width() / 2, ACS_BTEE);

    // result
    _draw_result(model);
}

void StatsTable::_draw_header(GameModel *model) {
    // draw header
    auto window = _get_window();
    auto &playerName = model->game_data().get_player_name(_player);
    window->set_attributes({TextAttr::Bold});
    window->print_text_at(1, (window->get_width() - playerName.length()) / 2, playerName.c_str());
    window->reset_attributes();
    for (auto posX = 0; posX < window->get_width(); ++posX) {
        window->add_character_at(2, posX, ACS_HLINE);
    }
    window->add_character_at(_startY - 1, window->get_width() / 2, ACS_TTEE);
}

void StatsTable::_draw_result(GameModel *model) {
    auto window = _get_window();
    auto tableHeight = window->get_height();
    auto tableWidth = window->get_width();
    bool playerIsWinner = model->player_stats(_player).is_winner();
    const std::string &result = playerIsWinner ? _winResult : _loseResult;

    // draw separating horizontal line
    for (auto posX = 0; posX < window->get_width(); ++posX) {
        window->add_character_at(tableHeight - 3, posX, ACS_HLINE);
    }
    // set colors and attributes
    if (playerIsWinner) {
        window->set_color_pair(TextColor::GREEN_ON_BLACK);
    }
    else {
        window->set_color_pair(TextColor::RED_ON_BLACK);
    }
    window->set_attributes({TextAttr::Bold});
    // print result
    window->print_text_at(tableHeight - 2, (tableWidth - result.length()) / 2, result.c_str());

    window->reset_attributes();
    window->reset_color_pair();
}

void StatsTable::_draw_table_line(int line, const std::string &lineName, const std::string &lineValue) {
    auto window = _get_window();
    auto tableHeight = window->get_height();
    auto tableWidth = window->get_width();
    auto columnWidth = tableWidth / 2;

    // print values
    window->print_text_at(line, (columnWidth - lineName.length()) / 2, lineName.c_str());
    window->print_text_at(line, columnWidth + (columnWidth - lineValue.length()) / 2, lineValue.c_str());

    // draw separating horizontal line
    for (auto posX = 0; posX < tableWidth; ++posX) {
        window->add_character_at(line + 1, posX, ACS_HLINE);
    }
    // draw vertical separator
    window->add_character_at(line, tableWidth / 2, ACS_VLINE);
    window->add_character_at(line + 1, tableWidth / 2, ACS_PLUS);
}

// continue button

class ContinueButton: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
private:
    const std::string _buttonText = "Continue";
};

void ContinueButton::_draw_object(GameModel *model) {
    auto window = _get_window();
    auto buttonWidth = window->get_width();
    auto buttonHeight = window->get_height();

    window->set_attributes({TextAttr::Highlight});
    window->print_text_at(buttonHeight / 2, (buttonWidth - _buttonText.length()) / 2, _buttonText.c_str());
    window->reset_attributes();
}

class StatsScreen: public ConsoleScreen {
public:
    StatsScreen() {
        _load_background("ascii/stats");

        // add stats tables
        auto screenWidth = ConsoleView::CONSOLE_WIDTH;
        auto screenHeight = ConsoleView::CONSOLE_HEIGHT;
        int tableWidth = 35, tableHeight = 20;
        int tableMargin = 4;
        int leftTablePosX = (screenWidth - (2 * tableWidth + tableMargin)) / 2;
        int rightTablePosX = leftTablePosX + tableWidth + tableMargin;
        int leftTablePosY = 1, rightTablePosY = leftTablePosY;

        StatsTable *leftTable = new StatsTable(tableWidth, tableHeight, leftTablePosY, leftTablePosX);
        StatsTable *rightTable = new StatsTable(tableWidth, tableHeight, rightTablePosY, rightTablePosX);

        leftTable->bind_to_player(PlayerNumber::Player1);
        rightTable->bind_to_player(PlayerNumber::Player2);

        _add_object(leftTable);
        _add_object(rightTable);

        // add continue button
        int buttonWidth = 16;
        int buttonHeight = 3;
        _add_object(new ContinueButton(buttonWidth, buttonHeight,
                                       leftTablePosY + tableHeight, (screenWidth - buttonWidth) / 2));
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<StatsScreen>(GameState::Stats);
    return true;
}

bool screenReg = register_screen();

}
