#include <algorithm>
#include <unordered_map>
#include <fstream>
#include "wordlist.h"

namespace Words {

bool compare_default(const Record &left, const Record &right) {
    // sorts by the word occurrence in descending order
    // if equal, sorts by words in alphabetical order
    return (left.second == right.second) ?
                left.first < right.first : left.second > right.second;
}

void List::fill(std::istream &is) {
    // create a parser to retrieve words from the given stream
    Parser parser(is);
    // map used to count the words
    std::unordered_map<std::string, unsigned> wordCounter;
    // reset total words counter
    _totalWords = 0;
    // collect the information from the stream
    std::string word;
    while (parser.get_word(word)) {
        ++wordCounter[word];
        ++_totalWords;
    }
    // make the list from the map
    _wordlist.assign(wordCounter.cbegin(), wordCounter.cend());
}

void List::print_as_csv(std::ostream &os) {
    // print in csv format: <word>,<number of occurrences>,<fraction of occurrences>
    // set precision to 3 decimal places
    auto oldPrecision = os.precision(3);
    for (const auto &record: _wordlist) {
        os << record.first << "," << record.second << ","
            << 100.0 * record.second / _totalWords << std::endl;
    }
    os.precision(oldPrecision);
}

void List::print_as_dict(std::ostream &os) {
    // print in dictionary format: <word> : <number of occurrences>
    for (const auto &record: _wordlist) {
        os << record.first << ": " << record.second << std::endl;
    }
}

} // namespace Words

