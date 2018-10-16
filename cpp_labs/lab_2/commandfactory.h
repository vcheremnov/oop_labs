#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <memory>
#include <string>
#include <unordered_map>
#include "command.h"
#include "commandcreator.h"

class CommandFactory {
public:
    std::shared_ptr<Command> get_command(const std::string &cmdName);
    void register_command(const std::string &cmdName, CommandCreator &creator);
    static CommandFactory &instance() {
        static CommandFactory _instance;
        return _instance;
    }
private:
    CommandFactory() = default;
    std::unordered_map<std::string, CommandCreator*> _registry;
};

#endif // COMMANDFACTORY_H
