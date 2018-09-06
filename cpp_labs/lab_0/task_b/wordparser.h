#ifndef WORDPARSER_H
#define WORDPARSER_H

#include <iostream>
#include <string>
#include <list>
#include <map>

struct WordItem {
    std::string word;
    unsigned frequency = 0;         // frequency of word's occurence in the file
    double fraction = 0.0;          // frequency in %
    WordItem(const std::string wrd, unsigned freq, double frac):
        word(wrd), frequency(freq), fraction(frac) {}

    using Compare = bool (*)(const WordItem&, const WordItem&);
    static bool compare_default(const WordItem&, const WordItem&);
};

class WordParser {
    unsigned wordCount = 0;                                 // word counter
    WordItem::Compare compare = WordItem::compare_default;  // comparator for sorting
    std::map<const std::string, unsigned> words;            // used to store word - frequency pairs
    // private methods
    void add_word(const std::string &word);
    void parse_line(const std::string &line);
    void reset();                                           // reset internal data
public:
    WordParser() {}
    std::list<WordItem> parse_file(std::istream &is);
    void set_comparator(WordItem::Compare cmp) { compare = cmp; }
};

#endif // WORDPARSER_H
