#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <vector>
#include <iostream>
#include <map>

class Calculator {
public:
    // type names
    using ArgList = std::vector<std::string>;
    using VarList = std::vector<std::string>;
    // public methods
    Calculator(std::istream &inputStream, std::ostream &outputStream);
    void calculate();
    // Execution Context
    class Context {
        friend class Calculator;
    public:
        // stack operations
        void stack_pop();
        double &stack_top();
        void stack_push(double val);
        bool stack_empty();
        std::stack<double>::size_type stack_size();
        // variables operations
        VarList get_variable_list();
        bool is_variable(const std::string &varName);
        void set_variable_value(const std::string &varName, double varValue);
        double get_variable_value(const std::string &varName);
        static bool is_valid_varname(const std::string &varName);
        // stream operations
        template<typename T>
        void print_val(T val) {
            _calcRef._outputStream << val << std::endl;
        }
    private:
        Context(Calculator &calc): _calcRef(calc) {}
        Calculator &_calcRef;
    };
private:
    std::istream &_inputStream;
    std::ostream &_outputStream;
    std::stack<double> _stack;
    std::map<std::string, double> _variables;
    // line parsing
    bool _parse_line(std::string &commandName, ArgList &argList);
};


#endif // CALCULATOR_H
