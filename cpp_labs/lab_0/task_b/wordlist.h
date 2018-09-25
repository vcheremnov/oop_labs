#ifndef CSVMAKER_H
#define CSVMAKER_H

#include <iostream>
#include "wordparser.h"

class WordList {
public:
    void fill(std::istream &is);
    void print_as_csv(std::ostream &os);
    void print_as_dict(std::ostream &os);
private:
    static WordParser parser;
    std::list<WordItem> mWordlist;
    unsigned mTotalWords = 0;
    // private methods
    void write_records_to(std::ostream &os);
};

#endif // CSVMAKER_H
