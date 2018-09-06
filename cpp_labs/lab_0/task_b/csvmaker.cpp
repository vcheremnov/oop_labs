#include <iostream>
#include <fstream>
#include "csvmaker.h"

std::list<WordItem> CSVmaker::get_word_list(const std::string &inFile) {
    std::ifstream ifs(inFile);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << inFile << "!" << std::endl;
        return std::list<WordItem>();
    }
    std::list<WordItem> wordList = parser.parse_file(ifs);
    ifs.close();
    return wordList;
}

void CSVmaker::make_csv(const std::string &inFile, const std::string &outFile) {
    std::list<WordItem> wordList = get_word_list(inFile);

    if (!wordList.empty()) {
        std::ofstream ofs(outFile);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open " << inFile << "!" << std::endl;
            return;
        }
        // put records to the file
        ofs.precision(3);   // 3 decimal spaces
        for (const auto &record: wordList) {
            ofs << record.word << "," << record.frequency << ","
                << record.fraction << std::endl;
        }
        ofs.close();
    }
}
