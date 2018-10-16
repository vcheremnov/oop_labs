#include "commandfactory.h"

std::shared_ptr<Command> CommandFactory::get_command(const std::string &cmdName) {
    if (_registry.find(cmdName) == _registry.end()) {
        throw CommandError::UnknownCommand("Unknown command \"" + cmdName + "\"");
    }
    return _registry[cmdName]->create_command();
}

void CommandFactory::register_command(const std::string &cmdName, CommandCreator &creator) {
    if (_registry.find(cmdName) == _registry.end()) {
        _registry[cmdName] = &creator;
    }
}
