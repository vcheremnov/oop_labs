#include <iostream>
#include <fstream>
#include "calc_modes.h"
#include "calculator.h"

namespace Calc {

void print_usage(const std::string &programName) {
    std::cerr << "Usage:\n"
              << programName << " <no args> - interactive mode\n"
              << programName << " [in_file] [out_file] - file mode"
              << std::endl;
}

int process_file(std::istream &inFile, std::ostream &outFile) {
    Calculator calc(inFile, outFile);
    calc.calculate();
    return EXIT_SUCCESS;
}

int interactive_mode() {
    return process_file(std::cin, std::cout);
}

int file_mode(const std::string &inFile, const std::string &outFile) {
    try {
        std::ifstream fin(inFile);
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open " + inFile + "!");
        }
        std::ofstream fout(outFile);
        if (!fout.is_open()) {
            throw std::runtime_error("Failed to open " + outFile + "!");
        }
        return process_file(fin, fout);
    }
    catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

} // namespace Calc
