#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <list>
#include "context.h"
#include "command_errors.h"

// abstract command

class Command {
public:
    using ArgList = std::list<std::string>;
    virtual ~Command() = default;
    virtual void execute(const ArgList &args, Context &context) = 0;
protected:
    using ArgNum = std::size_t;
    static void _arg_number_check(ArgNum requiredNum, ArgNum actualNum);
};

#endif // COMMAND_H
