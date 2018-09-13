#include <iostream>
#include <fstream>
#include "csvmaker.h"

void CSVmaker::get_wordlist_from(std::istream &is) {
    wordlist = parser.get_wordlist_from(is);
}

void CSVmaker::write_records_to(std::ostream &os) {
    // put records to the stream
    os.precision(3);   // set precision to 3 decimal places
    for (const auto &record: wordlist) {
        os << record.word << "," << record.frequency << ","
            << record.fraction << std::endl;
    }
}

void CSVmaker::make_csv(const std::string &inFile, const std::string &outFile) {
    // make a cleanup for the previous call
    reset();
    // get word list from the inFile
    std::ifstream ifs(inFile);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << inFile << "!" << std::endl;
        return;
    }
    get_wordlist_from(ifs);
    ifs.close();
    // write csv records to the outFile
    std::ofstream ofs(outFile);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open " << outFile << "!" << std::endl;
        return;
    }
    write_records_to(ofs);
    ofs.close();
}
