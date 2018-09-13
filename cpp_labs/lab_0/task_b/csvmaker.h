#ifndef CSVMAKER_H
#define CSVMAKER_H

#include <iostream>
#include "wordparser.h"

// a simple version of the csv maker
// works with WordItem structures (from wordparser.h)

class CSVmaker {
    WordParser parser;
    std::list<WordItem> wordlist;
    void get_wordlist_from(std::istream &is);
    void write_records_to(std::ostream &os);
    void reset() { wordlist.clear(); }
public:
    void make_csv(const std::string &inFile, const std::string &outFile);
};

#endif // CSVMAKER_H
