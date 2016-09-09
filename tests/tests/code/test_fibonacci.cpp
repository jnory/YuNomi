#include "gtest/gtest.h"

#include "yunomi/code/fibonacci.hpp"

TEST(TestFibFunc, test_val) {
    yunomi::code::FibFunc ff;
    ASSERT_EQ(1, ff[0]);
    ASSERT_EQ(1, ff[1]);
    ASSERT_EQ(2, ff[2]);
    ASSERT_EQ(3, ff[3]);
    ASSERT_EQ(5, ff[4]);
}

TEST(TestFibFunc, test_lookup_maximum_index_smaller_than) {
    yunomi::code::FibFunc ff;
    ASSERT_EQ(1, ff.lookup_maximum_index_smaller_than(1).index);
    ASSERT_EQ(2, ff.lookup_maximum_index_smaller_than(2).index);
    ASSERT_EQ(3, ff.lookup_maximum_index_smaller_than(4).index);
    ASSERT_EQ(4, ff.lookup_maximum_index_smaller_than(5).index);

    ASSERT_EQ(0, ff.lookup_maximum_index_smaller_than(1).rest);
    ASSERT_EQ(0, ff.lookup_maximum_index_smaller_than(2).rest);
    ASSERT_EQ(1, ff.lookup_maximum_index_smaller_than(4).rest);
    ASSERT_EQ(0, ff.lookup_maximum_index_smaller_than(5).rest);
}

TEST(TestFibonacci, test_encode){
    yunomi::code::FibFunc ff;
    yunomi::code::Fibonacci fib;
    ASSERT_EQ(0x3ULL, fib.encode(1));
    ASSERT_EQ(0xDULL, fib.encode(4));
    ASSERT_EQ(0xC000000000000000ULL, fib.encode(ff[63]));
    //TODO write more tests.
}

TEST(TestFibonacci, test_decode){
    yunomi::code::FibFunc ff;
    yunomi::code::Fibonacci fib;
    ASSERT_EQ(1, fib.decode(0x3ULL));
    ASSERT_EQ(4, fib.decode(0xDULL));
    ASSERT_EQ(ff[63], fib.decode(0xC000000000000000ULL));
    //TODO write more tests.
}

