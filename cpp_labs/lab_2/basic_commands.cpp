#include <exception>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "command.h"
#include "commandfactory.h"

namespace {

using ArgList = Calculator::ArgList;
using Context = Calculator::Context;

} // anonymous namespace

// # command - a comment line

class CommentCommand: public Command {
public:
    void execute(const ArgList &args, Context &context) {}
};

// PRINT command  - print the top of the stack

class PrintCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void PrintCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_empty()) {
        throw CommandError::StackError("The stack is empty");
    }
    double topValue = context.stack_top();
    context.print_val(topValue);
}

// DEFINE command - define a new variable

class DefineCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void DefineCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(2, args.size());
    double varValue = Command::_convert_to_value(args.back(), context);
    context.set_variable_value(args.front(), varValue);
}

// PRINTVAR command - print variable's value

class PrintVarCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void PrintVarCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(1, args.size());
    double val = context.get_variable_value(args.front());
    context.print_val(val);
}

// PUSH command - push a value onto the stack

class PushCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void PushCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(1, args.size());
    double val = Command::_convert_to_value(args.front(), context);
    context.stack_push(val);
}

// POP command - pop a value from the stack

class PopCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void PopCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_empty()) {
        throw CommandError::StackError("The stack is empty");
    }
    context.stack_pop();
}

// SQRT command - calculate square root of the top stack value

class SqrtCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void SqrtCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_empty()) {
        throw CommandError::StackError("The stack is empty");
    }
    double topVal = context.stack_top();
    if (topVal < 0.0) {
        std::stringstream msgStream;
        msgStream << "A negative number " << topVal << " has been passed to SQRT";
        throw CommandError::FunctionDomainError(msgStream.str());
    }
    context.stack_top() = std::sqrt(topVal);
    context.stack_push(std::sqrt(topVal));
}

// + command

class PlusCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void PlusCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_size() < 2) {
        throw CommandError::StackError("Too little elements in the stack");
    }
    double rightArg = context.stack_top();
    context.stack_pop();
    context.stack_top() += rightArg;
}

// - command

class MinusCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void MinusCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_size() < 2) {
        throw CommandError::StackError("Too little elements in the stack");
    }
    double rightArg = context.stack_top();
    context.stack_pop();
    context.stack_top() -= rightArg;
}

// * command

class MultCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void MultCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_size() < 2) {
        throw CommandError::StackError("Too little elements in the stack");
    }
    double rightArg = context.stack_top();
    context.stack_pop();
    context.stack_top() *= rightArg;
}

// / command

class DivCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void DivCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    if (context.stack_size() < 2) {
        throw CommandError::StackError("Too little elements in the stack");
    }
    double rightArg = context.stack_top();
    if (rightArg == 0.0) {
        throw CommandError::DivisionByZero("Division by zero");
    }
    context.stack_pop();
    context.stack_top() /= rightArg;
}

// command info command

class CmdInfoCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context);
};

void CmdInfoCommand::execute(const Calculator::ArgList &args, Calculator::Context &context) {
    _arg_number_check(1, args.size());
    std::string cmdInfo = CommandFactory::instance().command_info(args.front());
    context.print_val(cmdInfo);
}

// command list command

class CmdListCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context);
};

void CmdListCommand::execute(const Calculator::ArgList &args, Calculator::Context &context) {
    _arg_number_check(0, args.size());
    auto cmdList = CommandFactory::instance().get_command_list();
    for (const auto &cmdName: cmdList) {
        context.print_val(cmdName);
    }
}

// variable list command

class VarListCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context);
};

void VarListCommand::execute(const Calculator::ArgList &args, Calculator::Context &context) {
    _arg_number_check(0, args.size());
    auto varList = context.get_variable_list();
    for (const auto &varName: varList) {
        context.print_val(varName);
    }
}

namespace { // creators registration

CreatorOf<CommentCommand>   commentCreator  ("#",           "# <comment>");
CreatorOf<DefineCommand>    defineCreator   ("DEFINE",      "DEFINE <variable name> <variable value>");
CreatorOf<PrintVarCommand>  printVarCreator ("PRINTVAR",    "PRINTVAR <variable name>");
CreatorOf<PrintCommand>     printCreator    ("PRINT",       "PRINT <no args>");
CreatorOf<PushCommand>      pushCreator     ("PUSH",        "PUSH <value>");
CreatorOf<PopCommand>       popCreator      ("POP",         "POP <no args>");
CreatorOf<SqrtCommand>      sqrtCreator     ("SQRT",        "SQRT <no args>");
CreatorOf<PlusCommand>      plusCreator     ("+",           "+ <no args>");
CreatorOf<MinusCommand>     minusCreator    ("-",           "- <no args>");
CreatorOf<MultCommand>      multCreator     ("*",           "* <no args>");
CreatorOf<DivCommand>       divCreator      ("/",           "/ <no args>");
CreatorOf<CmdInfoCommand>   helpCreator     ("CMDINFO",     "CMDINFO <command name>");
CreatorOf<CmdListCommand>   cmdListCreator  ("CMDLIST",     "CMDLIST <no args>");
CreatorOf<VarListCommand>   varListCreator  ("VARLIST",     "VARLIST <no args>");

} // anonymous namespace
