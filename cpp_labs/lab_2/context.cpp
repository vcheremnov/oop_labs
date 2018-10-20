#include <algorithm>
#include <sstream>
#include <iomanip>
#include "calculator.h"
#include "command_errors.h"

namespace {

using Context = Calculator::Context;

} // anonymous namespace

// stack operations

void Context::stack_pop() {
    _calcRef._stack.pop();
}

double &Context::stack_top() {
    return _calcRef._stack.top();
}

void Context::stack_push(double val) {
    _calcRef._stack.push(val);
}

std::stack<double>::size_type Context::stack_size() {
    return _calcRef._stack.size();
}

bool Context::stack_empty() {
    return _calcRef._stack.empty();
}

// variables operations

bool Context::is_variable(const std::string &varName) {
    return _calcRef._variables.find(varName) != _calcRef._variables.end();
}

void Context::set_variable(const std::string &varName, double varValue) {
    if (!is_valid_varname(varName)) {
        std::stringstream msgStream;
        msgStream << "The variable name " << std::quoted(varName) << " is invalid";
        throw CommandError::InvalidVariableName(msgStream.str());
    }
    _calcRef._variables[varName] = varValue;
}

double Context::get_variable(const std::string &varName) {
    double val = 0.0;
    try {
        val = _calcRef._variables.at(varName);
    }
    catch (const std::out_of_range&) {
        std::stringstream msgStream;
        msgStream << "The identifier " << std::quoted(varName) << " is unknown";
        throw CommandError::MissingVariable(msgStream.str());
    }
    return val;
}

// stream operations

void Context::print_val(double val) {
    _calcRef._outputStream << val << std::endl;
}
