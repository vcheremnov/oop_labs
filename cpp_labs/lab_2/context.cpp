#include <algorithm>
#include <sstream>
#include <iomanip>
#include "context.h"
#include "command_errors.h"

namespace {

bool _isdigit(char ch) {
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool _isalnum(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch));
}

} // anonymous namespace

// variables operations

bool Context::is_variable(const std::string &varName) const {
    return _variables.find(varName) != _variables.end();
}

bool Context::is_valid_varname(const std::string &varName) {
    if (varName.empty()) {
        return false;
    }
    if (_isdigit(varName.front())) {
        return false;
    }
    return std::find_if_not(varName.cbegin(), varName.cend(), _isalnum) == varName.cend();
}

void Context::set_variable_value(const std::string &varName, double varValue) {
    if (!is_valid_varname(varName)) {
        std::stringstream msgStream;
        msgStream << "The variable name " << std::quoted(varName) << " is invalid";
        throw CommandError::InvalidVariableName(msgStream.str());
    }
    _variables[varName] = varValue;
}

double Context::get_variable_value(const std::string &varName) const {
    double val = 0.0;
    try {
        val = _variables.at(varName);
    }
    catch (const std::out_of_range&) {
        std::stringstream msgStream;
        msgStream << "The identifier " << std::quoted(varName) << " is unknown";
        throw CommandError::MissingVariable(msgStream.str());
    }
    return val;
}

double Context::get_value_from(const std::string &str) {
    double val = 0.0;
    std::stringstream msgStream;
    try {
        std::size_t endIx = 0;
        val = std::stod(str, &endIx);
        if (endIx != str.length()) {
            throw std::invalid_argument("");
        }
    }
    catch (const std::invalid_argument&) {
        msgStream << std::quoted(str) << " is not representable as value";
        throw CommandError::InvalidArgument(msgStream.str());
    }
    catch (const std::out_of_range&) {
        msgStream << std::quoted(str) << " is out of range of representable values";
        throw CommandError::ArgumentError(msgStream.str());
    }
    return val;
}
