#ifndef WORDLIST_H
#define WORDLIST_H

#include <iostream>
#include "wordparser.h"

namespace Words {
    class List {
    public:
        void fill(std::istream &is);
        void print_as_csv(std::ostream &os);
        void print_as_dict(std::ostream &os);
    private:
        static Parser parser;
        std::list<Record> mWordlist;
        unsigned mTotalWords = 0;
        // private methods
        void write_records_to(std::ostream &os);
    };
}

#endif // WORDLIST_H
