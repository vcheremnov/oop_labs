#ifndef WORDLIST_H
#define WORDLIST_H

#include <list>
#include <string>
#include <iostream>
#include "wordparser.h"

namespace Words {

using Record = std::pair<std::string, unsigned>;
using Compare = bool (*)(const Record&, const Record&);

bool compare_default(const Record&, const Record&);

class List {
public:
    void fill(std::istream &is);                    // fill the list from the input stream
    void print_as_csv(std::ostream &os);            // print the list in csv format
    void print_as_dict(std::ostream &os);           // print the list in dictionary format
    void sort(Compare cmp = compare_default) {      // sort the list using given compare function
        _wordlist.sort(cmp);
    }
private:
    std::list<Record> _wordlist;
    unsigned _totalWords = 0;
};

} // namespace Words

#endif // WORDLIST_H
