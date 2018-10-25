#ifndef CALC_MODES_H
#define CALC_MODES_H

#include <string>

namespace Calc {

void print_usage(const std::string &programName);
int interactive_mode();
int file_mode(const std::string &inFile, const std::string &outFile);

} // namespace Calc

#endif // CALC_MODES_H
