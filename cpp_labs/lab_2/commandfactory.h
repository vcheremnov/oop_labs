#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include "command.h"

class CommandCreator;

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
    void register_command(const std::string &cmdName, CommandCreator &creator);
    void forget_command(const std::string &cmdName);
    // singleton access
    static CommandFactory &instance() {
        static CommandFactory _instance;
        return _instance;
    }
private:
    CommandFactory() = default;
    std::map<std::string, CommandCreator*> _registry;
};

// abstract command creator

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual const std::string &command_name() const = 0;
    virtual const std::string &command_info() const = 0;
    virtual std::shared_ptr<Command> create_command() const = 0;
};

// template concrete command creator

template<class Cmd>
class CreatorOf: public CommandCreator {
    static_assert(std::is_base_of<Command, Cmd>::value, "Class Cmd has to inherit from the class Command");
public:
    CreatorOf(const std::string &cmdName, const std::string &cmdInfo):
        _commandName(cmdName), _commandInfo(cmdInfo) {
        CommandFactory::instance().register_command(command_name(), *this);
    }
    ~CreatorOf() {
        CommandFactory::instance().forget_command(command_name());
    }
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

#endif // COMMANDFACTORY_H
