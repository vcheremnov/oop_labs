#include "gameview.h"
#include "gamemodel/gamemodel.h"
#include <stdexcept>


GameView::GameView(GameModel *model): Observer(model), _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("GameView::GameView(): model is NULL");
    }
}
