#include <cctype>
#include <algorithm>
#include <exception>
#include "command.h"

namespace {

bool _isdigit(char ch) {
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool _isalnum(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch));
}

} // anonymous namespace

bool Command::_is_valid_varname(const std::string &varName) {
    if (varName.empty()) {
        return false;
    }
    if (_isdigit(varName.front())) {
        return false;
    }
    return std::find_if_not(varName.cbegin(), varName.cend(), _isalnum) == varName.cend();
}

double Command::_convert_to_value(const std::string &arg, Calculator::Context &context) {
    if (context.is_variable(arg)) {
        return context.get_variable(arg);
    }
    // try to convert to double value
    double val = 0.0;
    try {
        std::size_t endIx = 0;
        val = std::stod(arg, &endIx);
        if (endIx != arg.length()) {
            throw std::invalid_argument("");
        }
    }
    catch (const std::invalid_argument) {
        throw CommandError::InvalidArgument("\"" + arg + "\" is not representable as value");
    }
    catch (const std::out_of_range) {
        throw CommandError::ArgumentError("\"" + arg + "\" is out of range of representable values");
    }
    return val;
}

// exceptions

namespace CommandError {

// common command error

Error::Error(const std::string &what_arg):
    std::runtime_error(what_arg) {}

// unknown command

UnknownCommand::UnknownCommand(const std::string &what_arg):
    Error(what_arg) {}

// arithmetic error

ArithmeticError::ArithmeticError(const std::string &what_arg):
    Error(what_arg) {}

DivisionByZero::DivisionByZero(const std::string &what_arg):
    ArithmeticError(what_arg) {}

FunctionDomainError::FunctionDomainError(const std::string &what_arg):
    ArithmeticError(what_arg) {}

// argument error

ArgumentError::ArgumentError(const std::string &what_arg):
    Error(what_arg) {}

InvalidArgument::InvalidArgument(const std::string &what_arg):
    ArgumentError(what_arg) {}

ArgumentMismatch::ArgumentMismatch(const std::string &what_arg):
    ArgumentError(what_arg) {}

// calculator's context error

ContextError::ContextError(const std::string &what_arg):
    Error(what_arg) {}

StackError::StackError(const std::string &what_arg):
    ContextError(what_arg) {}

VariableError::VariableError(const std::string &what_arg):
    ContextError(what_arg) {}

InvalidVariableName::InvalidVariableName(const std::string &what_arg):
    VariableError(what_arg) {}

MissingVariable::MissingVariable(const std::string &what_arg):
    VariableError(what_arg) {}

} // namespace CommandError


