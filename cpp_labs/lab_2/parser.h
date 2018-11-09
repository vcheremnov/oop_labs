#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>

class Parser {
public:
    using TokenList = std::vector<std::string>;
    Parser(std::istream &inputStream):
        _inputStream(inputStream) {}
    bool has_reached_end()
        { return _inputStream.eof(); }
    bool parse_line(TokenList &tokenList);
private:
    std::istream &_inputStream;
};

#endif // PARSER_H
