#pragma once

#include <iostream>
#include <sstream>
#include <tuple>
#include <limits>
#include <vector>
#include <algorithm>

template<typename ...Ts>
class CSVParser {
public:
    using value_type = std::tuple<Ts...>;
    // iterator
    class iterator: public std::iterator<std::input_iterator_tag, value_type,
                                         long, value_type*, value_type> {
    public:
        explicit iterator(CSVParser &parser, bool reachedEnd = false):
            _parser(parser), _reachedEnd(reachedEnd) {
            if (reachedEnd) {
                _lineNo = std::numeric_limits<std::size_t>::max();
            }
            else {
                // skip requested number of lines
                for (; _lineNo < _parser._linesToSkip; ++_lineNo) {
                    _parser._inputStream.ignore(std::numeric_limits<std::streamsize>::max(),
                                                _parser._inputStream.widen('\n'));
                }
                // get initial value
                _val = _parser._retrieve_value();
                _reachedEnd |= _parser._inputStream.eof();
            }
        }
        iterator& operator++() {
            _lineNo = _reachedEnd ? _lineNo : (_lineNo + 1);
            _val = _parser._retrieve_value();
            _reachedEnd |= _parser._inputStream.eof();
            return *this;
        }
        iterator operator++(int) {
            iterator resIt = *this;
            ++(*this);
            return resIt;
        }
        bool operator==(const iterator &other) {
            return (_reachedEnd && other._reachedEnd) || (_lineNo == other._lineNo);
        }
        bool operator!=(const iterator &other) {
            return !(*this == other);
        }
        value_type &operator*() {
            return _val;
        }
    private:
        CSVParser& _parser;
        bool _reachedEnd = false;
        std::size_t _lineNo = 0;
        value_type _val;
    };

    CSVParser(std::istream &inputStream, char delimiter = ',', std::size_t linesToSkip = 0);
    iterator begin()
        { return iterator(*this, false); }
    iterator end()
        { return iterator(*this, true); }
private:
    char _delimiter;
    std::size_t _linesToSkip;
    std::istream& _inputStream;
    // private methods
    value_type _retrieve_value();
    std::vector<std::string> _get_tokens_from(const std::string &line);
    template<typename T>
    static T _retrieve_type_value(const std::string &token);
};


template<typename ...Ts>
CSVParser<Ts...>::CSVParser(std::istream &inputStream, char delimiter, std::size_t linesToSkip):
    _inputStream(inputStream), _delimiter(delimiter), _linesToSkip(linesToSkip) {}

template<typename ...Ts>
typename CSVParser<Ts...>::value_type CSVParser<Ts...>::_retrieve_value() {
    if (!_inputStream.eof()) {
        std::string line;
        std::getline(_inputStream, line);
        if (!_inputStream.eof()) {
            auto tokenList = _get_tokens_from(line);
            auto tokenIter = tokenList.begin();
            return {_retrieve_type_value<Ts>(*tokenIter++)...};
        }
    }
    return std::make_tuple<>(Ts()...);
}

template<typename ...Ts>
std::vector<std::string> CSVParser<Ts...>::_get_tokens_from(const std::string &line) {
    std::istringstream sstr(line);
    std::string token;

    std::vector<std::string> tokenList;
    while (std::getline(sstr, token, _delimiter)) {
        tokenList.push_back(token);
    }

    return tokenList;
}

template<typename ...Ts>
template<typename T>
T CSVParser<Ts...>::_retrieve_type_value(const std::string &token) {
    static std::istringstream convertStream;

    convertStream.clear();
    convertStream.str(token);
    T val;
    convertStream >> val;
    return val;
}

// tuple printing

template<std::size_t ...> struct seq{};

template<std::size_t N, std::size_t ...Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>{};

template<std::size_t ...Is>
struct gen_seq<0, Is...> : seq<Is...>{};

template<typename Ostream, typename Tuple, std::size_t... Is>
void print_tuple(Ostream &os, const Tuple &t, seq<Is...>){
    using dummy = int[];
    (void)dummy{0, (os << (Is == 0 ? "" : ", ") << std::get<Is>(t), 0)...};
}

template<typename Ch, typename Tr, typename ...Ts>
auto &operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Ts...> &t) {
    print_tuple(os, t, gen_seq<sizeof...(Ts)>{});
    return os;
}
