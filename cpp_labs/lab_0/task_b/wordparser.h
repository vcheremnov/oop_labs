#ifndef WORDPARSER_H
#define WORDPARSER_H

#include <string>
#include <iostream>

namespace Words {

class Parser {
public:
    // public methods
    Parser(std::istream &is);
    bool get_word(std::string &word);               // retrieve the next word from the stream
    bool has_reached_end() {                        // checks if the parser has reached the EOF
        return _is.eof();
    }
private:
    std::istream &_is;                              // input stream associated with the parser
    std::string _curLine;                           // current line
    std::string::const_iterator _linePos;           // current position in the current line
    // private methods
    void get_line();                                // get the next line from the stream
    static bool safe_isalnum(unsigned char ch);     // safe version of the std::isalnum
};

} // namespace Words

#endif // WORDPARSER_H
