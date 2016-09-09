#include <string>
#include "gtest/gtest.h"
#include "yunomi/io/file.hpp"
#include "yunomi/io/hdf5.hpp"

TEST(TestHDF5File, test_init){
    std::string path = "hdf5file_test_init.bin";
    ASSERT_FALSE(yunomi::io::File::exists(path));

    yunomi::io::HDF5File file(path, "w");
    file.close();
    ASSERT_TRUE(yunomi::io::File::exists(path));

    yunomi::io::File::remove(path);
    ASSERT_FALSE(yunomi::io::File::exists(path));
}

TEST(TestHDF5File, test_read_write){
    std::string path = "hdf5file_test_read_write.bin";

    std::unique_ptr<uint64_t[]> data1(new uint64_t[10]);
    for(uint64_t i = 0; i < 10; ++i){
        data1[i] = i;
    }
    uint64_t data2 = 100;
    uint64_t data3 = 1000;

    {
        yunomi::io::HDF5File file(path, "w");
        auto h1 = file.create_group("h1");
        h1.create_dataset_array("data1", data1.get(), 10);
        h1.create_dataset_immediate("data2", data2);
        auto h2 = h1.create_group("h2");
        h2.create_dataset_immediate("data3", data3);
        file.close();
    }

    {
        yunomi::io::HDF5File file_read(path, "r");
        auto h1 = file_read.group("h1");
        auto read1 = h1.dataset("data1").read_array<uint64_t>(10);
        auto read2 = h1.dataset("data2").read_immediate<uint64_t>();
        auto h2 = h1.group("h2");
        auto read3 = h2.dataset("data3").read_immediate<uint64_t>();
        for(uint64_t i = 0; i < 10; ++i){
            EXPECT_EQ(i, read1[i]);
        }
        EXPECT_EQ(100, read2);
        EXPECT_EQ(1000, read3);
        file_read.close();
    }

    yunomi::io::File::remove(path);
}
