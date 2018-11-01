#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <iostream>
#include "context.h"

// Calculator

class Calculator {
public:
    // type names
    using ArgList = std::vector<std::string>;
    // public methods
    Calculator(std::istream &inputStream, std::ostream &outputStream):
        _context(outputStream), _inputStream(inputStream) {}
    void calculate();
private:
    Context _context;
    std::istream &_inputStream;
    // line parsing
    bool _parse_line(std::string &commandName, ArgList &argList);
};


#endif // CALCULATOR_H
