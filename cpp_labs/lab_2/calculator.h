#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <vector>
#include <iostream>
#include <map>

// Execution Context

class Context {
public:
    // type names
    using VarList = std::vector<std::string>;
    // public methods
    Context(std::ostream &outputStream):
        _outputStream(outputStream) {}
    // stream operations
    template<typename T>
    void print_val(T val)
        { _outputStream << val << std::endl; }
    // stack operations
    void stack_pop()
        { _stack.pop(); }
    double &stack_top()
        { return _stack.top(); }
    void stack_push(double val)
        { _stack.push(val); }
    bool stack_empty()
        { return _stack.empty(); }
    std::stack<double>::size_type stack_size()
        { return _stack.size(); }
    // variables operations
    bool is_variable(const std::string &varName);
    void set_variable_value(const std::string &varName, double varValue);
    double get_variable_value(const std::string &varName);
    static bool is_valid_varname(const std::string &varName);
private:
    std::ostream &_outputStream;
    std::stack<double> _stack;
    std::map<std::string, double> _variables;
};

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
