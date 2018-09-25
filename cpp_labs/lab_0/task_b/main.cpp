#include <iostream>
#include <fstream>
#include <libgen.h>
#include "wordlist.h"

namespace Argv {
    const int PROG_NAME = 0;
    const int IN_FILENAME = 1;
    const int OUT_FILENAME = 2;
    const int DEFAULT_MODE_ARGC = 1;
    const int FILE_MODE_ARGC = 3;
}

void print_usage(const std::string &filename) {
    std::cerr << "Usage:\n"
              << filename << " <no args> - default mode\n"
              << filename << " [in_file] [out_file] - file mode"
              << std::endl;
}

void default_mode() {
    WordList list;
    list.fill(std::cin);
    list.print_as_csv(std::cout);
}

void file_mode(const std::string &inFile, const std::string &outFile) {
    WordList list;
    std::ifstream fin(inFile);
    if (!fin.is_open()) {
        throw inFile;
    }
    std::ofstream fout(outFile);
    if (!fout.is_open()) {
        throw outFile;
    }
    list.fill(fin);
    list.print_as_csv(fout);
}

int main(int argc, char *argv[]) {
    if (argc == Argv::DEFAULT_MODE_ARGC) {
        // use standard streams
        default_mode();
    }
    else if (argc == Argv::FILE_MODE_ARGC) {
        // use file streams
        try {
            file_mode(argv[Argv::IN_FILENAME], argv[Argv::OUT_FILENAME]);
        }
        catch (const std::string &filename) {
            std::cerr << "Failed to open " << filename << "!" << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        print_usage(basename(argv[Argv::PROG_NAME]));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
