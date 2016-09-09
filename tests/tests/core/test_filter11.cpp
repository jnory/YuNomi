#include "gtest/gtest.h"

#include "yunomi/core/filter11.hpp"

yunomi::core::Converted create_expected(uint8_t value, bool carry) {
    uint8_t expected = 0;
    bool flg = carry;
    for(uint8_t i = 0; i < 8; ++i){
        if(((value >> i) & 0x1ULL) > 0){
            if(flg) {
                expected |= (0x1ULL << i);
                flg = false;
            }else{
                flg = true;
            }
        }else{
            flg = false;
        }
    }
    yunomi::core::Converted ret(expected, flg);

    return ret;
}

TEST(TestFilterEleven8bit, test_call_nocarry){
    yunomi::core::FilterEleven8bit fe;
    for(uint16_t i = 0; i < 256; ++i) {
        yunomi::core::Converted expected = create_expected(i, false);
        yunomi::core::Converted actual = fe(i, false);
        EXPECT_EQ(expected, actual);
    }
}

TEST(TestFilterEleven8bit, test_call_carry){
    yunomi::core::FilterEleven8bit fe;
    for(uint16_t i = 0; i < 256; ++i) {
        EXPECT_EQ(create_expected(i, true), fe(i, true));
    }
}

TEST(TestFilterElevenDict, test_call){
    // test FilterElevenDict::dict_ doesn't have mistake.
    constexpr yunomi::core::FilterElevenDict dict;
    for(uint16_t i = 0; i < 256; ++i) {
        EXPECT_EQ(yunomi::core::FilterEleven8bit()(i, false), dict[i][false]);
        EXPECT_EQ(yunomi::core::FilterEleven8bit()(i, true), dict[i][true]);
    }
}

TEST(TestFilter11, test_filter11) {
    yunomi::core::Filter11 filter11;
    yunomi::core::Converted64 conv;
    conv.value = 0xFFFFFFFFFFFFFFFFULL;
    conv.carry = false;
    yunomi::core::Converted64 actual = filter11(conv);
    EXPECT_EQ(0xAAAAAAAAAAAAAAAAULL, actual.value);
    EXPECT_EQ(false, actual.carry);

    conv.value = 0xFFFFFFFFFFFFFFFFULL;
    conv.carry = true;
    actual = filter11(conv);
    EXPECT_EQ(0x5555555555555555ULL, actual.value);
    EXPECT_EQ(true, actual.carry);

    conv.value = 0x0ULL;
    conv.carry = false;
    actual = filter11(conv);
    EXPECT_EQ(0x0ULL, actual.value);
    EXPECT_EQ(false, actual.carry);

    conv.value = 0x0ULL;
    conv.carry = true;
    actual = filter11(conv);
    EXPECT_EQ(0x0ULL, actual.value);
    EXPECT_EQ(false, actual.carry);
}
