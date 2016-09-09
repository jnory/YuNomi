#include "gtest/gtest.h"

#include "yunomi/core/register.hpp"
#include "yunomi/core/succinct.hpp"

TEST(TestRegister, test_init){
    yunomi::core::Register reg(10);
}

TEST(TestRegister, test_popcount){
    yunomi::core::Register reg(10);
    EXPECT_EQ(2, reg.n_ones());
}

TEST(TestRegister, test_n_digits){
    yunomi::core::Register reg(4);
    EXPECT_EQ(3, reg.n_digits());
}

