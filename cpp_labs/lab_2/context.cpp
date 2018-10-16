#include "calculator.h"

namespace {

using Context = Calculator::Context;

} // anonymous namespace

// stack operations

void Context::pop() {
    _calcRef._stack.pop();
}

double &Context::top() {
    return _calcRef._stack.top();
}

void Context::push(double val) {
    _calcRef._stack.push(val);
}

std::stack<double>::size_type Context::size() {
    return _calcRef._stack.size();
}

bool Context::empty() {
    return _calcRef._stack.empty();
}

// variables operations

bool Context::is_variable(const std::string &varName) {
    return _calcRef._variables.find(varName) != _calcRef._variables.end();
}

void Context::set_variable(const std::string &varName, double varValue) {
    // variable name is assumed to be valid (checked in the corresponding command)
    _calcRef._variables[varName] = varValue;
}

double Context::get_variable(const std::string &varName) {
    // throws std::out_of_range if fails
    return _calcRef._variables.at(varName);
}

// stream operations

void Context::print_top() {
    _calcRef._outputStream << top();
}
