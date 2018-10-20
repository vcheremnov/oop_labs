#include <sstream>
#include <iomanip>
#include "commandfactory.h"

std::shared_ptr<Command> CommandFactory::get_command(const std::string &cmdName) {
    if (_registry.find(cmdName) == _registry.end()) {
        std::stringstream msgStream;
        msgStream << "Unknown command " << std::quoted(cmdName);
        throw CommandError::UnknownCommand(msgStream.str());
    }
    return _registry[cmdName]->create_command();
}

void CommandFactory::register_command(const std::string &cmdName, CommandCreator &creator) {
    if (_registry.find(cmdName) == _registry.end()) {
        _registry[cmdName] = &creator;
    }
}

void CommandFactory::deregister_command(const std::string &cmdName) {
    // reconsider! (possibly can erase other elements with the equivalent keys)
    _registry.erase(cmdName);
}
