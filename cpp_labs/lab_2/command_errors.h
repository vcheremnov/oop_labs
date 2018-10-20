#ifndef COMMAND_ERRORS_H
#define COMMAND_ERRORS_H

#include <string>
#include <exception>
#include <stdexcept>

// exception hierarchy

namespace CommandError {

// common command error

class Error: public std::runtime_error {
public:
    Error(const std::string &what_arg);
};

// unknown command

class UnknownCommand: public Error {
public:
    UnknownCommand(const std::string &what_arg);
};

// arithmetic error

class ArithmeticError: public Error {
public:
    ArithmeticError(const std::string &what_arg);
};

class DivisionByZero: public ArithmeticError {
public:
    DivisionByZero(const std::string &what_arg);
};

class FunctionDomainError: public ArithmeticError {
public:
    FunctionDomainError(const std::string &what_arg);
};

// argument error

class ArgumentError: public Error {
public:
    ArgumentError(const std::string &what_arg);
};

class InvalidArgument: public ArgumentError {
public:
    InvalidArgument(const std::string &what_arg);
};

class ArgumentMismatch: public ArgumentError {
public:
    ArgumentMismatch(const std::string &what_arg);
};

// calculator's context error

class ContextError: public Error {
public:
    ContextError(const std::string &what_arg);
};

class StackError: public ContextError {
public:
    StackError(const std::string &what_arg);
};

class VariableError: public ContextError {
public:
    VariableError(const std::string &what_arg);
};

class InvalidVariableName: public VariableError {
public:
    InvalidVariableName(const std::string &what_arg);
};

class MissingVariable: public VariableError {
public:
    MissingVariable(const std::string &what_arg);
};

} // namespace CommandError

#endif // COMMAND_ERRORS_H
