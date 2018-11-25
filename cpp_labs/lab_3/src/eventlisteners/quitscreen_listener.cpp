#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class QuitScreenListener: public EventListener {
public:

};

bool register_listener() {
    ListenerFactory::instance().register_listener<QuitScreenListener>(GameState::QuitScreen);
    return true;
}

bool regHandlers = register_listener();

}
