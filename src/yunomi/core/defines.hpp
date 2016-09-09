#pragma once

#include <cstddef>
#include <cstdint>

constexpr std::size_t UINT64_T_SIZE = 64;
constexpr std::size_t UINT128_T_SIZE_BYTE = 16;

constexpr std::size_t UINT64_T_SIZE_BITS = 6;
constexpr std::size_t UINT128_T_SIZE_BITS = 7;
constexpr std::size_t UINT8_T_SIZE_BITS = 3;
constexpr uint64_t UINT8_T_SIZE_MASK = 0x7ULL;
constexpr uint64_t UINT64_T_SIZE_MASK = 0x3FULL;
constexpr uint64_t UINT128_T_SIZE_MASK = 0x7FULL;
