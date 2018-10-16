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
        void pop();
        double &top();
        void push(double val);
        std::stack<double>::size_type size();
        bool empty();
        // variables operations
        bool is_variable(const std::string &varName);
        void set_variable(const std::string &varName, double varValue);
        double get_variable(const std::string &varName);
        // stream operations
        void print_top();
    private:
        Context(Calculator &calc): _calcRef(calc) {}
        bool _is_valid_varname(const std::string &varName);
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
