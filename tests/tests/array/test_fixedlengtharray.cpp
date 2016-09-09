#include "gtest/gtest.h"

#include "yunomi/array/fixedlengtharray.hpp"

TEST(TestFixedLengthArray, test_init){
    yunomi::array::FixedLengthArray array(10, 1);
    EXPECT_EQ(10, array.size());
    EXPECT_EQ(1, array.bits_per_slot());
}

TEST(TestFixedLengthArray, test_ten_values){
    yunomi::array::FixedLengthArray array(10, 4);
    EXPECT_EQ(10, array.size());
    EXPECT_EQ(4, array.bits_per_slot());
    for(std::size_t i = 0; i < 10; ++i){
        array[i] = i;
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 0; i < 10; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values){
    yunomi::array::FixedLengthArray array(1000, 10);
    EXPECT_EQ(1000, array.size());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_resize){
    yunomi::array::FixedLengthArray array(1000, 10);
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    array.resize(1500, 10);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_resize2){
    yunomi::array::FixedLengthArray array(1000, 10);
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    array.resize(1500, 11);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values_template){
    yunomi::array::ConstLengthArray<10> array(1000);
    EXPECT_EQ(1000, array.size());
    EXPECT_EQ(10, array.bits_per_slot());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    array.resize(1500);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values_template_8bit){
    yunomi::array::ConstLengthArray<8> array(1000);
    EXPECT_EQ(1000, array.size());
    EXPECT_EQ(8, array.bits_per_slot());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i % 256;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i % 256, uint64_t(array[i]));
    }
    array.resize(1500);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i % 256, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values_template_16bit){
    yunomi::array::ConstLengthArray<16> array(1000);
    EXPECT_EQ(1000, array.size());
    EXPECT_EQ(16, array.bits_per_slot());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    array.resize(1500);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values_template_32bit){
    yunomi::array::ConstLengthArray<32> array(1000);
    EXPECT_EQ(1000, array.size());
    EXPECT_EQ(32, array.bits_per_slot());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    array.resize(1500);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}

TEST(TestFixedLengthArray, test_thousand_values_template_64bit){
    yunomi::array::ConstLengthArray<64> array(1000);
    EXPECT_EQ(1000, array.size());
    EXPECT_EQ(64, array.bits_per_slot());
    for(std::size_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    array.resize(1500);
    for(std::size_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, uint64_t(array[i]));
    }
    for(std::size_t i = 1000; i < 1500; ++i){
        EXPECT_EQ(0, uint64_t(array[i]));
    }
}
//TODO write tests
//TODO write tests for the size==0