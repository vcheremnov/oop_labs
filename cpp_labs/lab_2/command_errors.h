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
    using std::runtime_error::runtime_error;
};

// unknown command

class UnknownCommand: public Error {
public:
    using Error::Error;
};

// arithmetic error

class ArithmeticError: public Error {
public:
    using Error::Error;
};

class DivisionByZero: public ArithmeticError {
public:
    using ArithmeticError::ArithmeticError;
};

class FunctionDomainError: public ArithmeticError {
public:
    using ArithmeticError::ArithmeticError;
};

// argument error

class ArgumentError: public Error {
public:
    using Error::Error;
};

class InvalidArgument: public ArgumentError {
public:
    using ArgumentError::ArgumentError;
};

class ArgumentMismatch: public ArgumentError {
public:
    using ArgumentError::ArgumentError;
};

// calculator's context error

class ContextError: public Error {
public:
    using Error::Error;
};

class StackError: public ContextError {
public:
    using ContextError::ContextError;
};

class VariableError: public ContextError {
public:
    using ContextError::ContextError;
};

class InvalidVariableName: public VariableError {
public:
    using VariableError::VariableError;
};

class MissingVariable: public VariableError {
public:
    using VariableError::VariableError;
};

} // namespace CommandError

#endif // COMMAND_ERRORS_H
