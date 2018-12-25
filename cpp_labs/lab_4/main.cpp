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
        try {
            for (auto &record: parser) {
                std::cout << record << std::endl;
            }
        }
        catch (const ParserError::Error &exc) {
            std::cerr << "Error at line " << exc.get_line_number() << ", column " << exc.get_column_number()
                      << ": " << std::endl << exc.get_line() << std::endl << exc.what() << std::endl;
        }
    }

    return 0;
}
