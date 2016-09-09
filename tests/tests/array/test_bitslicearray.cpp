#include "gtest/gtest.h"

#include "yunomi/array/bitslicearray.hpp"

TEST(TestBitSliceArray, test_init){
    yunomi::array::BitSliceArray array;
}

TEST(TestBitSliceArray, test_set_get){
    yunomi::array::BitSliceArray array;

    uint64_t ten_bits_value = array(5, 15);
    EXPECT_EQ(0, ten_bits_value);

    array(5, 15) = 512;
    ten_bits_value = array(5, 15);
    EXPECT_EQ(512, ten_bits_value);
}

TEST(TestBitSliceArray, test_set_get2){
    yunomi::array::BitSliceArray array;

    uint64_t zero_bits_value = array(5, 5);
    EXPECT_EQ(0, zero_bits_value);

    array(5, 6) = 1;
    zero_bits_value = array(5, 5);
    EXPECT_EQ(0, zero_bits_value);
}

TEST(TestBitSliceArray, test_set_get3){
    yunomi::array::BitSliceArray array;

    uint64_t sixty_four_bits_value = array(5, 69);
    EXPECT_EQ(0, sixty_four_bits_value);

    array(5, 69) = std::numeric_limits<uint64_t>::max();
    sixty_four_bits_value = array(5, 69);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), sixty_four_bits_value);
}

TEST(TestBitSliceArray, test_set_get4){
    yunomi::array::BitSliceArray array;

    uint64_t ten_bits_value = array(0, 10);
    EXPECT_EQ(0, ten_bits_value);

    array(0, 10) = 1023;
    ten_bits_value = array(0, 10);
    EXPECT_EQ(1023, ten_bits_value);
}

TEST(TestBitSliceArray, test_set_get5){
    yunomi::array::BitSliceArray array;

    array(0, 10) = 1023;
    array(10, 20) = 1000;
    uint64_t ten_bits_value = array(0, 10);
    EXPECT_EQ(1023, ten_bits_value);
    ten_bits_value = array(10, 20);
    EXPECT_EQ(1000, ten_bits_value);
}

TEST(TestBitSliceArray, test_set_get6){
    yunomi::array::BitSliceArray array;

    array(5, 15) = 512;
    array(16, 80) = 0xFFFFFFFFULL;
    EXPECT_EQ(512, array(5, 15));
    EXPECT_EQ(0xFFFFFFFFULL, array(16, 80));
}

TEST(TestBitSliceArray, test_size){
    yunomi::array::BitSliceArray array;
    EXPECT_EQ(array.size(), 1024);
}

TEST(TestBitSliceArray, test_resize){
    yunomi::array::BitSliceArray array;
    array(5, 15) = 512;

    array.resize(2048);

    uint64_t ten_bits_value = array(5, 15);
    EXPECT_EQ(512, ten_bits_value);
}

TEST(TestBitSliceArray, test_n_ones){
    yunomi::array::BitSliceArray array;
    EXPECT_EQ(0, array.n_ones());

    array(5, 15) = 512;
    EXPECT_EQ(1, array.n_ones());
}

TEST(TestBitSliceArray, test_n_ones2){
    yunomi::array::BitSliceArray array;
    EXPECT_EQ(0, array.n_ones());

    array(5, 15) = 512;
    EXPECT_EQ(512, array(5, 15));
    EXPECT_EQ(1, yunomi::core::Register(array(5, 15)).n_ones());

    array(16, 80) = 0xFFFFFFFFFFFFFFFFULL;
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFULL, array(16, 80));
    EXPECT_EQ(64, yunomi::core::Register(array(16, 80)).n_ones());

    uint64_t count = 0;
    count += yunomi::core::Register(array(0, 64)).n_ones();
    count += yunomi::core::Register(array(64, 128)).n_ones();
    count += yunomi::core::Register(array(128, 192)).n_ones();
    count += yunomi::core::Register(array(192, 256)).n_ones();
    EXPECT_EQ(65, count);

    array(80, 144) = 0xFFFFFFFFFFFFFFFFULL;
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFULL, array(80, 144));

    count = 0;
    count += yunomi::core::Register(array(0, 64)).n_ones();
    count += yunomi::core::Register(array(64, 128)).n_ones();
    count += yunomi::core::Register(array(128, 192)).n_ones();
    count += yunomi::core::Register(array(192, 256)).n_ones();
    EXPECT_EQ(129, count);
}

TEST(TestBitSliceArray, test_copy_init){
    yunomi::array::BitSliceArray array;
    array(5, 15) = 512;
    array(16, 80) = 0xFFFFFFFFFFFFFFFFULL;
    array(80, 144) = 0xFFFFFFFFFFFFFFFFULL;

    yunomi::array::BitSliceArray array2(array);
    EXPECT_EQ(512, array2(5, 15));
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFULL, array2(16, 80));
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFULL, array2(80, 144));
    uint64_t count = 0;
    count += yunomi::core::Register(array(0, 64)).n_ones();
    count += yunomi::core::Register(array(64, 128)).n_ones();
    count += yunomi::core::Register(array(128, 192)).n_ones();
    count += yunomi::core::Register(array(192, 256)).n_ones();
    EXPECT_EQ(129, count);
}

TEST(TestBitSliceArray, test_clear){
    yunomi::array::BitSliceArray array;
    array(5, 15) = 512;
    array(16, 80) = 0xFFFFFFFFFFFFFFFFULL;
    array(80, 144) = 0xFFFFFFFFFFFFFFFFULL;

    array.clear();
    EXPECT_EQ(0, array(5, 15));
    EXPECT_EQ(0, array(16, 80));
    EXPECT_EQ(0, array(80, 144));
    uint64_t count = 0;
    count += yunomi::core::Register(array(0, 64)).n_ones();
    count += yunomi::core::Register(array(64, 128)).n_ones();
    count += yunomi::core::Register(array(128, 192)).n_ones();
    count += yunomi::core::Register(array(192, 256)).n_ones();
    EXPECT_EQ(0, count);
}
//TODO write more tests.