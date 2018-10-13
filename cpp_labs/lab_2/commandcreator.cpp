#include "commandcreator.h"
#include <exception>

// command factory

std::shared_ptr<Command> CommandFactory::get_command(const std::string &cmdName) {
    if (_registry.find(cmdName) == _registry.end()) {
        throw std::runtime_error("Unknown command " + cmdName);
    }
    return _registry[cmdName]->create_command();
}

void CommandFactory::register_command(const std::string &cmdName, CommandCreator *creator) {
    if (creator == nullptr) {
        throw std::runtime_error("Invalid command creator!");
    }
    if (_registry.find(cmdName) == _registry.end()) {
        _registry[cmdName] = creator;
    }
}

// command creators

namespace { // instances of creators

CommentCreator commentCreator;

} // anonymous namespace

// # command

CommentCreator::CommentCreator() {
    auto cmd = create_command();
    CommandFactory::instance().register_command(cmd->get_name(), this);
}
