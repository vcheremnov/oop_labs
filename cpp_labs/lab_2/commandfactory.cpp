#include <sstream>
#include <iomanip>
#include "commandfactory.h"

CommandFactory::CommandList CommandFactory::get_command_list() {
    CommandList cmdList;
    for (const auto &mapPair: _registry) {
        cmdList.push_back(mapPair.first);
    }
    return cmdList;
}

const std::string &CommandFactory::command_info(const std::string &cmdName) {
    if (_registry.find(cmdName) == _registry.end()) {
        std::stringstream msgStream;
        msgStream << "Unknown command " << std::quoted(cmdName);
        throw CommandError::UnknownCommand(msgStream.str());
    }
    return _registry[cmdName]->command_info();
}

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

void CommandFactory::forget_command(const std::string &cmdName) {
    _registry.erase(cmdName);
}
