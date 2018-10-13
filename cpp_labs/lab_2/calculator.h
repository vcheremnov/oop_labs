#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>

class Calculator {
public:
    // type names
    using ArgList = std::vector<std::string>;
    Calculator(std::istream &inputStream, std::ostream &outputStream);
    void calculate();
    // Execution Context internal class
    class Context {
        friend class Calculator;
    public:
        // stack operations
        double pop();
        double push();
        double size();
        double empty();
        // variables operations
        void set_variable(const std::string &varName, double varValue);
        double get_variable(const std::string &varName);
        // stream operations
        void print_value();
    private:
        Context(Calculator &calc): _calcRef(calc) {}
        Calculator &_calcRef;
    };
private:
    std::istream &_inputStream;
    std::ostream &_outputStream;
    std::stack<double> _stack;
    std::unordered_map<std::string, double> _variables;
    // private methods
    bool _parse_line(std::string &cmdName, ArgList &args);
};


#endif // CALCULATOR_H
