#include <exception>
#include <sstream>
#include "command.h"
#include "commandfactory.h"
#include "commandcreator.h"

// concrete command creators

// # command

class CommentCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context) {}
};

class CommentCreator: public CommandCreator {
public:
    CommentCreator() {
        CommandFactory::instance().register_command(command_name(), *this);
    }
    std::string command_name() {
        return "#";
    }
    void command_info(std::ostream &outputStream) {
        outputStream << "# <comment>" << std::endl;
    }
    std::shared_ptr<Command> create_command() {
        return std::make_shared<CommentCommand>();
    }
};

// PUSH command

class PushCommand: public Command {
public:
    void execute(const Calculator::ArgList &args, Calculator::Context &context);
};

void PushCommand::execute(const Calculator::ArgList &args, Calculator::Context &context) {
    if (args.size() != 1) {
        std::ostringstream errMsg;
        errMsg << "PUSH requires exactly 1 argument; " << args.size() << " has been passed";
        throw CommandError::ArgumentMismatch(errMsg.str());
    }
    double val = Command::_convert_to_value(args.front(), context);
    context.push(val);
}

class PushCreator: public CommandCreator {
public:
    PushCreator() {
        CommandFactory::instance().register_command(command_name(), *this);
    }
    std::string command_name() {
        return "PUSH";
    }
    void command_info(std::ostream &outputStream) {
        outputStream << "PUSH <value>" << std::endl;
    }
    std::shared_ptr<Command> create_command() {
        return std::make_shared<PushCommand>();
    }
};

namespace { // creators registration

CommentCreator commentCreator;
PushCreator pushCreator;

} // anonymous namespace
