#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "yunomi/io/file.hpp"

TEST(TestFile, test_init){
    std::string path = "file_test_init.bin";
    ASSERT_FALSE(yunomi::io::File::exists(path));

    std::ofstream ofs(path);
    ofs << "test";
    ofs.close();

    ASSERT_TRUE(yunomi::io::File::exists(path));
    yunomi::io::File::remove(path);
    ASSERT_FALSE(yunomi::io::File::exists(path));
}

