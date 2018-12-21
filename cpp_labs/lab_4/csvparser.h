#pragma once

#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <utility>
#include <algorithm>
#include "tuple_printing.h"

template<typename ...Ts>
class CSVParser {
public:
    using value_type = std::tuple<Ts...>;

    // iterator
    class iterator: public std::iterator<std::input_iterator_tag, value_type,
                                         long, value_type*, value_type> {
    public:
        // create end iterator
        iterator(): _isEnd(true) {}

        // create begin iterator
        explicit iterator(CSVParser &parser): _parser(&parser) {
            auto &inputStream = _parser->_inputStream;
            // skip requested number of lines
            for (; !inputStream.eof() && _lineNo < _parser->_linesToSkip; ++_lineNo) {
                inputStream.ignore(std::numeric_limits<std::streamsize>::max(), inputStream.widen('\n'));
            }
            // get initial value
            _val = _parser->_retrieve_record();
            _isEnd = inputStream.eof();
        }

        iterator& operator++() {
            if (!_isEnd) {
                ++_lineNo;
                _val = _parser->_retrieve_record();
                _isEnd = _parser->_inputStream.eof();
            }
            return *this;
        }

        iterator operator++(int) {
            iterator resIt = *this;
            ++(*this);
            return resIt;
        }

        bool operator==(const iterator &other)
            { return (_isEnd && other._isEnd) || (!_isEnd && !other._isEnd && (_lineNo == other._lineNo)); }

        bool operator!=(const iterator &other)
            { return !(*this == other); }

        value_type &operator*()
            { return _val; }
    private:
        CSVParser* _parser = nullptr;
        bool _isEnd = false;
        std::size_t _lineNo = 0;
        value_type _val;
    }; // iterator

    CSVParser(std::istream &inputStream, char delimiter = ',', std::size_t linesToSkip = 0):
        _inputStream(inputStream), _delimiter(delimiter), _linesToSkip(linesToSkip) {}
    iterator begin()
        { return iterator(*this); }
    iterator end()
        { return iterator(); }
private:
    char _delimiter;
    std::size_t _linesToSkip;
    std::istream& _inputStream;
    // private methods
    value_type _retrieve_record();
    std::vector<std::string> _get_tokens_from(const std::string &line);
    template<typename T>
    static T _retrieve_value(const std::string &token);
};


template<typename ...Ts>
typename CSVParser<Ts...>::value_type CSVParser<Ts...>::_retrieve_record() {
    if (!_inputStream.eof()) {
        std::string line;
        std::getline(_inputStream, line);
        if (!_inputStream.eof()) {
            auto tokenList = _get_tokens_from(line);
            auto tokenIter = tokenList.begin();
            return std::tuple<Ts...>{_retrieve_value<Ts>(*tokenIter++)...};
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
T CSVParser<Ts...>::_retrieve_value(const std::string &token) {
    static std::istringstream convertStream;

    T val;
    convertStream.clear();
    convertStream.str(token);
    convertStream >> val;
    return val;
}
