#include "consoleobject.h"

void ConsoleObject::render(GameModel *model) {
    auto window = _get_window();
    window->clear();
    _draw_object(model);
    window->draw_border();
    window->refresh();
}
