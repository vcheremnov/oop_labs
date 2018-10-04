#include <fstream>
#include <exception>
#include "wordlist.h"
#include "operating_modes.h"

namespace Main {

void print_usage(const std::string &programName) {
    std::cerr << "Usage:\n"
              << programName << " <no args> - default mode\n"
              << programName << " [in_file] [out_file] - file mode"
              << std::endl;
}

int interactive_mode() {
    process_stream(std::cin, std::cout);
    return EXIT_SUCCESS;
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
        process_stream(fin, fout);
        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

void process_stream(std::istream &is, std::ostream &os) {
    Words::List list;
    list.fill(is);
    list.sort();
    list.print_as_csv(os);
}

} // namespace Main
