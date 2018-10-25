#include <cctype>
#include <algorithm>
#include <exception>
#include <sstream>
#include <iomanip>
#include "command.h"

void Command::_arg_number_check(ArgNum requiredNum, ArgNum actualNum) {
    // throws an exception in case of argument number mismatch
    if (requiredNum != actualNum) {
        std::stringstream msgStream;
        msgStream << "Expected " << requiredNum << (requiredNum == 1 ? " argument" : " arguments")
                  << ", but received " << actualNum;
        throw CommandError::ArgumentMismatch(msgStream.str());
    }
}

double Command::_convert_to_value(const std::string &arg, Context &context) {
    // try to interpret as a variable
    if (context.is_valid_varname(arg)) {
        return context.get_variable_value(arg);
    }
    // try to convert to double value
    double val = 0.0;
    std::stringstream msgStream;
    try {
        std::size_t endIx = 0;
        val = std::stod(arg, &endIx);
        if (endIx != arg.length()) {
            throw std::invalid_argument("");
        }
    }
    catch (const std::invalid_argument&) {
        msgStream << std::quoted(arg) << " is not representable as value";
        throw CommandError::InvalidArgument(msgStream.str());
    }
    catch (const std::out_of_range&) {
        msgStream << std::quoted(arg) << " is out of range of representable values";
        throw CommandError::ArgumentError(msgStream.str());
    }
    return val;
}
