#include <map>
#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include "board.h"

namespace {

// move info panel

class MoveInfoPanel: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
private:
    static const std::map<MoveMaker::MoveResult, std::string> _messages;
    static const std::string _moveMsg;
    static const std::string _proceedMsg;
};

const std::string MoveInfoPanel::_moveMsg = "Move of the player:";
const std::string MoveInfoPanel::_proceedMsg = "ENTER to continue";

const std::map<MoveMaker::MoveResult, std::string> MoveInfoPanel::_messages = {
    {MoveMaker::MoveResult::Destroyed, "Destroyed!"},
    {MoveMaker::MoveResult::Hit, "Hit!"},
    {MoveMaker::MoveResult::Miss, "Miss!"},
};

void MoveInfoPanel::_draw_object(GameModel *model) {
    auto &moveMaker = model->move_maker();
    auto moveResult = moveMaker.last_move_result();
    auto window = _get_window();
    auto panelWidth = window->get_width();

    if (moveMaker.move_was_made()) {
        // print message
        auto &msg = _messages.at(moveResult);
        window->set_attributes({TextAttr::Bold});
        window->print_text_at(1, (panelWidth - msg.length()) / 2, msg.c_str());
        window->reset_attributes();
        // print prompt
        window->set_attributes({TextAttr::Blink});
        window->print_text_at(2, (panelWidth - _proceedMsg.length()) / 2, _proceedMsg.c_str());
    }
    else {
        auto &playerName = model->get_player_name(model->get_active_player());
        window->print_text_at(1, (panelWidth - _moveMsg.length()) / 2, _moveMsg.c_str());
        window->set_attributes({TextAttr::Bold});
        window->print_text_at(2, (panelWidth - playerName.length()) / 2, playerName.c_str());
    }
    window->reset_attributes();
}

// action button

class ActionButton: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
    static const std::string _continueMsg;
    static const std::string _makeShotMsg;
};

const std::string ActionButton::_continueMsg = "Continue...";
const std::string ActionButton::_makeShotMsg = "Make shot";


void ActionButton::_draw_object(GameModel *model) {
    auto &moveMaker = model->move_maker();
    auto window = _get_window();
    auto buttonWidth = window->get_width();

    if (moveMaker.move_was_made()) {
        window->set_attributes({TextAttr::Highlight});
        window->print_text_at(1, (buttonWidth - _continueMsg.length()) / 2, _continueMsg.c_str());
    }
    else {
        if (moveMaker.move_is_valid()) {
            window->set_attributes({TextAttr::Highlight});
        }
        else {
            window->set_attributes({TextAttr::Dim});
        }
        window->print_text_at(1, (buttonWidth - _makeShotMsg.length()) / 2, _makeShotMsg.c_str());
    }
    window->reset_attributes();
}

// battle screen

class BattleScreen: public ConsoleScreen {
public:
    BattleScreen() {
        auto screenWidth = ConsoleView::CONSOLE_WIDTH;
        auto screenHeight = ConsoleView::CONSOLE_HEIGHT;
        // add battle board
        int boardWidth = Field::WIDTH * Board::CELL_WIDTH + 3;
        int boardHeight = Field::HEIGHT * Board::CELL_HEIGHT + 3;
        int boardBegX = 4;
        int boardBegY = 1;
        _add_object(new BattleBoard(boardWidth, boardHeight,
                                    boardBegY, boardBegX));
        // add player's mini-board
        int miniBoardWidth = Field::WIDTH * MiniBoard::CELL_WIDTH + 3;
        int miniBoardHeight = Field::HEIGHT * MiniBoard::CELL_HEIGHT + 2;
        int freeSpaceWidth = screenWidth - (boardBegX + boardWidth);
        int miniBoardBegX = boardBegX + boardWidth + (freeSpaceWidth - miniBoardWidth) / 2;
        int miniBoardBegY = boardBegY;
        _add_object(new MiniBoard(miniBoardWidth, miniBoardHeight,
                                  miniBoardBegY, miniBoardBegX));
        // add move info panel
        int infoPanelWidth = miniBoardWidth;
        int infoPanelHeight = 4;
        int infoPanelBegX = miniBoardBegX;
        int infoPanelBegY = miniBoardBegY + miniBoardHeight + 1;
        _add_object(new MoveInfoPanel(infoPanelWidth, infoPanelHeight,
                                      infoPanelBegY, infoPanelBegX));
        // add action button
        int buttonHeight = 3;
        int buttonWidth = infoPanelWidth;
        int buttonBegX = infoPanelBegX;
        int buttonBegY = screenHeight - buttonHeight - 1;
        _add_object(new ActionButton(buttonWidth, buttonHeight,
                                     buttonBegY, buttonBegX));
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<BattleScreen>(GameState::Battle);
    return true;
}

bool screenReg = register_screen();

}
