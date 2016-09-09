#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "yunomi/array/bitslicearray.hpp"
#include "yunomi/array/fixedlengtharray.hpp"
#include "yunomi/array/filter11array.hpp"
#include "yunomi/array/fibonacciarray.hpp"
#include "yunomi/io/file.hpp"
#include "yunomi/succinct/rank.hpp"
#include "yunomi/succinct/dual_block_index.hpp"
#include "yunomi/serialize/hdf5.hpp"

void set_array(yunomi::array::BitSliceArray &array);

TEST(TestHDF5Serializer, test_init){
    std::string path = "h5s_test_init.bin";
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.close();
    ASSERT_TRUE(yunomi::io::File::exists(path));
    yunomi::io::File::remove(path);
    ASSERT_FALSE(yunomi::io::File::exists(path));
}

TEST(TestHDF5Serializer, test_read_write_bitslicearray){
    std::string path = "h5s_test_read_write_bitslicearray.bin";
    yunomi::array::BitSliceArray array(800);
    set_array(array);

    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::BitSliceArray array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(800, array2.size());
    for(uint64_t i = 0; i < 100; ++i){
        EXPECT_EQ(i % 5, array2(i * 5, (i + 1) * 5));
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_constlengtharray) {
    std::string path = "h5s_test_read_write_constlengtharray.bin";
    yunomi::array::ConstLengthArray<10> array(1000);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::ConstLengthArray<10> array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_constlengtharray8) {
    std::string path = "h5s_test_read_write_constlengtharray8.bin";
    yunomi::array::ConstLengthArray<8> array(1000);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = (uint8_t)(i % 256);
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::ConstLengthArray<8> array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i % 256, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_constlengtharray16) {
    std::string path = "h5s_test_read_write_constlengtharray16.bin";
    yunomi::array::ConstLengthArray<16> array(1000);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::ConstLengthArray<16> array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_constlengtharray32) {
    std::string path = "h5s_test_read_write_constlengtharray32.bin";
    yunomi::array::ConstLengthArray<32> array(1000);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::ConstLengthArray<32> array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_constlengtharray64) {
    std::string path = "h5s_test_read_write_constlengtharray64.bin";
    yunomi::array::ConstLengthArray<64> array(1000);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::ConstLengthArray<64> array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_fixedlengtharray) {
    std::string path = "h5s_test_read_write_fixedlengtharray.bin";
    yunomi::array::FixedLengthArray array(1000, 10);
    for(uint64_t i = 0; i < 1000; ++i){
        array[i] = i;
    }
    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::FixedLengthArray array2;
    deserializer.deserialize("array", array2);
    deserializer.close();

    ASSERT_EQ(1000, array2.size());
    for(uint64_t i = 0; i < 1000; ++i){
        EXPECT_EQ(i, array2[i]);
    }
    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_filter11) {
    std::string path = "h5s_test_read_write_filter11.bin";
    yunomi::array::BitSliceArray  array(1024);
    set_array(array);

    yunomi::array::Filter11Array f11array(array);

    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.serialize("f11", f11array);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::BitSliceArray array2;
    yunomi::array::Filter11Array f11array2(array2, false);

    deserializer.deserialize("array", array2);
    deserializer.deserialize("f11", f11array2);
    deserializer.close();

    for(uint64_t i = 0; i < 20; ++i){
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 0, i * 25 + 1));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 1, i * 25 + 2));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 2, i * 25 + 3));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 3, i * 25 + 4));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 4, i * 25 + 5));

        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 5, i * 25 + 6));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 6, i * 25 + 7));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 7, i * 25 + 8));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 8, i * 25 + 9));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 9, i * 25 + 10));

        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 10, i * 25 + 11));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 11, i * 25 + 12));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 12, i * 25 + 13));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 13, i * 25 + 14));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 14, i * 25 + 15));

        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 15, i * 25 + 16));
        EXPECT_EQ(1, (uint64_t) f11array2(i * 25 + 16, i * 25 + 17));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 17, i * 25 + 18));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 18, i * 25 + 19));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 19, i * 25 + 20));

        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 20, i * 25 + 21));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 21, i * 25 + 22));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 22, i * 25 + 23));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 23, i * 25 + 24));
        EXPECT_EQ(0, (uint64_t) f11array2(i * 25 + 24, i * 25 + 25));
    }

    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_dualblockindex) {
    std::string path = "h5s_test_read_write_dualblockindex.bin";
    yunomi::array::BitSliceArray array(1024);
    set_array(array);

    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);

    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("array", array);
    serializer.serialize("index", index);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::BitSliceArray  array2;
    decltype(index) index2(array, false);

    deserializer.deserialize("array", array2);
    deserializer.deserialize("index", index2);
    deserializer.close();

    yunomi::succinct::Rank< decltype(index2) > rank(index2);

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

    yunomi::io::File::remove(path);
}

TEST(TestHDF5Serializer, test_read_write_fibonacciarray) {
    std::string path = "h5s_test_read_write_fibonacciarray.bin";

    yunomi::array::FibonacciDataStorage storage;
    for(uint64_t i = 0; i < 1024; ++i){
        storage.push_back(i + 1);
    }

    yunomi::array::FibonacciArray fa(storage);

    yunomi::serialize::HDF5Serializer serializer(path);
    serializer.serialize("storage", storage);
    serializer.serialize("array", fa);
    serializer.close();

    yunomi::serialize::HDF5Deserializer deserializer(path);
    yunomi::array::FibonacciDataStorage storage2;
    yunomi::array::FibonacciArray fa2(storage2, false);
    deserializer.deserialize("storage", storage2);
    deserializer.deserialize("array", fa2);
    deserializer.close();

    for(uint64_t i = 0; i < 1024; ++i){
        EXPECT_EQ(i + 1, fa2[i]);
    }

    yunomi::io::File::remove(path);
}
