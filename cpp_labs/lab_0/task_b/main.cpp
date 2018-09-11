#include <iostream>
#include <libgen.h>
#include "csvmaker.h"

enum Argv {
    ProgName = 0,
    InFile = 1,
    OutFile = 2,
    ArgcMin = 3
};

int main(int argc, char *argv[]) {
    if (argc < ArgcMin) {
        std::cout << "Usage: " << basename(argv[ProgName])
                    << " [in_file] [out_file]" << std::endl;
        return 1;
    }

    CSVmaker recycler;
    recycler.make_csv(argv[InFile], argv[OutFile]);

    return 0;
}
