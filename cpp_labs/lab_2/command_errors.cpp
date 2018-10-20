#include "command_errors.h"

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

