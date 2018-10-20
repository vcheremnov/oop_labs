#include <cctype>
#include <algorithm>
#include <exception>
#include <sstream>
#include <iomanip>
#include "command.h"

double Command::_convert_to_value(const std::string &arg, Calculator::Context &context) {
    if (context.is_variable(arg)) {
        return context.get_variable(arg);
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
