#include "menu.h"
#include <ncurses.h>

void ConsoleMenu::_draw_object(GameModel *model) {
    // draw header
    auto window = _get_window();
    window->set_attributes({TextAttr::Bold});
    window->print_text_at(1, (window->get_width() - _get_header().length()) / 2, _get_header().c_str());
    window->reset_attributes();
    for (auto posX = 0; posX < window->get_width(); ++posX) {
        window->add_character_at(2, posX, ACS_HLINE);
    }
    // list menu options
    _draw_options(model);
}
