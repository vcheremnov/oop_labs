#include <iostream>
#include <fstream>
#include <libgen.h>
#include "csvmaker.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << basename(argv[0])
                    << " [in_file] [out_file]" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << argv[1] << "!" << std::endl;
        return 1;
    }

    WordParser parser;
    std::list<WordItem> wordList = parser.parse_file(ifs);


//    std::ofstream ofs(argv[2]);
//    if (!ofs.is_open()) {
//        std::cerr << "Failed to open " << argv[2] << "!" << std::endl;
//        return 1;
//    }

    ifs.close();
//    ofs.close();

    return 0;
}
