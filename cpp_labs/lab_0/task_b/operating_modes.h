#ifndef OPERATING_MODES_H
#define OPERATING_MODES_H

#include <iostream>

namespace Main {
    void print_usage(const std::string &programName);
    int interactive_mode();
    int file_mode(const std::string &inFile, const std::string &outFile);
    void process_stream(std::istream &is, std::ostream &os);
}

#endif // OPERATING_MODES_H
