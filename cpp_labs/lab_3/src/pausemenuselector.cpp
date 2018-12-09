#include "pausemenuselector.h"
#include "gamemodel.h"

namespace  {

using Option = PauseMenuSelector::Option;

} // namespace

PauseMenuSelector::PauseMenuSelector(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("PauseMenuSelector::PauseMenuSelector(..): model is NULL");
    }
    _optionNames = {
        {Option::Continue, "Continue"},
        {Option::GotoMainMenu, "Go to Main Menu"},
        {Option::Quit, "Quit"}
    };
}

void PauseMenuSelector::reset() {
    _option = Option::Continue;
}

void PauseMenuSelector::next_option() {
    if (++_option == Option::Total) {
        _option = Option::Continue;
    }
    _model->notify_views();
}

void PauseMenuSelector::prev_option() {
    if (--_option == Option::Total) {
        --_option;
    }
    _model->notify_views();
}

Option &operator++(Option &option) {
    if (option == Option::Total) {
        option = Option::Continue;
    }
    else {
        option = static_cast<Option>(static_cast<int>(option) + 1);
    }
    return option;
}

Option &operator--(Option &option) {
    if (option == Option::Continue) {
        option = Option::Total;
    }
    else {
        option = static_cast<Option>(static_cast<int>(option) - 1);
    }
    return option;
}
