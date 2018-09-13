#ifndef WORDPARSER_H
#define WORDPARSER_H

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

struct WordItem {
    std::string word;
    unsigned frequency = 0;         // number of occurrences of the word
    double fraction = 0.0;          // frequency of occurrences in %
    WordItem(const std::string &wrd, unsigned freq, double frac):
        word(wrd), frequency(freq), fraction(frac) {}
};

class WordParser {
public:
    using Compare = bool (*)(const WordItem&, const WordItem&);
    static bool compare_default(const WordItem&, const WordItem&);
    std::list<WordItem> get_wordlist_from(std::istream &is);
    void set_comparator(Compare cmp) { compare = cmp; }
private:
    unsigned wordCount = 0;                                 // word counter
    Compare compare = compare_default;                      // comparator for sorting
    std::unordered_map<std::string, unsigned> wordCounter;  // counts word occurrences
    // private methods
    void add_word(const std::string &word);
    void parse_line(const std::string &line);
    void parse_file(std::istream &is);
    std::list<WordItem> make_wordlist();
    void reset();                                           // clear internal data
};

#endif // WORDPARSER_H
