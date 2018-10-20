#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include "calculator.h"
#include "command_errors.h"

// abstract command

class Command {
public:
    Command() = default;
    virtual ~Command() = default;
    virtual void execute(const Calculator::ArgList &args, Calculator::Context &context) = 0;
protected:
    static double _convert_to_value(const std::string &arg, Calculator::Context &context);
};

#endif // COMMAND_H
