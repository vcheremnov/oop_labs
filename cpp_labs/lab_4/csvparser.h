#pragma once

#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <utility>
#include <algorithm>
#include "tuple_printing.h"
#include "parser_errors.h"

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
            for (; !_isEnd && _lineNo < _parser->_linesToSkip; ++_lineNo) {
                inputStream.ignore(std::numeric_limits<std::streamsize>::max(), inputStream.widen('\n'));
                _isEnd = inputStream.eof();
            }
            // get initial value
            _retrieve_record();
        }

        iterator& operator++() {
            if (!_isEnd) {
                ++_lineNo;
                _retrieve_record();
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
            { return _record; }
    private:
        CSVParser* _parser = nullptr;
        bool _isEnd = false;
        std::size_t _lineNo = 0;
        value_type _record;
        // private methods
        void _retrieve_record() {
            if (!_isEnd) {
                std::string line;
                // read a line, skipping empty ones
                do {
                    std::getline(_parser->_inputStream, line);
                    _isEnd = _parser->_inputStream.eof();
                } while (!_isEnd && line.empty());
                // retrieve record from the line
                try {
                    _record = _parser->_retrieve_record(line);
                }
                catch (ParserError::Error &exc) {
                    exc.set_line(line);
                    exc.set_line_number(_lineNo);
                    throw;
                }
            }
        }
    }; // iterator

    // constructor
    CSVParser(std::istream &inputStream, std::size_t linesToSkip = 0):
        _inputStream(inputStream), _linesToSkip(linesToSkip) {}
    // iterators
    iterator begin()
        { return iterator(*this); }
    iterator end()
        { return iterator(); }
    // configuration
    void set_delimiter(char columnDelimiter)
        { _delimiter = columnDelimiter; }
    void set_escape_character(char escapeChar)
        { _escapeChar = escapeChar; }
private:
    char _delimiter = ',';
    char _escapeChar = '\"';
    std::size_t _linesToSkip;
    std::istream& _inputStream;
    // private methods
    value_type _retrieve_record(const std::string &line);
    std::vector<std::string> _get_tokens_from(const std::string &line);
    // general value making method
    template<typename T, typename std::enable_if<!std::is_same<T, std::string>::value>::type* = nullptr>
    static T _retrieve_value(const std::string &token, std::size_t tokenNo) {
        static std::istringstream convertStream;

        T val;
        convertStream.clear();
        convertStream.str(token);
        convertStream >> val;

        if (!convertStream.eof()) {
            // eof was not reached => value format error
            throw ParserError::ValueFormatError("", 0, tokenNo);
        }
        else if (convertStream.fail()) {
            // failed to make a value
            throw ParserError::ValueMakingError("", 0, tokenNo);
        }

        return val;
    }
    // specialization for strings
    template<typename T, typename std::enable_if<std::is_same<T, std::string>::value>::type* = nullptr>
    static const T& _retrieve_value(const std::string &token, std::size_t) {
        return token;
    }
};


template<typename ...Ts>
typename CSVParser<Ts...>::value_type CSVParser<Ts...>::_retrieve_record(const std::string &line) {
    if (_inputStream.eof()) {
        return std::make_tuple<>(Ts()...);
    }
    auto tokenList = _get_tokens_from(line);
    auto tokenIter = tokenList.begin();
    std::size_t tokenNo = 0;
    return std::tuple<Ts...>{_retrieve_value<Ts>(*tokenIter++, tokenNo++)...};
}

template<typename ...Ts>
std::vector<std::string> CSVParser<Ts...>::_get_tokens_from(const std::string &line) {
    auto tokenBeg = line.cbegin(), tokenEnd = tokenBeg;
    auto lineEnd = line.cend();
    std::vector<std::string> tokenList;
    std::string token;
    bool isEscaped = false;

    for (auto columnNo = 0u; tokenBeg != lineEnd; ++columnNo, tokenBeg = tokenEnd) {
        isEscaped = (*tokenBeg == _escapeChar);
        if (isEscaped) { // escaped value
            ++tokenBeg; ++tokenEnd;
            token.clear();
            // searching the closing escape character
            do {
                tokenEnd = std::find_if(tokenEnd, lineEnd, [this](char symbol) { return symbol == _escapeChar; });
                if (tokenEnd == lineEnd) {
                    // unclosed escaped value
                    throw ParserError::EscapedValueError("", 0, columnNo);
                }
                // add the part checked so far
                token.append(tokenBeg, tokenEnd);
                // check if the met escape character is closing
                if (std::next(tokenEnd) != lineEnd && tokenEnd[1] == _escapeChar) {
                    // append escaped character
                    token.push_back(_escapeChar);
                    // skip escaped character
                    std::advance(tokenEnd, 2);
                    tokenBeg = tokenEnd;
                }
                else {
                    ++tokenEnd;
                    isEscaped = false;
                }
            } while (isEscaped);
            // add token
            tokenList.push_back(token);
            // move to another column
            if (tokenEnd != lineEnd) {
                if (*tokenEnd != _delimiter) {
                    throw ParserError::DelimiterError("", 0, columnNo);
                }
                ++tokenEnd;
            }
        }
        else { // ordinary value
            tokenEnd = std::find_if(tokenEnd, lineEnd,
                                    [this](char symbol) { return symbol == _escapeChar || symbol == _delimiter; });
            if (tokenEnd != lineEnd && *tokenEnd == _escapeChar) {
                throw ParserError::UnexpectedEscapeChar("", 0, columnNo);
            }
            // make token
            tokenList.emplace_back(tokenBeg, tokenEnd);
            // move to another column
            if (tokenEnd != lineEnd) {
                ++tokenEnd;
            }
        }
    }
    // check values number
    if (sizeof... (Ts) != tokenList.size()) {
        throw ParserError::ValueNumberError("", 0, tokenList.size());
    }

    return tokenList;
}
