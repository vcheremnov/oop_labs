#include "parser.h"
#include <sstream>

bool Parser::parse_line(TokenList &tokenList) {
    // returns true if something has been read, false otherwise
    std::string word, line;
    // clear previous contents
    tokenList.clear();
    // parse line
    if (std::getline(_inputStream, line)) {
        std::stringstream wordStream(line);
        while (wordStream >> word) {
            tokenList.push_back(word);
        }
    }
    return !tokenList.empty();
}
