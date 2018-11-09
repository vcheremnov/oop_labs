#include <memory>
#include <sstream>
#include <iomanip>
#include <exception>
#include <iterator>
#include "calculator.h"
#include "commandfactory.h"
#include "parser.h"

// public methods

void Calculator::calculate(std::istream &inputStream, std::ostream &outputStream) {
    Parser tokenParser(inputStream);
    Context context(outputStream);
    _process_stream(tokenParser, context);
}

// private methods

void Calculator::_process_stream(Parser &tokenParser, Context &context) {
    std::string cmdName;
    Command::ArgList argList;
    Parser::TokenList tokenList;
    std::shared_ptr<Command> cmd;

    for (std::size_t lineNo = 1; !tokenParser.has_reached_end(); ++lineNo) {
        if (tokenParser.parse_line(tokenList)) try {
            // move token list content to command name string & argument list
            cmdName = _get_cmdname_from(tokenList);
            argList = _get_arglist_from(tokenList);
            // get command by name & execute
            cmd = CommandFactory::instance().get_command(cmdName);
            cmd->execute(argList, context);
        }
        catch (const CommandError::Error &ex) {
            std::cerr << "Error at line " << lineNo << ", command "
                      << std::quoted(cmdName) << ": " << ex.what() << std::endl;
        }
    }
}
