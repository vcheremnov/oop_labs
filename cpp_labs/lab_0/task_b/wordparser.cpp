#include "wordparser.h"

bool WordParser::compare_default(const WordItem &first, const WordItem &second) {
    return (first.frequency == second.frequency) ?
                first.word < second.word : first.frequency > second.frequency;
}

void WordParser::reset() {
    wordCounter.clear();
    wordCount = 0;
}

void WordParser::add_word(const std::string &word) {
    wordCounter[word] += 1;
    wordCount += 1;
}

void WordParser::parse_line(const std::string &line) {
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

void WordParser::parse_file(std::istream &is) {
    std::string line;
    while (std::getline(is, line)) {
        parse_line(line);
    }
}

std::list<WordItem> WordParser::make_wordlist() {
    std::list<WordItem> wordlist;
    for (const auto &item: wordCounter) {
        wordlist.emplace_back(item.first, item.second,
                                    static_cast<double>(item.second) / wordCount * 100.0);
    }
    return wordlist;
}

std::list<WordItem> WordParser::get_wordlist_from(std::istream &is) {
    // parse file & collect word statistics
    parse_file(is);
    // make a word list from the collected data
    std::list<WordItem> wordlist = make_wordlist();
    // sort the list according to user's comparator
    wordlist.sort(compare);
    // clear parser's internal data related to this file
    reset();

    return wordlist;
}
