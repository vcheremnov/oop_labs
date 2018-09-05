#include <module2.h>
#include <iostream>

namespace Module2 {
    std::string getMyName() {
        return std::string("Name #2");
    }

    namespace Module3 {
        std::string getMyName() {
            return std::string("Name #3");
        }
    }
}
