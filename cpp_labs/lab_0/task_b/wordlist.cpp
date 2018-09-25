#include <algorithm>
#include <iostream>
#include <fstream>
#include "wordlist.h"

WordParser WordList::parser;

void WordList::fill(std::istream &is) {
    // reset total words counter
    mTotalWords = 0;
    // get a new wordlist
    mWordlist = parser.get_wordlist_from(is);
    // count the total number of words
    std::for_each(mWordlist.begin(), mWordlist.end(),
                  [this](const WordItem &item) { mTotalWords += item.second; });
}

void WordList::print_as_csv(std::ostream &os) {
    // print in csv format: <word>,<number of occurrences>,<fraction of occurrences>
    // set precision to 3 decimal places
    os.precision(3);
    for (const auto &record: mWordlist) {
        os << record.first << "," << record.second << ","
            << 100.0 * record.second / mTotalWords << std::endl;
    }
}

void WordList::print_as_dict(std::ostream &os) {
    // print in dictionary format: <word> : <number of occurrences>
    for (const auto &record: mWordlist) {
        os << record.first << ": " << record.second << std::endl;
    }
}
