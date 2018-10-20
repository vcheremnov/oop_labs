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
    // public methods
    Calculator(std::istream &inputStream, std::ostream &outputStream);
    static bool is_valid_varname(const std::string &varName);
    void calculate();
    // Execution Context
    class Context {
        friend class Calculator;
    public:
        // stack operations
        void stack_pop();
        double &stack_top();
        void stack_push(double val);
        std::stack<double>::size_type stack_size();
        bool stack_empty();
        // variables operations
        bool is_variable(const std::string &varName);
        void set_variable(const std::string &varName, double varValue);
        double get_variable(const std::string &varName);
        // stream operations
        void print_val(double val);
    private:
        Context(Calculator &calc): _calcRef(calc) {}
        Calculator &_calcRef;
    };
private:
    using TokenList = std::vector<std::string>;
    std::istream &_inputStream;
    std::ostream &_outputStream;
    std::stack<double> _stack;
    std::unordered_map<std::string, double> _variables;
    // private methods
    TokenList _parse_line();
};


#endif // CALCULATOR_H
