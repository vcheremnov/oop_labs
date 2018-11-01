#include <sstream>
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
