#include <cctype>
#include <algorithm>
#include "wordparser.h"

namespace Words {

// public methods

Parser::Parser(std::istream &is):  _is(is) {
    _linePos = _curLine.cbegin();
}

bool Parser::get_word(std::string &word) {
    // discard the word's content
    word.clear();
    // alphanumeric sequence's boundaries
    auto wordStart = _linePos, wordEnd = wordStart;
    while (!has_reached_end()) {
        // find the first alphanumeric character in the line
        wordStart = std::find_if(_linePos, _curLine.cend(), safe_isalnum);
        // find the end of the alphanumeric sequence starting from the wordStart
        wordEnd = std::find_if_not(wordStart, _curLine.cend(), safe_isalnum);
        // move current position to the end of the sequence
        _linePos = wordEnd;
        // check if a sequence is not empty
        if (wordStart != wordEnd) {
            // copy sequence to the word
            word.assign(wordStart, wordEnd);
            // success: a word has been retrieved
            return true;
        }
        // end of the line has been reached; get the next one
        get_line();
    }
    // failure: end of file has been reached
    return false;
}

// private methods

void Parser::get_line() {
    std::getline(_is, _curLine);
    _linePos = _curLine.cbegin();
}

bool Parser::safe_isalnum(unsigned char ch) {
    return std::isalnum(ch);
}

} // namespace Words

