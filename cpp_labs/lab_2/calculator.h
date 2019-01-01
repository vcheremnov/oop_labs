#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <iostream>
#include <iterator>
#include "context.h"
#include "command.h"
#include "parser.h"

// Calculator

class Calculator {
public:
    void calculate(std::istream &inputStream, std::ostream &outputStream);
private:
    std::string _get_cmdname_from(Parser::TokenList &tokenList)
        { return std::string(std::move(tokenList.front())); }
    Command::ArgList _get_arglist_from(Parser::TokenList &tokenList)
        { return Command::ArgList(std::make_move_iterator(std::next(tokenList.begin())),
                                  std::make_move_iterator(tokenList.end())); }
    void _process_stream(Parser &tokenParser, Context &context);
};


#endif // CALCULATOR_H
