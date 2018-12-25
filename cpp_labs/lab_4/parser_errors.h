#pragma once

#include <stdexcept>
#include <exception>

namespace ParserError {

class Error: public std::exception {
public:
    Error(const std::string &line = "", std::size_t lineNo = 0, std::size_t columnNo = 0):
        _line(line), _lineNo(lineNo), _columnNo(columnNo) {}
    const std::string& get_line() const
        { return _line; }
    std::size_t get_line_number() const
        { return _lineNo; }
    std::size_t get_column_number() const
        { return _columnNo; }
    void set_line(const std::string &line)
        { _line = line; }
    void set_line_number(std::size_t lineNo)
        { _lineNo = lineNo; }
    void set_column_number(std::size_t columnNo)
        { _columnNo = columnNo; }
    const char *what() const noexcept override
        { return "Parser error has occurred"; }
private:
    std::string _line;
    std::size_t _lineNo;
    std::size_t _columnNo;
};

class ValueNumberError: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Incorrect number of values"; }
};

class EscapedValueError: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Missing closing escape character"; }
};

class DelimiterError: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Missing delimiter character after the escaped value"; }
};

class UnexpectedEscapeChar: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Unexpected escape character"; }
};

class ValueMakingError: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Failed to make a value from the column"; }
};

class ValueFormatError: public Error {
public:
    using Error::Error;
    const char *what() const noexcept override
        { return "Invalid column format for making a requested value"; }
};

} // namespace ParserError

