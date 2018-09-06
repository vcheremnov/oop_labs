#include "wordparser.h"

bool WordItem::compare_default(const WordItem &first, const WordItem &second) {
    return first.frequency > second.frequency;
}

void WordParser::reset() {
    words.clear();
    wordCount = 0;
}

void WordParser::add_word(const std::string &word) {
    words[word] += 1;
    wordCount += 1;
}

void WordParser::parse_line(const std::string &line) {
    auto wordStart = line.cbegin();
    for (auto cur = line.cbegin(); cur != line.cend(); ++cur) {
        if (isalnum(*cur)) {
            continue;
        }
        if (cur != wordStart) {
            add_word(std::string(wordStart, cur));
            wordStart = cur + 1;
        }
        else {
            ++wordStart;
        }
    }
    if (wordStart != line.cend()) {
        add_word(std::string(wordStart, line.cend()));
    }
}

std::list<WordItem> WordParser::parse_file(std::istream &is) {
    // parse file
    std::string line;
    while (std::getline(is, line)) {
        parse_line(line);
    }
    // make a word list
    std::list<WordItem> wordList;
    for (const auto &item: words) {
        wordList.push_back(WordItem(item.first, item.second,
                                    (double)item.second / wordCount * 100.0));
    }
    // sort the list
    wordList.sort(compare);
    // reset parser's internal info related to this file
    reset();

    return wordList;
}
