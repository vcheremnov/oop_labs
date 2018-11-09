#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "commandfactory.h"

namespace {

bool _isspace(char ch) {
    return std::isspace(static_cast<unsigned char>(ch));
}

} // anonymous namespace

bool CommandFactory::_is_valid_cmdname(const std::string &cmdName) {
    if (cmdName.empty()) {
        return false;
    }
    return std::find_if(cmdName.cbegin(), cmdName.cend(), _isspace) == cmdName.cend();
}

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
