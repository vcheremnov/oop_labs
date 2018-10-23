#include <iostream>
#include "calculator.h"
#include "commandfactory.h"

int main() {
    Calculator calc(std::cin, std::cout);
    calc.calculate();
    return 0;
}
