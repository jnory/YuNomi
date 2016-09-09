#include "gtest/gtest.h"

#include "yunomi/core/succinct.hpp"

TEST(TestSelectDictImpl, test_call){
    // test SelectDictImpl::dict_ doesn't have mistake.
    constexpr yunomi::core::SelectDict dict;
    for(uint16_t i = 0; i < 256; ++i) {
        for(uint8_t j = 0; j < 8; ++j) {
            EXPECT_EQ(yunomi::core::Select8Bit()(i, j), dict(i, j));
        }
    }
}

TEST(TestRegisterSuccinct, test_select){
    yunomi::core::RegisterSuccinct reg(4);
    EXPECT_EQ(2, reg.select(0));
    EXPECT_EQ(yunomi::core::RegisterSuccinct::not_found(), reg.select(1));
}

TEST(TestRegisterSuccinct, test_select2){
    yunomi::core::RegisterSuccinct reg(0x1ULL << 13);
    EXPECT_EQ(13, reg.select(0));
    EXPECT_EQ(yunomi::core::RegisterSuccinct::not_found(), reg.select(1));
}

TEST(TestRegisterSuccinct, test_select3){
    yunomi::core::RegisterSuccinct reg((0x1ULL << 13) + (0x1ULL << 4));
    EXPECT_EQ(4, reg.select(0));
    EXPECT_EQ(13, reg.select(1));
    EXPECT_EQ(yunomi::core::RegisterSuccinct::not_found(), reg.select(2));
}

TEST(TestRegisterSuccinct, test_select4){
    yunomi::core::RegisterSuccinct reg((0x1ULL << 60) + (0x1ULL << 20) + (0x1ULL << 4));
    EXPECT_EQ(4, reg.select(0));
    EXPECT_EQ(20, reg.select(1));
    EXPECT_EQ(60, reg.select(2));
    EXPECT_EQ(yunomi::core::RegisterSuccinct::not_found(), reg.select(3));
}
