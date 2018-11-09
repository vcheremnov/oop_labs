#include <exception>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "command.h"
#include "commandfactory.h"

namespace {

// # command - a comment line

class CommentCommand: public Command {
public:
    void execute(const ArgList&, Context&) {}
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
    double varValue = 0.0;
    if (context.is_valid_varname(args.back())) {
        varValue = context.get_variable_value(args.back());
    }
    else {
        varValue = context.get_value_from(args.back());
    }
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
    double val = 0.0;
    if (context.is_valid_varname(args.front())) {
        val = context.get_variable_value(args.front());
    }
    else {
        val = context.get_value_from(args.front());
    }
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
    void execute(const ArgList &args, Context &context);
};

void CmdInfoCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(1, args.size());
    std::string cmdInfo = CommandFactory::instance().command_info(args.front());
    context.print_val(cmdInfo);
}

// command list command

class CmdListCommand: public Command {
public:
    void execute(const ArgList &args, Context &context);
};

void CmdListCommand::execute(const ArgList &args, Context &context) {
    _arg_number_check(0, args.size());
    auto cmdList = CommandFactory::instance().get_command_list();
    for (const auto &cmdName: cmdList) {
        context.print_val(cmdName);
    }
}

// command registration

bool register_commands() {
    auto &cmdFactory = CommandFactory::instance();

    cmdFactory.register_command<CommentCommand> ("#", "# <comment>");
    cmdFactory.register_command<DefineCommand>  ("DEFINE", "DEFINE <variable name> <variable value>");
    cmdFactory.register_command<PrintVarCommand>("PRINTVAR", "PRINTVAR <variable name>");
    cmdFactory.register_command<PrintCommand>   ("PRINT", "PRINT <no args>");
    cmdFactory.register_command<PushCommand>    ("PUSH", "PUSH <value>");
    cmdFactory.register_command<PopCommand>     ("POP", "POP <no args>");
    cmdFactory.register_command<SqrtCommand>    ("SQRT", "SQRT <no args>");
    cmdFactory.register_command<PlusCommand>    ("+", "+ <no args>");
    cmdFactory.register_command<MinusCommand>   ("-", "- <no args>");
    cmdFactory.register_command<MultCommand>    ("*", "* <no args>");
    cmdFactory.register_command<DivCommand>     ("/", "/ <no args>");
    cmdFactory.register_command<CmdInfoCommand> ("CMDINFO", "CMDINFO <command name>");
    cmdFactory.register_command<CmdListCommand> ("CMDLIST", "CMDLIST <no args>");

    return true;
}

bool commandReg = register_commands();

} // anonymous namespace
