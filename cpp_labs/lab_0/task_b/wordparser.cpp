#include "wordparser.h"

namespace Words {
    bool Parser::compare_default(const Record &left, const Record &right) {
        // sorts by the word occurrence in descending order
        // if equal, sorts by words in alphabetical order
        return (left.second == right.second) ?
                    left.first < right.first : left.second > right.second;
    }

    void Parser::add_word(const std::string &word) {
        wordCounter[word] += 1;
    }

    void Parser::parse_line(const std::string &line) {
        auto wordStart = line.cbegin();
        for (auto iter = line.cbegin(); iter != line.cend(); ++iter) {
            if (isalnum(*iter)) {
                continue;
            }
            if (iter != wordStart) {
                add_word(std::string(wordStart, iter));
                wordStart = iter + 1;
            }
            else {
                ++wordStart;
            }
        }
        if (wordStart != line.cend()) {
            add_word(std::string(wordStart, line.cend()));
        }
    }

    void Parser::parse_file(std::istream &is) {
        std::string line;
        while (std::getline(is, line)) {
            parse_line(line);
        }
    }

    std::list<Record> Parser::make_wordlist() {
        return std::list<Record>(wordCounter.begin(), wordCounter.end());
    }

    std::list<Record> Parser::get_wordlist_from(std::istream &is) {
        // parse file & collect word statistics
        parse_file(is);
        // make a word list from the collected data
        std::list<Record> wordlist = make_wordlist();
        // sort the list according to comparator
        wordlist.sort(mComparator);
        // clear parser's internal data
        wordCounter.clear();

        return wordlist;
    }
}

