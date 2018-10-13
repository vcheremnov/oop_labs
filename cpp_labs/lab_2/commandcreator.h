#ifndef COMMANDCREATOR_H
#define COMMANDCREATOR_H

#include <memory>
#include <unordered_map>
#include "command.h"

// abstract command creator

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual std::shared_ptr<Command> create_command() = 0;
};

// command factory

class CommandFactory {
public:
    std::shared_ptr<Command> get_command(const std::string &cmdName);
    void register_command(const std::string &cmdName, CommandCreator *creator);
    static CommandFactory &instance() {
        static CommandFactory _instance;
        return _instance;
    }
protected:
    CommandFactory() = default;
private:
    std::unordered_map<std::string, CommandCreator*> _registry;
};

// concrete command creators

class CommentCreator: public CommandCreator {
public:
    CommentCreator();
    std::shared_ptr<Command> create_command() {
        return std::make_shared<CommentCommand>();
    }
};

#endif // COMMANDCREATOR_H
