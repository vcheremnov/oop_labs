#ifndef TRITWISE_OPERATIONS_H
#define TRITWISE_OPERATIONS_H

enum class Trit {
    Unknown,
    False,
    True
};

Trit operator& (Trit val1, Trit val2);
Trit operator| (Trit val1, Trit val2);
Trit operator~ (Trit val);


#endif // TRITWISE_OPERATIONS_H
