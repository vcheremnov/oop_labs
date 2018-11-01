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
    virtual void execute(const Calculator::ArgList &args, Context &context) = 0;
protected:
    using ArgNum = std::size_t;
    static void _arg_number_check(ArgNum requiredNum, ArgNum actualNum);
};

#endif // COMMAND_H
