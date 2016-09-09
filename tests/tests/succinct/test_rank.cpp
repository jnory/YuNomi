#include "gtest/gtest.h"

#include "yunomi/succinct/rank.hpp"

void set_array(yunomi::array::BitSliceArray &array){
    // 00000 10000 01000 11000 00100 ... (repeat them 20 times)
    for(uint64_t i = 0; i < 100; ++i){
        array(i * 5, (i + 1) * 5) = i % 5;
    }
}

TEST(TestRank, test_init){
    yunomi::array::BitSliceArray array(1024);
    set_array(array);

    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);
    yunomi::succinct::Rank< decltype(index) > rank(index);
}

TEST(TestRank, test_rank_values){
    yunomi::array::BitSliceArray array;
    set_array(array);

    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);
    yunomi::succinct::Rank< decltype(index) > rank(index);

    for(uint64_t i = 0; i < 20; ++i){
        EXPECT_EQ(5 * i + 0, rank(25 * i + 0));
        EXPECT_EQ(5 * i + 0, rank(25 * i + 1));
        EXPECT_EQ(5 * i + 0, rank(25 * i + 2));
        EXPECT_EQ(5 * i + 0, rank(25 * i + 3));
        EXPECT_EQ(5 * i + 0, rank(25 * i + 4));
        EXPECT_EQ(5 * i + 0, rank(25 * i + 5));

        EXPECT_EQ(5 * i + 1, rank(25 * i + 6));
        EXPECT_EQ(5 * i + 1, rank(25 * i + 7));
        EXPECT_EQ(5 * i + 1, rank(25 * i + 8));
        EXPECT_EQ(5 * i + 1, rank(25 * i + 9));
        EXPECT_EQ(5 * i + 1, rank(25 * i + 10));
        EXPECT_EQ(5 * i + 1, rank(25 * i + 11));

        EXPECT_EQ(5 * i + 2, rank(25 * i + 12));
        EXPECT_EQ(5 * i + 2, rank(25 * i + 13));
        EXPECT_EQ(5 * i + 2, rank(25 * i + 14));
        EXPECT_EQ(5 * i + 2, rank(25 * i + 15));

        EXPECT_EQ(5 * i + 3, rank(25 * i + 16));

        EXPECT_EQ(5 * i + 4, rank(25 * i + 17));
        EXPECT_EQ(5 * i + 4, rank(25 * i + 18));
        EXPECT_EQ(5 * i + 4, rank(25 * i + 19));
        EXPECT_EQ(5 * i + 4, rank(25 * i + 20));
        EXPECT_EQ(5 * i + 4, rank(25 * i + 21));
        EXPECT_EQ(5 * i + 4, rank(25 * i + 22));

        EXPECT_EQ(5 * i + 5, rank(25 * i + 23));
        EXPECT_EQ(5 * i + 5, rank(25 * i + 24));
    }

    for(uint64_t i = 500; i < array.size(); ++i){
        EXPECT_EQ(100, rank(i));
    }
    EXPECT_EQ(100, rank(501));
}

//TODO write tests

