#ifndef COMMANDCREATOR_H
#define COMMANDCREATOR_H

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include "command.h"

// abstract command creator

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual std::string command_name() = 0;
    virtual std::shared_ptr<Command> create_command() = 0;
    virtual void command_info(std::ostream &outputStream) = 0;
};

#endif // COMMANDCREATOR_H
