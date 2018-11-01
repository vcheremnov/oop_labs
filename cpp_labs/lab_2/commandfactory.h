#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include "command.h"

// abstract command creator

class BaseCommandCreator {
public:
    virtual ~BaseCommandCreator() = default;
    virtual const std::string &command_name() const = 0;
    virtual const std::string &command_info() const = 0;
    virtual std::shared_ptr<Command> create_command() const = 0;
};

// template concrete command creator

template<typename Cmd>
class CommandCreator: public BaseCommandCreator {
    static_assert(std::is_base_of<Command, Cmd>::value, "Class Cmd has to inherit from the class Command");
public:
    CommandCreator(const std::string &cmdName, const std::string &cmdInfo):
        _commandName(cmdName), _commandInfo(cmdInfo) {}
    const std::string &command_name() const {
        return _commandName;
    }
    const std::string &command_info() const {
        return _commandInfo;
    }
    std::shared_ptr<Command> create_command() const {
        return std::make_shared<Cmd>();
    }
private:
    const std::string _commandName;
    const std::string _commandInfo;
};

// command factory

class CommandFactory {
public:
    // command info
    using CommandList = std::vector<std::string>;
    CommandList get_command_list();
    const std::string &command_info(const std::string &cmdName);
    // command creation
    std::shared_ptr<Command> get_command(const std::string &cmdName);
    // command registration
    template<typename Cmd>
    void register_command(const std::string &cmdName, const std::string &cmdInfo) {
        static_assert(std::is_base_of<Command, Cmd>::value, "Class Cmd has to inherit from the class Command");
        _registry[cmdName].reset(new CommandCreator<Cmd>(cmdName, cmdInfo));
    }
    void forget_command(const std::string &cmdName)
        { _registry.erase(cmdName); }
    // singleton access
    static CommandFactory &instance() {
        static CommandFactory _instance;
        return _instance;
    }
private:
    CommandFactory() = default;
    CommandFactory(const CommandFactory&) = delete;
    CommandFactory& operator= (const CommandFactory&) = delete;
    std::map<std::string, std::unique_ptr<BaseCommandCreator>> _registry;
};

#endif // COMMANDFACTORY_H
