#include <iostream>
#include <fstream>
#include "csvparser.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open the file!");
        }

        CSVParser<std::string, int, double> parser(file);
        for (auto &record: parser) {
            std::cout << record << std::endl;
        }
    }

    return 0;
}
