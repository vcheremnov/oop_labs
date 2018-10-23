#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../calculator.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}