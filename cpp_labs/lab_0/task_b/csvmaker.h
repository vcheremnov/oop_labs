#ifndef CSVMAKER_H
#define CSVMAKER_H

#include "wordparser.h"

// a simple version of the csv maker
// works with WordItem structures (from wordparser.h)

class CSVmaker {
    WordParser parser;
    std::list<WordItem> get_word_list(const std::string &inFile);
public:
    CSVmaker() {}
    void make_csv(const std::string &inFile, const std::string &outFile);
};

#endif // CSVMAKER_H
