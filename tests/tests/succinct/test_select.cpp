#include "gtest/gtest.h"

#include "yunomi/succinct/select.hpp"

void set_array(yunomi::array::BitSliceArray &array);

TEST(TestSelect, test_init){
    yunomi::array::BitSliceArray array(1024);
    set_array(array);

    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);
    yunomi::succinct::Select< decltype(index) > select(index);
}

TEST(TestSelect, test_select_values){
    yunomi::array::BitSliceArray array;
    set_array(array);

    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);
    yunomi::succinct::Select< decltype(index) > select(index);

    for(std::size_t i = 0; i < 20; ++i){
        EXPECT_EQ(25 * i + 5, select(5 * i + 0));
        EXPECT_EQ(25 * i + 11, select(5 * i + 1));
        EXPECT_EQ(25 * i + 15, select(5 * i + 2));
        EXPECT_EQ(25 * i + 16, select(5 * i + 3));
        EXPECT_EQ(25 * i + 22, select(5 * i + 4));
    }

    EXPECT_EQ(array.size(), select(100));
}


