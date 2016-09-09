#include "gtest/gtest.h"

#include "yunomi/array/filter11array.hpp"
#include "yunomi/code/fibonacci.hpp"

void set_array(yunomi::array::BitSliceArray  &array);

void set_array_all_one(yunomi::array::BitSliceArray  &array){
    for(uint64_t i = 0; i < array.size(); ++i){
        array(i, i + 1) = 1;
    }
}

TEST(TestFilter11Array, test_init) {
    yunomi::array::BitSliceArray  array(1024);
    set_array(array);

    yunomi::array::Filter11Array f11array(array);
}

TEST(TestFilter11Array, test_size) {
    yunomi::array::BitSliceArray  array(1024);
    set_array(array);

    yunomi::array::Filter11Array f11array(array);
    ASSERT_EQ(1024, f11array.size());
}

TEST(TestFilter11Array, test_n_ones) {
    yunomi::array::BitSliceArray  array(1024);
    set_array(array);

    yunomi::array::Filter11Array f11array(array);
    ASSERT_EQ(20, f11array.n_ones());
}

TEST(TestFilter11Array, test_call) {
    yunomi::array::BitSliceArray  array(1024);
    set_array(array);

    yunomi::array::Filter11Array f11array(array);
    for(uint64_t i = 0; i < 20; ++i){
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 0, i * 25 + 1));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 1, i * 25 + 2));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 2, i * 25 + 3));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 3, i * 25 + 4));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 4, i * 25 + 5));

        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 5, i * 25 + 6));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 6, i * 25 + 7));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 7, i * 25 + 8));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 8, i * 25 + 9));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 9, i * 25 + 10));

        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 10, i * 25 + 11));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 11, i * 25 + 12));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 12, i * 25 + 13));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 13, i * 25 + 14));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 14, i * 25 + 15));

        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 15, i * 25 + 16));
        EXPECT_EQ(1, (uint64_t) f11array(i * 25 + 16, i * 25 + 17));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 17, i * 25 + 18));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 18, i * 25 + 19));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 19, i * 25 + 20));

        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 20, i * 25 + 21));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 21, i * 25 + 22));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 22, i * 25 + 23));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 23, i * 25 + 24));
        EXPECT_EQ(0, (uint64_t) f11array(i * 25 + 24, i * 25 + 25));
    }
}

TEST(TestFilter11Array, test_call_all_one) {
    yunomi::array::BitSliceArray  array(1024);
    set_array_all_one(array);

    yunomi::array::Filter11Array f11array(array);
    for(uint64_t i = 0; i < 512; ++i) {
        EXPECT_EQ(0, (uint64_t) f11array(i * 2 + 0, i * 2 + 1));
        EXPECT_EQ(1, (uint64_t) f11array(i * 2 + 1, i * 2 + 2));
    }
}

TEST(TestFilter11Array, test_call2) {
    yunomi::array::BitSliceArray array(9009);
    yunomi::code::Fibonacci fib;

    uint64_t cursor = 0;
    std::vector<uint64_t> one_pos;
    for(uint64_t i = 0; i < 706; ++i){
        uint64_t value = i + 1;
        uint64_t encoded = fib.encode(value);
        uint64_t n_digits = yunomi::core::Register(encoded).n_digits();
        array(cursor, cursor + n_digits) = encoded;
        cursor += n_digits;
        one_pos.push_back(cursor - 1);
        ASSERT_LT(cursor, 10000);
    }
    ASSERT_EQ(9009, cursor);

    yunomi::array::Filter11Array filter(array);

    cursor = 0;
    for(uint64_t i = 0; i < 706; ++i){
        uint64_t target = one_pos[i];
        for(uint64_t j = cursor; j < target; ++j){
            ASSERT_EQ(0, filter(j, j + 1));
        }
        ASSERT_EQ(1, filter(target, target + 1));

        cursor = target + 1;
    }
}

TEST(TestFilter11Array, test_resize) {
    yunomi::array::BitSliceArray  array(1024);
    set_array_all_one(array);

    yunomi::array::Filter11Array f11array(array);
    ASSERT_THROW(f11array.resize(2000), std::runtime_error);
}
