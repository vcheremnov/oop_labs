#include <iostream>
#include <libgen.h>
#include "csvmaker.h"

enum Argv {
    InFile = 1,
    OutFile = 2,
    argcMin = 3
};

int main(int argc, char *argv[]) {
    if (argc < argcMin) {
        std::cout << "Usage: " << basename(argv[0])
                    << " [in_file] [out_file]" << std::endl;
        return 1;
    }

    CSVmaker recycler;
    recycler.make_csv(argv[InFile], argv[OutFile]);

    return 0;
}
