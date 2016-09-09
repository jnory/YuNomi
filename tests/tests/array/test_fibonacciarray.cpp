#include "gtest/gtest.h"

#include "yunomi/array/fibonacciarray.hpp"

TEST(TestFibonacciArray, test_init) {
    yunomi::array::FibonacciDataStorage storage;
    for(uint64_t i = 0; i < 1024; ++i){
        storage.push_back(i + 1);
    }

    yunomi::array::FibonacciArray fa(storage);
    for(uint64_t i = 0; i < 1024; ++i){
        EXPECT_EQ(i + 1, fa[i]);
    }
}
