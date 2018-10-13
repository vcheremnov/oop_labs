#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include "calculator.h"

class Command {
public:
    virtual void execute(const Calculator::ArgList &args, Calculator::Context &context) = 0;
    virtual std::string get_name() = 0;
    virtual ~Command() = default;
};

class CommentCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context);
    std::string get_name();
};

#endif // COMMAND_H
