#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <stack>
#include <vector>
#include <iostream>

// Execution Context

class Context {
public:
    // type names
    using VarList = std::vector<std::string>;
    using size_type = std::stack<double>::size_type;
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
    size_type stack_size()
        { return _stack.size(); }
    // variables operations
    bool is_variable(const std::string &varName) const;
    void set_variable_value(const std::string &varName, double varValue);
    double get_variable_value(const std::string &varName) const;
    static bool is_valid_varname(const std::string &varName);
    // retrieve value from a string
    static double get_value_from(const std::string &str);
private:
    std::ostream &_outputStream;
    std::stack<double> _stack;
    std::map<std::string, double> _variables;
};

#endif // CONTEXT_H
