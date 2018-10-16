#include <iostream>
#include "calculator.h"

int main() {
    Calculator calc(std::cin, std::cout);
    calc.calculate();
    return 0;
}
