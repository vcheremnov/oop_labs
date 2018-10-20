#include <memory>
#include <cctype>
#include <vector>
#include <exception>
#include <algorithm>
#include "calculator.h"
#include "commandfactory.h"

namespace {

bool _isspace(char ch) {
    return std::isspace(static_cast<unsigned char>(ch));
}

bool _isdigit(char ch) {
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool _isalnum(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch));
}

} // anonymous namespace

// public methods

Calculator::Calculator(std::istream &inputStream, std::ostream &outputStream):
    _inputStream(inputStream), _outputStream(outputStream) {}

bool Calculator::is_valid_varname(const std::string &varName) {
    if (varName.empty()) {
        return false;
    }
    if (_isdigit(varName.front())) {
        return false;
    }
    return std::find_if_not(varName.cbegin(), varName.cend(), _isalnum) == varName.cend();
}

void Calculator::calculate() {
    Context context(*this);
    TokenList tokenList;
    std::shared_ptr<Command> cmd;

    while (!_inputStream.eof()) {
        try {
            // the first element of a token list is considered a command,
            // and other elements are arguments of the command
            tokenList = _parse_line();
            if (!tokenList.empty()) {
                cmd = CommandFactory::instance().get_command(tokenList.front());
                // remake! avoid redundant copying
                cmd->execute(ArgList(tokenList.cbegin() + 1, tokenList.cend()), context);
            }
        }
        catch (const CommandError::UnknownCommand &ex) {
            std::cerr << ex.what() << std::endl;
        }
        catch (const CommandError::ArithmeticError &ex) {
            std::cerr << "Arithmetic error: " << ex.what() << std::endl;
        }
        catch (const CommandError::ArgumentError &ex) {
            std::cerr << "Argument error: " << ex.what() << std::endl;
        }
        catch (const CommandError::ContextError &ex) {
            std::cerr << "Calculator's context error: " << ex.what() << std::endl;
        }
        catch (const CommandError::Error &ex) {
            std::cerr << "Command error: " << ex.what() << std::endl;
        }
    }
}

// private methods

Calculator::TokenList Calculator::_parse_line() {
    std::string line;
    TokenList tokenList;
    // parse line
    if (std::getline(_inputStream, line)) {
        auto tokenBeg = line.cbegin(), tokenEnd = tokenBeg;
        while (tokenEnd != line.cend()) {
            // find the beginning of the next token
            tokenBeg = std::find_if_not(tokenEnd, line.cend(), _isspace);
            // find the end of the token
            tokenEnd = std::find_if(tokenBeg, line.cend(), _isspace);
            // add token to the list
            if (tokenBeg != tokenEnd) {
                tokenList.emplace_back(tokenBeg, tokenEnd);
            }
        }
    }
    return tokenList;
}
