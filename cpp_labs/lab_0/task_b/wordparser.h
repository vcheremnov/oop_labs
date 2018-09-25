#ifndef WORDPARSER_H
#define WORDPARSER_H

#include <list>
#include <iostream>
#include <unordered_map>

// WordItem types

using WordItem = std::pair<std::string, unsigned>;
using Compare = bool (*)(const WordItem&, const WordItem&);

class WordParser {
public:
    // comparator
    static bool compare_default(const WordItem&, const WordItem&);
    // public methods
    std::list<WordItem> get_wordlist_from(std::istream &is);
    void set_comparator(Compare cmp) {
        mComparator = cmp;
    }
private:
    Compare mComparator = compare_default;                       // comparator for sorting
    std::unordered_map<std::string, unsigned> wordCounter;      // counts word occurrences
    // private methods
    void add_word(const std::string &word);
    void parse_line(const std::string &line);
    void parse_file(std::istream &is);
    std::list<WordItem> make_wordlist();
};

#endif // WORDPARSER_H
