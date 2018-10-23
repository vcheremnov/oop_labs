#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include "calculator.h"
#include "command_errors.h"

// abstract command

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const Calculator::ArgList &args, Calculator::Context &context) = 0;
protected:
    using ArgNum = std::size_t;
    void _arg_number_check(ArgNum requiredNum, ArgNum actualNum);
    static double _convert_to_value(const std::string &arg, Calculator::Context &context);
};

#endif // COMMAND_H
