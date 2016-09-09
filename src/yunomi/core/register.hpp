#pragma once

#include <cstdint>

#include "defines.hpp"

namespace yunomi {

    namespace core {


        class Register {
        public:
            constexpr
            Register(uint64_t value): value_(value){
            }

            constexpr
            uint8_t n_ones() const {
                return __builtin_popcountll(value_);
            }

            constexpr
            uint8_t n_digits() const {
                return UINT64_T_SIZE - __builtin_clzll(value_);
            }

        private:
            uint64_t value_;
        };

    }
}