#pragma once

#include <cstdint>

#include "defines.hpp"

namespace yunomi {

    namespace core {


        /**
         * @brief provide a register-based operations
         */
        class Register {
        public:
            constexpr
            Register(uint64_t value): value_(value){
            }

            /**
            * @brief calculate popcount on the given value.
            * @details
            * For given 64bit `value`, `n_ones()` calculates the number of ones appeared in the value on binary digits format.
            * This function works exactly same as:
            * @code
            * constexpr uint8_t n_ones() const {
            *     uint8_t count = 0;
            *     for(int i = 0; i < 64; i++){
            *         if(value & 0x1ULL==0){
            *             count++;
            *         }
            *         value = value >> 1;
            *     }
            *     return count;
            * }
            * @endcode
            */
            constexpr
            uint8_t n_ones() const {
                return __builtin_popcountll(value_);
            }

            /**
            * @brief calculate the number of digits on binary-digit form for given `value`.
            */
            constexpr
            uint8_t n_digits() const {
                return UINT64_T_SIZE - __builtin_clzll(value_);
            }

        private:
            uint64_t value_;
        };

    }
}