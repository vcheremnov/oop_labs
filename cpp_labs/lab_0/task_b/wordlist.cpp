#include <algorithm>
#include <iostream>
#include <fstream>
#include "wordlist.h"

namespace Words {
    Parser List::parser;

    void List::fill(std::istream &is) {
        // reset total words counter
        mTotalWords = 0;
        // get a new wordlist
        mWordlist = parser.get_wordlist_from(is);
        // count the total number of words
        std::for_each(mWordlist.begin(), mWordlist.end(),
                      [this](const Record &item) { mTotalWords += item.second; });
    }

    void List::print_as_csv(std::ostream &os) {
        // print in csv format: <word>,<number of occurrences>,<fraction of occurrences>
        // set precision to 3 decimal places
        auto oldPrecision = os.precision(3);
        for (const auto &record: mWordlist) {
            os << record.first << "," << record.second << ","
                << 100.0 * record.second / mTotalWords << std::endl;
        }
        os.precision(oldPrecision);
    }

    void List::print_as_dict(std::ostream &os) {
        // print in dictionary format: <word> : <number of occurrences>
        for (const auto &record: mWordlist) {
            os << record.first << ": " << record.second << std::endl;
        }
    }
}

