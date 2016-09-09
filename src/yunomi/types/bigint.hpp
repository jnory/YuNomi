#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include "yunomi/core/register.hpp"

namespace yunomi {
    namespace types {
        struct uint128_t {
            uint64_t lower;
            uint64_t upper;

            constexpr uint128_t(uint64_t lower): lower(lower), upper(0){
            }

            constexpr uint128_t(uint64_t lower, uint64_t upper): lower(lower), upper(upper){
            }

            template <typename T> constexpr
            uint128_t operator>>(T const width) const {
                return this->rshift(static_cast<std::size_t>(width % 128));
            }

            template <typename T> constexpr
            uint128_t operator<<(T const width) const {
                return this->lshift(static_cast<std::size_t>(width % 128));
            }

            template <typename T> constexpr
            uint128_t operator-(T const &other) const {
                return this->minus(uint128_t(static_cast<uint64_t>(other)));
            }

            constexpr uint128_t operator-(uint128_t const &other) const {
                return this->minus(other);
            }

            constexpr uint128_t operator~() const {
                return uint128_t(~lower, ~upper);
            }

            constexpr uint128_t operator&(uint128_t const &other) const {
                return uint128_t(lower & other.lower, upper & other.upper);
            }

            constexpr uint128_t operator|(uint128_t const &other) const {
                return uint128_t(lower | other.lower, upper | other.upper);
            }

            constexpr operator uint64_t() const {
                return lower;
            }

            constexpr std::size_t n_ones() const {
                return core::Register(lower).n_ones() + core::Register(upper).n_ones();
            }

        private:

            constexpr uint128_t minus(uint128_t const &other) const {
                return uint128_t(
                        lower - other.lower,
                        upper - other.upper - ((lower < other.lower)?1:0)
                );
            }

            constexpr uint128_t rshift(std::size_t width) const {
                return (width == 0)? *this:
                       (width >= 64)? uint128_t((upper >> (width - 64)), 0):
                       uint128_t((upper << (64 - width)) | (lower >> width), upper >> width);
            }

            constexpr uint128_t lshift(std::size_t width) const {
                return (width == 0)? *this:
                       (width >= 64)? uint128_t(0, (lower << (width - 64))):
                       uint128_t(lower << width, (upper << width) | (lower >> (64 - width)));
            }
        };
    }
}
