#include "calculator.h"
#include "commandcreator.h"
#include <exception>
#include <memory>
#include <vector>

Calculator::Calculator(std::istream &inputStream, std::ostream &outputStream):
    _inputStream(inputStream), _outputStream(outputStream) {}

bool Calculator::_parse_line(std::string &cmdName, ArgList &args) {
    return true;
}

void Calculator::calculate() {
    Context context(*this);
    ArgList args;
    std::shared_ptr<Command> cmd;
    std::string cmdName;

    while (_parse_line(cmdName, args)) {
        try {
            cmd = CommandFactory::instance().get_command(cmdName);
            cmd->execute(args, context);
        }
        catch (const std::runtime_error &ex) {
            std::cerr << ex.what() << std::endl;
        }
    }
}
