#include <memory>
#include <sstream>
#include <iomanip>
#include <exception>
#include "calculator.h"
#include "commandfactory.h"

// public methods

void Calculator::calculate() {
    ArgList argList;
    std::string cmdName;
    std::shared_ptr<Command> cmd;

    for (std::size_t lineNo = 1; !_inputStream.eof(); ++lineNo) {
        if (_parse_line(cmdName, argList)) try {
            cmd = CommandFactory::instance().get_command(cmdName);
            cmd->execute(argList, _context);
        }
        catch (const CommandError::Error &ex) {
            std::cerr << "Error at line " << lineNo << ", command "
                      << std::quoted(cmdName) << ": " << ex.what() << std::endl;
        }
    }
}

// private methods

bool Calculator::_parse_line(std::string &commandName, ArgList &argList) {
    // returns true if something has been read, false otherwise
    std::string word, line;
    // clear previous contents
    commandName.clear();
    argList.clear();
    // parse line
    if (std::getline(_inputStream, line)) {
        std::stringstream wordStream(line);
        wordStream >> commandName;
        while (wordStream >> word) {
            argList.push_back(word);
        }
    }
    return !commandName.empty();
}
