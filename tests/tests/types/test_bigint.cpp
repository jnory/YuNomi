#include "gtest/gtest.h"
#include "yunomi/types/bigint.hpp"

TEST(TestUint128_t, test_init){
    ASSERT_EQ(16, sizeof(yunomi::types::uint128_t));

    yunomi::types::uint128_t value(0);
    EXPECT_EQ(0, (uint64_t)value);

    yunomi::types::uint128_t value2(0, 0);
    EXPECT_EQ(0, (uint64_t)value2);
}

TEST(TestUint128_t, test_lshift){
    yunomi::types::uint128_t value(1);
    auto shift = value << 1;
    EXPECT_EQ(2, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value << 63;
    EXPECT_EQ(0x1ULL << 63, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value << 64;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(1, shift.upper);

    shift = value << 127;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(0x1ULL << 63, shift.upper);

    shift = value << 128;
    EXPECT_EQ(1, shift.lower);
    EXPECT_EQ(0, shift.upper);
}

TEST(TestUint128_t, test_lshift2){
    yunomi::types::uint128_t value(1, 1);
    auto shift = value << 1;
    EXPECT_EQ(2, shift.lower);
    EXPECT_EQ(2, shift.upper);

    shift = value << 63;
    EXPECT_EQ(0x1ULL << 63, shift.lower);
    EXPECT_EQ(0x1ULL << 63, shift.upper);

    shift = value << 64;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(1, shift.upper);

    shift = value << 127;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(0x1ULL << 63, shift.upper);

    shift = value << 128;
    EXPECT_EQ(1, shift.lower);
    EXPECT_EQ(1, shift.upper);
}

TEST(TestUint128_t, test_rshift){
    yunomi::types::uint128_t value(0x1ULL << 63);
    auto shift = value >> 1;
    EXPECT_EQ(0x1ULL << 62, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 63;
    EXPECT_EQ(1, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 64;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 127;
    EXPECT_EQ(0, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 128;
    EXPECT_EQ(0x1ULL << 63, shift.lower);
    EXPECT_EQ(0, shift.upper);
}

TEST(TestUint128_t, test_rshift2){
    yunomi::types::uint128_t value(0x1ULL << 63, 0x1ULL << 63);
    auto shift = value >> 1;
    EXPECT_EQ(0x1ULL << 62, shift.lower);
    EXPECT_EQ(0x1ULL << 62, shift.upper);

    shift = value >> 63;
    EXPECT_EQ(1, shift.lower);
    EXPECT_EQ(1, shift.upper);

    shift = value >> 64;
    EXPECT_EQ(0x1ULL << 63, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 127;
    EXPECT_EQ(1, shift.lower);
    EXPECT_EQ(0, shift.upper);

    shift = value >> 128;
    EXPECT_EQ(0x1ULL << 63, shift.lower);
    EXPECT_EQ(0x1ULL << 63, shift.upper);
}

TEST(TestUint128_t, test_minus){
    yunomi::types::uint128_t a(2);
    yunomi::types::uint128_t b(1);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(1, ans.lower);
    EXPECT_EQ(0, ans.upper);
}

TEST(TestUint128_t, test_minus2){
    yunomi::types::uint128_t a(1);
    yunomi::types::uint128_t b(2);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.lower);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.upper);
}

TEST(TestUint128_t, test_minus3){
    yunomi::types::uint128_t a(0, 2);
    yunomi::types::uint128_t b(0, 1);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(0, ans.lower);
    EXPECT_EQ(1, ans.upper);
}

TEST(TestUint128_t, test_minus4){
    yunomi::types::uint128_t a(0, 1);
    yunomi::types::uint128_t b(0, 2);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(0, ans.lower);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.upper);
}

TEST(TestUint128_t, test_minus5){
    yunomi::types::uint128_t a(2, 2);
    yunomi::types::uint128_t b(1, 1);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(1, ans.lower);
    EXPECT_EQ(1, ans.upper);
}

TEST(TestUint128_t, test_minus6){
    yunomi::types::uint128_t a(1, 1);
    yunomi::types::uint128_t b(2, 2);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.lower);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max() - 1, ans.upper);
}

TEST(TestUint128_t, test_minus7){
    yunomi::types::uint128_t a(2, 1);
    yunomi::types::uint128_t b(1, 2);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(1, ans.lower);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.upper);
}

TEST(TestUint128_t, test_minus8){
    yunomi::types::uint128_t a(1, 2);
    yunomi::types::uint128_t b(2, 1);
    yunomi::types::uint128_t ans = a - b;
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.lower);
    EXPECT_EQ(0, ans.upper);
}

TEST(TestUint128_t, test_minus_int){
    yunomi::types::uint128_t a(1);
    auto ans = a - 1;
    EXPECT_EQ(0, ans.lower);
    EXPECT_EQ(0, ans.upper);
}

TEST(TestUint128_t, test_bitwise_not){
    yunomi::types::uint128_t a(0);
    auto ans = ~a;
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.lower);
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), ans.upper);
}

TEST(TestUint128_t, test_bitwise_or){
    yunomi::types::uint128_t a(6, 3);
    yunomi::types::uint128_t b(3, 6);
    auto ans = a | b;
    EXPECT_EQ(7, ans.lower);
    EXPECT_EQ(7, ans.upper);
}

TEST(TestUint128_t, test_bitwise_and){
    yunomi::types::uint128_t a(6, 3);
    yunomi::types::uint128_t b(3, 6);
    auto ans = a & b;
    EXPECT_EQ(2, ans.lower);
    EXPECT_EQ(2, ans.upper);
}

TEST(TestUint128_t, test_bitwise_and_or){
    yunomi::types::uint128_t a(6, 3);
    yunomi::types::uint128_t b(3, 6);
    yunomi::types::uint128_t c(8, 8);
    auto d = a & b;
    auto ans = d | c;
    EXPECT_EQ(10, ans.lower);
    EXPECT_EQ(10, ans.upper);

    ans = (a & b) | c;
    EXPECT_EQ(10, ans.lower);
    EXPECT_EQ(10, ans.upper);
}

TEST(TestUint128_t, test_bitwise_and_or2){
    yunomi::types::uint128_t a(6, 3);
    yunomi::types::uint128_t b(3, 6);
    yunomi::types::uint128_t c(1, 1);

    yunomi::types::uint128_t ans = (a & b) | (c << 3);
    EXPECT_EQ(10, ans.lower);
    EXPECT_EQ(10, ans.upper);
}


TEST(TestUint128_t, test_cast_uint64_t){
    yunomi::types::uint128_t value(0);
    uint64_t v64 = value;
    EXPECT_EQ(0, v64);
}

TEST(TestUint128_t, test_cast_uint64_t2){
    yunomi::types::uint128_t value(1);
    uint64_t v64 = value;
    EXPECT_EQ(1, v64);
}

TEST(TestUint128_t, test_cast_uint64_t3){
    yunomi::types::uint128_t value(1, 1);
    uint64_t v64 = value;
    EXPECT_EQ(1, v64);
}

TEST(TestUint128_t, test_n_ones){
    yunomi::types::uint128_t value(1, 1);
    EXPECT_EQ(2, value.n_ones());
}
